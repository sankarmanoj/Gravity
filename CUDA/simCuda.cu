#include<stdio.h>
#include<math.h>
#define fg 0.0000001
#define fe 0.0001
#define damp 0.0001
#define critical_factor  0
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define STRONG_FORCE 0.001
const int MAX_SIZE = 1023;
// [self.x,self.y,self.z,self.vx,self.vy,self.vz,self.m,self.charge]
//    0     1       2       3       4       5     6         7
__global__ void myop( float * d_output, float *d_input,int size)
{
  __shared__ float buffer[MAX_SIZE*8];
  int id = threadIdx.x;
  float my[8];
  for(int i = 0; i< 8; i++)
  {
    float temp  =d_input[8*id+i];
  buffer[8*id+i]=temp;
  my[i]=temp;

  }
  __syncthreads();

  for(int i = 0; i< size; i++)
  {
    if(i==id)
    continue;
    float other[8];
    for(int j = 0; j<8;j++)
    {
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


  buffer[8*id+3]=my[3];
  buffer[8*id+4]=my[4];
  buffer[8*id+5]=my[5];
  buffer[8*id]+=my[3];
  buffer[8*id+1]+=my[4];
  buffer[8*id+2]+=my[5];
  __syncthreads();
  for(int i = 0; i< 8; i++)
  {
    d_output[8*id+i]=buffer[8*id+i];
  }

}
float * calcAndUpdate(float * input, int size)
{
  float * d_input, * d_output;
  float *result = (float *)malloc((size*8)*sizeof(float));
  if(size>MAX_SIZE)
  {
    return 0;

  }
  cudaMalloc(&d_input,(size*8)*sizeof(float));
  cudaMalloc(&d_output,(size*8)*sizeof(float));
//  printf("cuda Size = %lu",(size*8)*sizeof(float));
  cudaMemcpy(d_input,input,(size*8)*sizeof(float),cudaMemcpyHostToDevice);
  myop<<<1,size>>>(d_output,d_input,size);
  cudaMemcpy(result,d_output,(size*8)*sizeof(float),cudaMemcpyDeviceToHost);
  // for(int i = 0; i< 6*size; i++)
  // {
  //   printf("%f \t",result[i]);
  //   if(i%6==0)
  //   {
  //     printf("\n");
  //   }
  // }
  free(input);
  return result;

}
