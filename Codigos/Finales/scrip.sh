#!/bin/bash
# -*- ENCODING: UTF-8 -*-


if type gcc > /dev/null 2>&1; then
echo "gcc Instalado"
echo "Recorriendo documentos"
echo "---------------"
 
for LINEA in `cat fuentes.dat ` #LINEA guarda el resultado del fichero datos.txt
do
    NOMBRE=`echo $LINEA | cut -d ":" -f1` #Extrae nombre
    echo "$NOMBRE tiene años." #Muestra resultado.
done
gcc numProc.c -fopenmp -lm -o Knn/Procesadores.out

gcc Knn_secuencial.c -lm -o Knn/Secuencial.out
echo "Knn/Secuencial.out" >> Knn/rutas.dat
gcc Knn_multihilos.c -fopenmp -lm -o Knn/Multihilos.out
echo "Knn/Multihilos.out" >> Knn/rutas.dat
else
echo "Instalando gcc"
apt-get install gcc
sh scrip.sh
fi

if type icc > /dev/null 2>&1; then
echo "icc Instalado"
icc Knn_exhaustivo_MIC.c -openmp -lm -o Mic.out
echo "Knn/Mic.out" >> Knn/rutas.dat
else
echo "icc no instalado, este compilador es requerido para utilizar la Xeon Phi"
echo "" >> Knn/rutas.dat
fi

if type nvcc > /dev/null 2>&1; then
echo "Nvcc Instalado"
nvcc libreria.cu -gencode arch=compute_35,code=sm_35 -o Knn/Gpu.out
echo "Knn/Gpu.out" >> Knn/rutas.dat
mv -f final.cu Knn/final.cu
else
echo "nvcc no instalado, este compilador es requerido para utilizar la Nvidia GPU"
echo "" >> Knn/rutas.dat
fi

echo "Archivo rutas.dat"
cat Knn/rutas.dat

exit