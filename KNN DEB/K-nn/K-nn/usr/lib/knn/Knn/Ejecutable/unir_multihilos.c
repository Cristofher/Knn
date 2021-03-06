#include <stdio.h>
#include <string.h>

int existsFile(char* filename);
int system(const char *command);

int main(int argc, char *argv[])
{

	if (argc != 4) {
		printf("\nError :: Ejecutar como : salida.out file1 file2 file3\n");
		return 0;
	}

	FILE *p, *q, *r, *fp, *temp;
	char c;
	char textoExtraido [1000],nombre_funcion[1000];
	char file1[200], file2[200], file3[200];
	char cadena[1000],cad[1000];

	sprintf(file1, "%s", argv[1]);
	printf("\nAbriendo %s... ", argv[1]);
	fflush(stdout);
	p = fopen(file1, "r+");
	printf("OK\n");

	sprintf(file2, "%s", argv[2]);
	printf("\nAbriendo %s... ", argv[2]);
	fflush(stdout);
	q = fopen(file2, "r+");
	printf("OK\n");

	sprintf(file3, "%s", argv[3]);
	printf("\nCreando %s... ", argv[3]);
	fflush(stdout);
	r = fopen(file3, "w");
	printf("OK\n");

	fflush(stdout);

	char palabra[1000] = "->Funcion<-";
	char llamada_funcion[256];
	char cade[256];

	while(!feof(p))
	{
		if (fgets(cadena, 1000, p) != NULL)
		{
			if (strcmp(cadena,palabra)==10)
			{
				//printf("Entro\n");
				while(!feof(q))
				{
					fscanf(q, "%c", &c);
					fprintf(r, "%c", c);
				}

			}
			else{
				fprintf(r, "%s", cadena);
			}
		}
	}
	fprintf(r, "\n");
	fprintf(r, "\n");


	fclose(p);
	fclose(q);
	fclose(r);

	int existe = existsFile(argv[3]);
	if (existe == 1){
		char programa[500];
		sprintf(programa, "gcc %s -o %s", argv[3],argv[4]);
		printf("%s\n",programa );
		system(programa);
	}else{
		printf("ERROR\n");
	}
		
		printf("AFUERA");
		char programa[500];
		sprintf(programa, "gcc %s -o %s", argv[3],argv[4]);
		printf("%s\n",programa );
		system(programa);
		printf("AFUERA SALIO");

	return 0; 
}

int existsFile(char* filename) {
	FILE* f = NULL;
	f = fopen(filename,"r");
	if (f == NULL) 
		return 0;
	else {
		fclose(f);
		return 1;
	}
}