#!/bin/bash
# -*- ENCODING: UTF-8 -*-


echo "algo"

for LINEA in `cat /usr/lib/knn/Knn/Gpu/Fuentes/fuentes.dat ` #LINEA guarda el resultado del fichero datos.txt
do
    NOMBRE=`echo $LINEA | cut -d ":" -f1` #Extrae nombre
    gcc $NOMBRE -fopenmp -lm -o Knn/Procesadores.out
    echo "$NOMBRE tiene años." #Muestra resultado.
done
