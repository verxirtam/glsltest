/*
 * =====================================================================================
 *
 *       Filename:  glsltest_cuda.cu
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月11日 18時15分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */




__global__
void moveVAO_cuda_kernel(float* v)
{
	unsigned int tx = threadIdx.x;
	unsigned int ty = threadIdx.y;
	if(ty == 2)
	{
		int i = tx * 3 + ty;
		v[i] += 0.025;
		v[i] = ( v[i] > 1.0 ) ? -1.0f : v[i];
	}
}



void moveVAO_cuda(float* v, int vc)
{
	dim3 g(1,1,1);
	dim3 b(vc, 3, 1);
	moveVAO_cuda_kernel<<<g, b>>>(v);
}
