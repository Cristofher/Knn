#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *archivo;
	char caracter;
	
	archivo = fopen("base.c","r");
	
	if (archivo == NULL)
        {
            printf("\nError de apertura del archivo. \n\n");
        }
        else
        {
            printf("\nEl contenido del archivo de prueba es \n\n");
            while (feof(archivo) == 0)
	    {
                caracter = fgetc(archivo);
		printf("%c",caracter);
	    }
        }
        fclose(archivo);
	return 0;
}