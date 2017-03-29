#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void runDE(
	__read_only double *rawSignal,
	__constant int S,
	__global double **x,
	__constant double epsilon)
{
	//Create initial agents
	
}