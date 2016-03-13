#include<stdio.h>
#include <signal.h>
#define fg 0.1
#define fe 100
#define damp 0.01
#define critical_factor  0.008
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
const int MAX_SIZE = 500;
// [self.x,self.y,self.vx,self.vy,self.m,self.charge]
//    0     1       2       3       4       5
__global__ void myop( float * d_output, float *d_input,int size)
{
  __shared__ float buffer[MAX_SIZE*6];
  int id = threadIdx.x;
  float my[6];
  for(int i = 0; i< 6; i++)
  {
    float temp  =d_input[6*id+i];
  buffer[6*id+i]=temp;
  my[i]=temp;

  }
  __syncthreads();

  for(int i = 0; i< size; i++)
  {
    if(i==id)
    continue;
    float other[6];
    for(int j = 0; j<6;j++)
    {
      other[j]=buffer[6*i+j];
    }
    float dx = my[0]-other[0];
    float dy = my[1]-other[1];
    float r = sqrtf(dx*dx+dy*dy);
    float fx =-(fg*my[4]*other[4]-fe*my[5]*other[5])*dx;
    fx=fx/(r*r*r);
    float fy =-(fg*my[4]*other[4]-fe*my[5]*other[5])*dy;
    fy=fy/(r*r*r);
    if(r>10)
    {
      my[2]+=(fx/my[4]);
      my[3]+=(fy/my[4]);
    }
    else if(r>3)
    {
      my[2]+=fx*critical_factor*r*r;
      my[3]+=fy*critical_factor*r*r;
      my[2]-=damp*(my[2]-other[2])*abs(my[2])*abs(my[5]-other[5])*abs(my[1]-other[1]);
      my[3]-=damp*(my[3]-other[3])*abs(my[3])*abs(my[5]-other[5])*abs(my[2]-other[2]);
    }

  }
  buffer[6*id+2]=my[2];
  buffer[6*id+3]=my[3];
  buffer[6*id]+=my[2];
  buffer[6*id+1]+=my[3];
  if(buffer[6*id]>SCREEN_WIDTH)
  {
    buffer[6*id+2]=-buffer[6*id+2];
    buffer[6*id]=SCREEN_WIDTH-2;
  }
  else if(buffer[6*id]<0)
  {
    buffer[6*id+2]=-buffer[6*id+2];
    buffer[6*id]=2;
  }
  if(buffer[6*id+1]>SCREEN_HEIGHT)
  {
    buffer[6*id+3]=-buffer[6*id+3];
    buffer[6*id+1]=SCREEN_HEIGHT-2;
  }
  else if(buffer[6*id+1]<0)
    {
      buffer[6*id+3]=-buffer[6*id+3];
      buffer[6*id+1]=2;
    }
  __syncthreads();
  for(int i = 0; i< 6; i++)
  {
    d_output[6*id+i]=buffer[6*id+i];
  }
}
extern "C"{
float * calcAndUpdate(float * input, int size)
{
  float * d_input, * d_output;
  float *result = (float *)malloc((size*6+2)*sizeof(float));
  if(size>500)
  {
    return 0;

  }
  cudaMalloc(&d_input,(size*6)*sizeof(float));
  cudaMalloc(&d_output,(size*6)*sizeof(float));
  cudaMemcpy(d_input,input,(size*6)*sizeof(float),cudaMemcpyHostToDevice);
  myop<<<1,size>>>(d_output,d_input,size);
  cudaMemcpy(result,d_output,(size*6)*sizeof(float),cudaMemcpyDeviceToHost);
  // for(int i = 0; i< 6*size; i++)
  // {
  //   printf("%f \t",result[i]);
  //   if(i%6==0)
  //   {
  //     printf("\n");
  //   }
  // }
  return result;

}
}
int main()
{

}
