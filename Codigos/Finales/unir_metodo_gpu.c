#include <stdio.h>
#include <string.h>

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
	temp = tmpfile();



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
	char argumentos[1000] = "<<< N_BLOQUES, T_per_BLOCK>>> (Elems, (int)pitch, HEAPS_dev, (int)pitch_H, QUERY_dev, (int)pitch_Q, arr_Dist, (int)pitch_Dist, Q*cont, res_final);";
	char llamada_funcion[256];
	strcpy(llamada_funcion,"Batch_Heap_Reduction<<< N_BLOQUES, T_per_BLOCK>>> (Elems, (int)pitch, HEAPS_dev, (int)pitch_H, QUERY_dev, (int)pitch_Q, arr_Dist, (int)pitch_Dist, Q*cont, res_final);");
	char cade[256];

	while(!feof(p))
	{
		if (fgets(cadena, 1000, p) != NULL)
		{
			if (strcmp(cadena,palabra)==10)
			{
				//fprintf(temp,"%s" , cadena);
				fprintf(temp, "%s", cadena);
				for (int i = 0; i < 1000; ++i)
				{
					fprintf(temp, " ");
				}
				//printf("encontre\n");  
			}
			else{
				fprintf(temp, "%s", cadena);
			}
		}
	}
	fprintf(temp, "\n");
	fprintf(temp, "\n");

	while(!feof(q))
	{
		fscanf(q, "%c", &c);
		fprintf(temp, "%c", c);
	}

	rewind(q);
	if (fgets(textoExtraido, 256, q)!= NULL)

	rewind(temp);

	fseek( temp, 1862, SEEK_SET );
	int len = strlen(textoExtraido);
	textoExtraido[len-1]=';';
	//printf("%d\n", len);
	fprintf( temp,"%s",textoExtraido);

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

	rewind(temp);

	while (!feof(temp)){
		if (fgets(cade, 256, temp) != NULL)
		{
			//printf("%d\n",strcmp(cade,llamada_funcion));
			if (strcmp(cade,llamada_funcion)==10)
			{
				printf("%s\n",nombre_funcion);
				fprintf(r, "%s\n",nombre_funcion);  
			}
			else{
				fprintf(r, "%s", cade);
			}
		}
	}

	fclose(temp);
	fclose(r);

	

	return 0; 
}