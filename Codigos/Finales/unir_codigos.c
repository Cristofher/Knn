#include <stdio.h>
#include <string.h>

int system(const char *command);

int main(int argc, char *argv[])
{

	if (argc != 4) {
		printf("\nError :: Ejecutar como : a.out archivo_BD Num_elem archivo_queries Num_queries N_THREADS numero_K Dimension_objetos\n");
		return 0;
	}
	char str_f[256];
	FILE *p, *q, *r, *fp;
	char c;
	char textoExtraido [1000],nombre_funcion[1000];
	char file1[200], file2[200], file3[200];
	char cadena[1000];
	char cad[1000];

	sprintf(str_f, "%s", argv[1]);
	printf("\nAbriendo %s... ", argv[1]);
	fflush(stdout);
	p = fopen(str_f, "r+");
	printf("OK\n");

	sprintf(str_f, "%s", argv[2]);
	printf("\nAbriendo %s... ", argv[2]);
	fflush(stdout);
	q = fopen(str_f, "r+");
	printf("OK\n");

	sprintf(str_f, "%s", argv[3]);
	printf("\nCreando %s... ", argv[3]);
	fflush(stdout);
	r = fopen(str_f, "w");
	printf("OK\n");

	fflush(stdout);

	p=fopen(file1, "r+");
	q=fopen(file2, "r+");
	r=fopen(file3, "w");
	char palabra[1000] = "->Funcion<-";
	char argumentos[1000] = "<<< N_BLOQUES, T_per_BLOCK>>> (Elems, (int)pitch, HEAPS_dev, (int)pitch_H, QUERY_dev, (int)pitch_Q, arr_Dist, (int)pitch_Dist, Q*cont, res_final);";


	while(!feof(p))
	{
		if (fgets(cadena, 1000, p) != NULL)
		{
			if (strcmp(cadena,palabra)==10)
			{
				printf("Entro aca");
				fprintf(r, "%s", cadena);
				for (int i = 0; i < 1000; ++i)
				{
					fprintf(r, " ");
				}
				printf("encontre\n");  
			}
			else{
				fprintf(r, "%s", cadena);
			}
		}
	}
	fprintf(r, "\n");
	fprintf(r, "\n");

	while(!feof(q))
	{
		fscanf(q, "%c", &c);
		fprintf(r, "%c", c);
	}

	rewind(q);

	if (fgets(textoExtraido, 256, q)!= NULL)
	{
		printf("%s",textoExtraido);
	}

	fclose(r);
	fp=fopen(file3, "rw+");
	rewind(fp);



	while(!feof(fp))
	{
		if (fgets(cadena, 256, fp) != NULL)
		{
			if (strcmp(cadena,palabra)==0)
			{
				printf("reemplazar\n");  
				fprintf( fp,"%s",textoExtraido);
			}
		}
	}

	fseek( fp, 1862, SEEK_SET );
	int len = strlen(textoExtraido);
	textoExtraido[len-1]=';';
	printf("%d\n", len);
	fprintf( fp,"%s",textoExtraido);

	int largo = 0, aux = 0,var=0;
	while (textoExtraido[largo]!='\0'){

		if(textoExtraido[largo]==95){
			aux++;
		}
		largo++;
		if(aux==4){
			while(textoExtraido[largo] != 40){
				cad[var] = textoExtraido[largo];
				var++;
				largo++;
			}
			if (textoExtraido[largo]==40)
			{
				break;
			}
		}
		if(aux > 4){
			break;
		}
	}

	largo = 0,aux=0, var = 0;
	while (cad[largo]!='\0'){

		if(cad[largo]==32){
			printf("Entro\n");
			aux++;
		}
		largo++;
		if(aux==2){
			while(cad[largo] != 32){
				nombre_funcion[var] = cad[largo];
				var++;
				largo++;
			}
			if (cad[largo]==32)
			{
				break;
			}
		}
		if(aux > 2){
			break;
		}
	}

	printf("NOMBRE: %s\n",nombre_funcion);
	strcat(nombre_funcion,argumentos);

	printf("FUNCION: %s\n",nombre_funcion);

	fclose(p);
	fclose(q);
	fclose(fp);
	return 0; 
}