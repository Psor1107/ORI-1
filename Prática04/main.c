/*
Gabriel Lucchetta Garcia Sanchez - 828513
*/
#include <stdio.h>
#include <stdlib.h>

struct compras
{
    char ativo;
    char nome[50];
    int qtd;
    float valor;
} typedef Compras;

struct node
{
    Compras *compra;
    struct node *next;
} typedef Node;

void adicionarCompra(Node **head, Compras *compra);

int main()
{
    FILE *arquivo;
    float total = 0.0;
    int tamanho = 0;
    int i = 0;
    Compras *compras = NULL;

    char tempAtivo;
    char tempNome[50];
    int tempQtd;
    float tempValor;

    arquivo = fopen("compras.bin", "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }
    
    while (1) 
    {
        fread(&tempAtivo, sizeof(char), 1, arquivo);
        if (feof(arquivo)) 
        {
            break;
        }
        fread(&tempNome, sizeof(char), 50, arquivo);
        fread(&tempQtd, sizeof(int), 1, arquivo);
        fread(&tempValor, sizeof(float), 1, arquivo);
        total += tempQtd * tempValor;
    }
    fclose(arquivo);
    printf("Total da compra: R$%.2f\n", total);
    return 0;
}

/*int preencherCompras(Compras *compras, FILE *arquivo)
{
    char tempAtivo;
    char tempNome[50];
    int tempQtd;
    float tempValor;

    fread(tempAtivo, sizeof(char), 1, arquivo);
    if (feof(arquivo))
    {
        return 0;
    }
    fread(tempNome, sizeof(char), 50, arquivo);
    fread(tempQtd, sizeof(int), 1, arquivo);
    if (feof(arquivo))
    {
        return 0;
    }
    fread(tempValor, sizeof(float), 1, arquivo);
    if (feof(arquivo))
    {
        return 0;
    }

    compras->ativo = tempAtivo;
    compras->qtd = tempQtd;
    compras->valor = tempValor;
    for (int i = 0; i < 50; i++)
    {
        compras->nome[i] = tempNome[i];
    }
    return 1;
}*/

void adicionarCompra(Node **head, Compras *compra)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->compra = compra;
    newNode->next = *head;
    *head = newNode;
}