#include <stdio.h>
#include <string.h>

int main()
{
	FILE *p, *q, *r, *fp;
	char c;
	char textoExtraido [1000];
	char file1[200], file2[200], file3[200];
	char cadena[1000];
	char cad;
	printf("Escribe el nombre del primer archivo a unir: ");
	scanf("%s", file1);
	printf("Escribe el nombre del segundo archivo a unir: ");
	scanf("%s", file2);
	printf("Escribe el nombre del archivo resultado a crear: ");
	scanf("%s", file3);
	p=fopen(file1, "r+");
	q=fopen(file2, "r+");
	r=fopen(file3, "w");
	char palabra[1000] = "->Funcion<-";

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

	while (fgets(textoExtraido))


	fclose(p);
	fclose(q);
	fclose(fp);
	return 0; 
}