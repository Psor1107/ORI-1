#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct escola{
    int id;
    char estado[5];
    char municipio[50];
    char rede[25];
    float media_ciencias_natureza;
    float media_ciencias_humanas;
    float media_linguagem;
    float media_matematica;
    float media_redacao;
}typedef Escola;

struct indicePrimario{
    int id;
    long posicao;
}typedef IndicePrimario;

struct indiceSecundario{
    char estado[5];
    long posicao;
}typedef IndiceSecundario;

void criaIndices (FILE *dados, FILE *dataIndicePrimario, FILE *dataIndiceSecundario){

    char linha[250];
    long posicao = 0;
    
    IndicePrimario indicePrimario;
    IndiceSecundario indiceSecundario;

    while (1) {
        
        posicao = ftell(dados);

        if(fgets(linha, 250, dados) == NULL){
            break;
        }

        indicePrimario.posicao = posicao;
        indiceSecundario.posicao = posicao;

        char *token = strtok(linha, ";"); // id

        indicePrimario.id = atoi(token);
        fwrite(&indicePrimario, sizeof(IndicePrimario), 1, dataIndicePrimario);

        token = strtok(NULL, ";"); // estado

        strncpy(indiceSecundario.estado, token, sizeof(indiceSecundario.estado));
        fwrite(&indiceSecundario, sizeof(IndiceSecundario), 1, dataIndiceSecundario);
    }
}

int main(){
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
    criaIndices(dados, dataIndicePrimario, dataIndiceSecundario);
    fclose(dados);
    fclose(dataIndicePrimario);
    fclose(dataIndiceSecundario);
    return 0;
}