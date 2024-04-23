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
    Compras compra;
    struct node *next;
} typedef Node;

struct DeletedNode
{
    int RRN;
    struct DeletedNode *next;
} typedef DeletedNode;

int removeNode(Node **head, DeletedNode *free, int RRN);
void addNode(Node **head, DeletedNode *free, Compras *compra);

int main()
{
    FILE *arquivo;
    Compras buffer;
    Node *node = NULL;
    DeletedNode *deletedNode = NULL;
    float total = 0.0;

    //removeNode(&node, deletedNode, 0);

    arquivo = fopen("compras.bin", "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    while (1) 
    {
        fread(&buffer.ativo, sizeof(char), 1, arquivo);
        if(feof(arquivo)){break;} 
        if (buffer.ativo != '0') 
        {
            fread(&buffer.nome, sizeof(char), 51, arquivo);
            fread(&buffer.qtd, sizeof(int), 1, arquivo);
            fread(&buffer.valor, sizeof(float), 1, arquivo);
            total += buffer.qtd * buffer.valor;
            addNode(&node, deletedNode, &buffer);
        }else{
            fseek(arquivo, sizeof(Compras)-sizeof(char), SEEK_CUR);
        }    
    }
    fclose(arquivo);
    printf("Total da compra: R$%.2f\n", total);
    return 0;
}

void addNode(Node **head, DeletedNode *free, Compras *compra)
{
    if(free == NULL){
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->compra = *compra;
        newNode->next = *head;
        *head = newNode;
        return;
    }else{
        
    }
}

int removeNode(Node **head, DeletedNode *free, int RRN)
{
    FILE *arquivo = fopen("compras.bin", "rb+");
    fseek(arquivo, (RRN*sizeof(Compras)), SEEK_SET);
    fwrite("0", sizeof(char), 1, arquivo);
    fclose(arquivo);
    return 0;
}