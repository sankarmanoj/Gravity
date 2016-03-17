#include<stdio.h>
#include<math.h>
#define fg 0.00001
#define fe 0.001
#define damp 0.0001
#define critical_factor  0
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define STRONG_FORCE 0.001
const int BLOCK_SIZE = 1024;
// [self.x,self.y,self.z,self.vx,self.vy,self.vz,self.m,self.charge]
//    0     1       2       3       4       5     6         7
__global__ void myop( float *d_input)
{
  __shared__ float buffer[BLOCK_SIZE*8];
  int myid = threadIdx.x+BLOCK_SIZE*blockIdx.x;
  int otherid = threadIdx.x +BLOCK_SIZE*blockIdx.y;
  // printf("myid=%d, otherid=%d, threadIdx=%d, blockX=%d, blockY=%d \n",myid,otherid,threadIdx.x,blockIdx.x,blockIdx.y);
  float my[8];
  for(int i = 0; i< 8; i++)
  {
    my[i]=d_input[8*myid+i];
    buffer[8*threadIdx.x + i]=d_input[8*otherid+i];
    }
  __syncthreads();


  for(int i = 0; i< BLOCK_SIZE; i++)
  {
    if(blockIdx.x==blockIdx.y && threadIdx.x==i)
    continue;
    float other[8];
    for(int j = 0; j<8;j++)
    {
    //      printf("%f[%d],(%d,%d) \n",buffer[8*i+j],8*i+j,myid,blockIdx.y);
      other[j]=buffer[8*i+j];
    }
    float dx = my[0]-other[0];
    float dy = my[1]-other[1];
    float dz = my[2]-other[2];
    float r = sqrtf(dx*dx+dy*dy+dz*dz);
    float fx =-(fg*my[6]*other[6]-fe*my[7]*other[7])*dx;
    fx=fx/(r*r*r);
    float fy =-(fg*my[6]*other[6]-fe*my[7]*other[7])*dy;
    fy=fy/(r*r*r);
    float fz =-(fg*my[6]*other[6]-fe*my[7]*other[7])*dz;
    fz=fz/(r*r*r);
    if(fx!=fx || fy!=fy||isnan(fy))
    continue;
    if(r>0.2f)
    {
      my[3]+=(fx/my[6]);
      my[4]+=(fy/my[6]);
      my[5]+=(fz/my[6]);
    }
    else if(r<0.2f&r>0.01f)
    {
      if((my[7]+other[7])>1.9f)
      {
        my[3]-=(dx/r)*STRONG_FORCE;
        my[4]-=(dy/r)*STRONG_FORCE;
        my[5]-=(dz/r)*STRONG_FORCE;
        my[3]-=(my[3]-other[3])*damp;
        my[4]-=(my[4]-other[4])*damp;
        my[5]-=(my[5]-other[5])*damp;
      }
    }

  }


my[3]-=d_input[8*myid+3];
my[4]-=d_input[8*myid+4];
my[5]-=d_input[8*myid+5];
  for(int i = 3; i< 6; i++)
  {

    atomicAdd(&d_input[8*myid+i],my[i]);
  }
  __syncthreads();
  if(blockIdx.y==0)
  {
    for(int i = 0; i< 3; i++)
    {
      d_input[8*myid+i]+=d_input[8*myid+i+3];
    }

  }


}
float * calcAndUpdate(float * input, int size)
{
  float * d_input, * d_output;
  float *result = (float *)malloc((size*8)*sizeof(float));
  if(!(size%BLOCK_SIZE==0))
  {
    printf("Size is not a multiple of %d",BLOCK_SIZE);
    exit(0);
  }
  cudaMalloc(&d_input,(size*8)*sizeof(float));
  cudaMalloc(&d_output,(size*8)*sizeof(float));
//  printf("cuda Size = %lu",(size*8)*sizeof(float));
  cudaMemcpy(d_input,input,(size*8)*sizeof(float),cudaMemcpyHostToDevice);
  int numberOfBlocks = size/BLOCK_SIZE;
  dim3 blocks(numberOfBlocks,numberOfBlocks);
  myop<<<blocks,BLOCK_SIZE>>>(d_input);
  cudaError_t errSync  = cudaGetLastError();
  cudaError_t errAsync = cudaDeviceSynchronize();
  if (errSync != cudaSuccess)
    printf("Sync kernel error: %sn", cudaGetErrorString(errSync));
  if (errAsync != cudaSuccess)
    printf("Async kernel error: %sn", cudaGetErrorString(errAsync));
  cudaMemcpy(result,d_input,(size*8)*sizeof(float),cudaMemcpyDeviceToHost);
  free(input);
  return result;

}
