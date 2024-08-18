#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void match(FILE *dados1, FILE *dados2, FILE *saida)
{

    fseek(dados1, 0, SEEK_SET), fseek(dados2, 0, SEEK_SET), fseek(saida, 0, SEEK_SET);

    char c1[25], c2[25];
    char *res1 = fgets(c1, 25, dados1);
    char *res2 = fgets(c2, 25, dados2);

    while (res1 != NULL && res2 != NULL)
    {
        if (strcmp(c1, c2) < 0)
        {
            res1 = fgets(c1, 25, dados1);
        }
        else if (strcmp(c1, c2) > 0)
        {
            res2 = fgets(c2, 25, dados2);
        }
        else
        {
            fwrite(c1, 1, strlen(c1), saida);
            res1 = fgets(c1, 25, dados1);
            res2 = fgets(c2, 25, dados2);
        }
    }
}

void merge(FILE *dados1, FILE *dados2, FILE *saida)
{

    fseek(dados1, 0, SEEK_SET), fseek(dados2, 0, SEEK_SET), fseek(saida, 0, SEEK_SET);

    char c1[25], c2[25];
    char *res1 = fgets(c1, 25, dados1);
    char *res2 = fgets(c2, 25, dados2);

    while (res1 != NULL && res2 != NULL)
    {
        if (strcmp(c1, c2) < 0)
        {
            fwrite(c1, 1, strlen(c1), saida);
            res1 = fgets(c1, 25, dados1);
        }
        else if (strcmp(c1, c2) > 0)
        {
            fwrite(c2, 1, strlen(c2), saida);
            res2 = fgets(c2, 25, dados2);
        }
        else
        {
            fwrite(c1, 1, strlen(c1), saida);
            res1 = fgets(c1, 25, dados1);
            res2 = fgets(c2, 25, dados2);
        }
    }

    while (res1 != NULL)
    {
        fwrite(c1, 1, strlen(c1), saida);
        res1 = fgets(c1, 25, dados1);
    }

    // ops...
    fprintf(saida, "\n");

    while (res2 != NULL)
    {
        fwrite(c2, 1, strlen(c2), saida);
        res2 = fgets(c2, 25, dados2);
    }
}

int main()
{
    FILE *dados1 = fopen("lista1.txt", "r");
    FILE *dados2 = fopen("lista2.txt", "r");

    FILE *saidaMatch = fopen("lista3.txt", "w");
    FILE *saidaMerge = fopen("lista4.txt", "w");

    match(dados1, dados2, saidaMatch);
    merge(dados1, dados2, saidaMerge);

    fclose(dados1);
    fclose(dados2);
    fclose(saidaMatch);
    fclose(saidaMerge);

    return 0;
}