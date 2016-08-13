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
	
	int i = tx * 3 + 2;
	v[i] += 0.025;
	v[i] = ( v[i] > 1.0 ) ? -1.0f : v[i];
}



void moveVAO_cuda(float* v, int vc)
{
	moveVAO_cuda_kernel<<<1, vc>>>(v);
}
