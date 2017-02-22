#include <stdio.h>
#include <string.h>

int main()
{
    FILE *p, *q, *r, *fp;
    char c;
    char textoExtraido [50];
    char file1[200], file2[200], file3[200];
    char cadena[256];
    printf("Escribe el nombre del primer archivo a unir: ");
    scanf("%s", file1);
    printf("Escribe el nombre del segundo archivo a unir: ");
    scanf("%s", file2);
    printf("Escribe el nombre del archivo resultado a crear: ");
    scanf("%s", file3);
    p=fopen(file1, "r+");
    q=fopen(file2, "r+");
    r=fopen(file3, "w");
    while(!feof(p))
    {
        fscanf(p, "%c", &c);
        fprintf(r, "%c", c);
    }
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
    char palabra[256] = "->Funcion<-";
    rewind(fp);

    fseek( fp, 1862, SEEK_SET );
    fputs("\n", fp);
    fputs("C Programming Language", fp);

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

fclose(p);
fclose(q);
fclose(fp);
return 0; 
}