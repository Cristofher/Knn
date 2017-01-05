#!/bin/bash
# -*- ENCODING: UTF-8 -*-


if type gcc > /dev/null 2>&1; then
echo "gcc Instalado"
echo "Recorriendo documentos"
echo "---------------"
 
for LINEA in `cat Knn/Secuenciales/Fuentes/fuentes.dat ` #LINEA guarda el resultado del fichero datos.txt
do
    NOMBRE=`echo $LINEA | cut -d ":" -f1` #Extrae nombre
    echo "$NOMBRE tiene años." #Muestra resultado.
done
gcc numProc.c -fopenmp -lm -o Knn/Procesadores.out