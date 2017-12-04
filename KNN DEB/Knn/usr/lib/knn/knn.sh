#!/bin/bash
# -*- ENCODING: UTF-8 -*-


if type java > /dev/null 2>&1; then
echo "Java Instalado"

if type gcc > /dev/null 2>&1; then
echo "gcc Instalado"
echo "Recorriendo documentos"
echo "---------------"
 
gcc Knn/Ejecutable/numProc.c -fopenmp -lm -o Knn/Procesadores.out
Knn/Procesadores.out > Procesadores.dat

gcc Knn/Secuenciales/Fuentes/Knn_secuencial.c -lm -o Knn/Secuencial.out
echo "Secuencial.c" > Knn/Secuenciales/Fuentes/fuentes.dat
echo "Secuencial" > Knn/Secuenciales/Menu/nombres.dat
gcc Knn/Multihilos/Fuentes/Knn_multihilos.c -fopenmp -lm -o Knn/Multihilos.out
echo "Multihilos.c" > Knn/Multihilos/Fuentes/fuentes.dat
echo "Multihilos" > Knn/Multihilos/Menu/nombres.dat
else
echo "Instalando gcc"
sudo apt-get install gcc
sh knn.sh
fi

if type icc > /dev/null 2>&1; then
echo "icc Instalado"
icc Knn/Xeon_Phi/Fuentes/Knn_exhaustivo_MIC.c -openmp -lm -o Mic.out
echo "Mic.c" > Knn/Xeon_Phi/Fuentes/fuentes.dat
echo "Mic" > Knn/Xeon_Phi/Menu/nombres.dat
fi

if type nvcc > /dev/null 2>&1; then
echo "Nvcc Instalado"
nvcc Knn/Gpu/Fuentes/libreria.cu -gencode arch=compute_35,code=sm_35 -o Knn/Gpu.out
echo "libreria.cu" > Knn/Xeon_Phi/Fuentes/fuentes.dat
echo "Gpu" > Knn/Xeon_Phi/Menu/nombres.dat
fi


else
echo "Ud. no cuenta con Java, la instalación no se puede realizar"
fi

exit
