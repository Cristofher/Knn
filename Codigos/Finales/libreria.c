#include <stdio.h>
 
// Print device properties
void printDevProp(cudaDeviceProp devProp)
{
    printf("#define devProp_major          %d\n",  devProp.major);
    printf("#define devProp_minor          %d\n",  devProp.minor);
    printf("#define devProp_globalMemory   %u\n",  devProp.totalGlobalMem);
    printf("#define devProp_sharedMemoryPerBlock %u\n",  devProp.sharedMemPerBlock);
    printf("#define devProp_totalRegistersPerBlock    %d\n",  devProp.regsPerBlock);
    printf("#define devProp_warpSize                     %d\n",  devProp.warpSize);
    printf("#define devProp_maximumMemoryPitch          %u\n",  devProp.memPitch);
    printf("#define devProp_maximumThreadsPerBlock     %d\n",  devProp.maxThreadsPerBlock);
    for (int i = 0; i < 3; ++i)
    printf("#define devProp_maximumDimension%dOfBlock  %d\n", i, devProp.maxThreadsDim[i]);
    for (int i = 0; i < 3; ++i)
    printf("#define devProp_maximumDimension%dOfGrid   %d\n", i, devProp.maxGridSize[i]);
    printf("#define devProp_clockRate                   %d\n",  devProp.clockRate);
    printf("#define devProp_totalConstantMemory         %u\n",  devProp.totalConstMem);
    printf("#define devProp_textureAlignment             %u\n",  devProp.textureAlignment);
    printf("#define devProp_numberOfMultiprocessors     %d\n",  devProp.multiProcessorCount);
    return;
}
 
main(int argc, char *argv[])
{
    // Number of CUDA devices
    int devCount;
    cudaGetDeviceCount(&devCount);
    printf("#define devCount_cudaDevices %d\n", devCount);
 
    // Iterate through devices
    for (int i = 0; i < devCount; ++i)
    {
        // Get device properties
        cudaDeviceProp devProp;
        cudaGetDeviceProperties(&devProp, i);
        printDevProp(devProp);
    }
 
    return 0;
}