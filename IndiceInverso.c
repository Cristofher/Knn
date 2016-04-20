//Cristofher Andrés Rojas Rojas
//Indice Inverso

#define _GNU_SOURCE
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#define MAX 20
#define N_hilos 4

//Estructura tipo Lista utilizada
typedef struct nodo {
    char valor[MAX];
} Nodo;
typedef Nodo Lista;

int main(int argc, char *argv[]) {

	//Número de hilos a crear para trabajar
	omp_set_num_threads(N_hilos);
	
	//Lista para guardar datos de entrada
	Lista **lista;

	//Lista para guardar el indice invertido
	Lista **indice; 
    
    //Para lectura de archivo
    FILE *f_archivo;

    //Vector para almacenar la cantidad de documentos de cada página
    int *Vector;
    int *Vector_Indices;

    //Path archivo, String Palabra & página
    char path[256], palabra[MAX], pagina[MAX];

    //Enteros para leer el archivo
    int num_paginas, num_documentos, num_consultas;

    //Finalización en caso de no ejecutar correctamente el programa
    //Se solicita el ./Programa.out Path_Archivo
    if (argc != 2) {
        printf("Error :: Ejecutar como : a.out archivo\n");
        return 0;
    }

    //Extrae el valor de la variable argv[1] y copia el String a path
    sprintf(path, "%s", argv[1]);
    
    //Abre el archivo, con privilegios de solo lectura (r)
    f_archivo = fopen(path, "r");
    
    //Lee la primera linea del archivo, y se almacena en la variable palabra
    fscanf(f_archivo, "%s", palabra);
    
    //Se pasa de String a Int y se almacena el Número de páginas que se ingresaran
    num_paginas = atoi(palabra);

    //Da mayor tamaño a Vector, para almacenar el numero de documentos de cada página
    Vector = (int*)malloc(num_paginas*sizeof(int));
    
    //Se asigna tamaño para las Listas 
    lista = (Lista **)malloc(sizeof(Lista *)*num_paginas);
    
    //Se continua con la lectura del archivo de las n páginas 
    for (int i = 0; i < num_paginas; ++i) {
    	int j=0;
        
        fscanf(f_archivo, "%s", pagina);
        
        fscanf(f_archivo, "%s", palabra);
        //Se pasa de String a Int y se almacena el Número de documentos que se ingresaran
        num_documentos = atoi(palabra);
        
        //Se almacena el número de documentos en su pagina asociada 
        Vector[i]=num_documentos;

        //Se redefine la dimension para almacenar los String de los documentos
        lista[i] = (Lista *)malloc(sizeof(Lista)*num_documentos);
      	
      	//Copia de String en Estructura lista.Valor
        strcpy(lista[i][j].valor, pagina);
		//printf("%s\n",lista[i][j].valor );

		//Guarda la cantidad de documentos en su página asociada        
        for (j = 1; j < num_documentos+1; ++j) {
            fscanf(f_archivo, "%s", palabra);
            strcpy(lista[i][j].valor, palabra);
  			//printf("%s\n",lista[i][j].valor );
        }
    }

    //Lee string de la cantidad de Consultas
    fscanf(f_archivo, "%s", palabra);

    //Convierte String a Int la cantidad de consultas
    num_consultas = atoi(palabra);
    
    //Se le asigna memoria a las consultas que se realizaran
    indice = (Lista **)malloc(sizeof(Lista *)*num_consultas);
    Vector_Indices = (int*)malloc(num_consultas*sizeof(int)); 
    
    //Se recorren y almacenan las cantidades de consultas
    for (int k = 0; k < num_consultas; ++k) {
    	indice[k] = (Lista *)malloc(sizeof(Lista)*(num_paginas+1));
        fscanf(f_archivo, "%s", palabra);
        strcpy(indice[k][0].valor, palabra);
        //printf("%s\n",indice[k][0].valor );
    }
    //Cierre de archivo
    fclose(f_archivo);


    /*Trabajo con Hilos*/

    int j=0,aux_threads,k,num_indices;
	#pragma omp parallel firstprivate(aux_threads,j,k,num_indices,indice)
    {	
    	int tid = omp_get_thread_num();
    	num_indices=1;
    	for (j = 0; j < num_consultas; j=j+N_hilos)
    	{
    		aux_threads=tid+j;
    		if(aux_threads<num_consultas){
    			//printf("Palabra: %s\n",indice[aux_threads][0].valor );
    			for (int i = 0; i < num_paginas; ++i)
    			{
    				for (k = 1; k < Vector[i]+1; ++k)
    				{
    					int comparador = strcmp(indice[aux_threads][0].valor,lista[i][k].valor);
    					if(comparador == 0){
    						strcpy(indice[aux_threads][num_indices].valor,lista[i][0].valor);
    						Vector_Indices[aux_threads]=num_indices;
    						num_indices++;
    					}
    				}
    			}

    		}
        }
    }
    //Auxiliar para salidas nulas
    int aux;
    for (int i = 0; i < num_consultas; ++i)
    {
    	aux=1;
    	printf("Resultados para \"%s\": ",indice[i][0].valor);
    	for (int j = 1; j < num_paginas+1; ++j)
    	{	
    		if (indice[i][j].valor[0]!='\0')
    		{
    			printf("%s ",indice[i][j].valor);
    		}
    		
    		if (indice[i][j].valor[0]=='\0')
    		{
    			aux++;
    			if(aux==num_paginas+1){
    				printf("No hay resultados.");
    			}
    		}
    	}
    	printf("\n");
    }
    return 0;
}