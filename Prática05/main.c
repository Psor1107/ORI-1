#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 8192

/*
struct escola
{
    int id;
    char estado[5];
    char municipio[50];
    char rede[25];
    float media_ciencias_natureza;
    float media_ciencias_humanas;
    float media_linguagem;
    float media_matematica;
    float media_redacao;
} typedef Escola;
*/

struct indicePrimario
{
    int id;
    long posicao;
} typedef IndicePrimario;

struct indiceSecundario
{
    char estado[5];
    long posicao;
} typedef IndiceSecundario;

int sortPrimario(const void *a, const void *b)
{
    return ((IndicePrimario *)a)->id - ((IndicePrimario *)b)->id;
}

int sortSecundario(const void *a, const void *b)
{
    return strcmp(((IndiceSecundario *)a)->estado, ((IndiceSecundario *)b)->estado);
}

int criaIndices(FILE *dados, FILE *dataIndicePrimario, FILE *dataIndiceSecundario)
{

    char linha[250];
    long posicao = 0;
    int i = 0;
    int capacidade = BASE;

    IndicePrimario *indicePrimario = (IndicePrimario *)malloc(capacidade * sizeof(IndicePrimario));
    IndiceSecundario *indiceSecundario = (IndiceSecundario *)malloc(capacidade * sizeof(IndiceSecundario));

    if (indicePrimario == NULL || indiceSecundario == NULL)
    {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    while (fgets(linha, 250, dados) != NULL)
    {
        if (i >= capacidade)
        {

            capacidade *= 2;
            indicePrimario = (IndicePrimario *)realloc(indicePrimario, (capacidade * sizeof(IndicePrimario)));
            indiceSecundario = (IndiceSecundario *)realloc(indiceSecundario, (capacidade * sizeof(IndiceSecundario)));

            if (indicePrimario == NULL || indiceSecundario == NULL)
            {
                printf("Erro ao alocar memoria!\n");
                return 1;
            }
        }

        posicao = ftell(dados);

        indicePrimario[i].posicao = posicao;
        indiceSecundario[i].posicao = posicao;

        char *token = strtok(linha, ";"); // id

        indicePrimario[i].id = atoi(token);
        // fwrite(&indicePrimario, sizeof(IndicePrimario), 1, dataIndicePrimario);

        token = strtok(NULL, ";"); // estado

        strncpy(indiceSecundario[i].estado, token, sizeof(indiceSecundario[i].estado));
        // fwrite(&indiceSecundario, sizeof(IndiceSecundario), 1, dataIndiceSecundario);

        i++;
    }

    qsort(indicePrimario, i, sizeof(IndicePrimario), sortPrimario);
    qsort(indiceSecundario, i, sizeof(IndiceSecundario), sortSecundario);

    for (int c = 0; c < i; c++)
    {
        fwrite(&indicePrimario[c], sizeof(IndicePrimario), 1, dataIndicePrimario);
        fwrite(&indiceSecundario[c], sizeof(IndiceSecundario), 1, dataIndiceSecundario);
    }

    free(indicePrimario);
    free(indiceSecundario);

    return 0;
}

int main()
{

    int output = 0;

    FILE *dados = fopen("DadosEnem.txt", "r");
    if (dados == NULL)
    {
        printf("Erro ao abrir o arquivo de dados\n");
        return 1;
    }
    FILE *dataIndicePrimario = fopen("IndicePrimario.bin", "wb");
    if (dataIndicePrimario == NULL)
    {
        printf("Erro no arquivo primario\n");
        return 1;
    }
    FILE *dataIndiceSecundario = fopen("IndiceSecundario.bin", "wb");
    if (dataIndiceSecundario == NULL)
    {
        printf("Erro no arquivo secundario\n");
        return 1;
    }

    if(criaIndices(dados, dataIndicePrimario, dataIndiceSecundario)){
        printf("Erro ao criar os indices\n");
        output = 1;
    }else{
        printf("Indices criados com sucesso!\n");
        output = 0;
    }

    fclose(dados);
    fclose(dataIndicePrimario);
    fclose(dataIndiceSecundario);

    return output;
}