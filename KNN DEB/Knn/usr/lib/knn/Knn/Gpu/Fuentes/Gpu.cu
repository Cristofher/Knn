#include <stdio.h>

// Print device properties

FILE *Salida_Multihilo;
void printDevProp(cudaDeviceProp devProp)
{
    fprintf(Salida_Multihilo, "#define devProp_major                                %d\n",  devProp.major);
    fprintf(Salida_Multihilo, "#define devProp_minor                                %d\n",  devProp.minor);
    fprintf(Salida_Multihilo, "#define devProp_globalMemory                         %u\n",  devProp.totalGlobalMem);
    fprintf(Salida_Multihilo, "#define devProp_sharedMemoryPerBlock                 %u\n",  devProp.sharedMemPerBlock);
    fprintf(Salida_Multihilo, "#define devProp_totalRegistersPerBlock               %d\n",  devProp.regsPerBlock);
    fprintf(Salida_Multihilo, "#define devProp_warpSize                             %d\n",  devProp.warpSize);
    fprintf(Salida_Multihilo, "#define devProp_maximumMemoryPitch                   %u\n",  devProp.memPitch);
    fprintf(Salida_Multihilo, "#define devProp_maximumThreadsPerBlock               %d\n",  64);
    for (int i = 0; i < 3; ++i)
        fprintf(Salida_Multihilo, "#define devProp_maximumDimension%dOfBlock        %d\n", i, devProp.maxThreadsDim[i]);
    for (int i = 0; i < 3; ++i)
        fprintf(Salida_Multihilo, "#define devProp_maximumDimension%dOfGrid         %d\n", i, devProp.maxGridSize[i]);
    fprintf(Salida_Multihilo, "#define devProp_clockRate                            %d\n",  devProp.clockRate);
    fprintf(Salida_Multihilo, "#define devProp_totalConstantMemory                  %u\n",  devProp.totalConstMem);
    fprintf(Salida_Multihilo, "#define devProp_textureAlignment                     %u\n",  devProp.textureAlignment);
    fprintf(Salida_Multihilo, "#define devProp_numberOfMultiprocessors              %d\n",  devProp.multiProcessorCount);
    return;
}

int return_major(cudaDeviceProp devProp)
{
    return devProp.major;
}
int return_minor(cudaDeviceProp devProp)
{
    return devProp.minor;
}
int major, minor;
main(int argc, char *argv[])
{
    if (argc != 8)
    {
        printf("\nEjecutar como: a.out archivo_BD archivo_queries N_DB N_QUERIES DIM TOPK NOMBRE_USUARIO\n");
        return 0;
    }
    

    int devCount;
    cudaGetDeviceCount(&devCount);

    system("touch libreria.h"); 
    Salida_Multihilo = fopen("libreria.h", "w");
    char path[256];
    sprintf(path, "/home/%s/Salida.txt",argv[7]);
    printf("%s\n",path );
     
    fprintf(Salida_Multihilo, "#define devCount_cudaDevices         %d\n", devCount);

    fprintf(Salida_Multihilo, "#define DEFINE_N_ELEM                %d\n",  atoi(argv[3]));
    fprintf(Salida_Multihilo, "#define DEFINE_N_QUERIES             %d\n",  atoi(argv[4]));
    fprintf(Salida_Multihilo, "#define DEFINE_DIMENSION             %d\n",  atoi(argv[5]));
    fprintf(Salida_Multihilo, "#define DEFINE_archivo_BD            \"%s\" \n",  argv[1]);
    fprintf(Salida_Multihilo, "#define DEFINE_archivo_queries       \"%s\"\n",  argv[2]);
    fprintf(Salida_Multihilo, "#define DEFINE_TOPK                  %d\n",  atoi(argv[6]));
    fprintf(Salida_Multihilo,"#define DEFINE_PATH		            \"%s\" \n",path);


    // Iterate through devices
    for (int i = 0; i < devCount; ++i)
    {
        // Get device properties
        cudaDeviceProp devProp;
        cudaGetDeviceProperties(&devProp, i);
        printDevProp(devProp);

        major = return_major(devProp);
        minor = return_minor(devProp);

    }

    fclose(Salida_Multihilo);
    char programa[500];
    sprintf(programa, "nvcc final.cu -arch=sm_%d%d -o final.out", major,minor);
    system(programa);
    system("./final.out"); 
    return 0;
}
