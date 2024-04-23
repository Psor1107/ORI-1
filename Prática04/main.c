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
    int RRN;
    struct node *next;
} typedef Node;

int removeEntry(Node **free, int i, FILE *arquivo);
int addEntry(Node **free_node, FILE *arquivo, Compras *data);

void printDeletedNode(Node *free);
void printFile();

float getTotal(FILE *arquivo);

int main()
{
    FILE *arquivo = NULL;
    Node *deletedNode = NULL;
    float total = 0.0;

    //Caso de teste
    
    //addEntry(&deletedNode, arquivo, &(Compras){'1', "macarrao", 2, 5.0});
    //removeEntry(&deletedNode, 2, arquivo);
    //addEntry(&deletedNode, arquivo, &(Compras){'1', "desinfetante", 6, 4.0});

    //Prints de arquivo
    
    printFile();
    printDeletedNode(deletedNode);
    
    total = getTotal(arquivo);
    printf("Total da compra: R$%.2f\n", total);
    
    return 0;
}

int addEntry(Node **free_node, FILE *arquivo, Compras *data)
{
    arquivo = fopen("compras.bin", "rb+");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }
    if(*free_node == NULL){
        fseek(arquivo, 0, SEEK_END);
        fwrite(&data->ativo, sizeof(char), 1, arquivo);
        fwrite(&data->nome, sizeof(char), 51, arquivo);
        fwrite(&data->qtd, sizeof(int), 1, arquivo);
        fwrite(&data->valor, sizeof(float), 1, arquivo);
    }else{
        Node *aux = *free_node;
        fseek(arquivo, (aux->RRN*sizeof(Compras)), SEEK_SET);
        fwrite(&data->ativo, sizeof(char), 1, arquivo);
        fwrite(&data->nome, sizeof(char), 51, arquivo);
        fwrite(&data->qtd, sizeof(int), 1, arquivo);
        fwrite(&data->valor, sizeof(float), 1, arquivo);
        *free_node = aux->next;
        free(aux);
    }
    fclose(arquivo);
    return 0;
}

int removeEntry(Node **free, int i, FILE *arquivo)
{
    arquivo = fopen("compras.bin", "rb+");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }
    fseek(arquivo, (i*sizeof(Compras)), SEEK_SET);
    fwrite("0", sizeof(char), 1, arquivo);
    
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->RRN = i;
    newNode->next = *free;
    *free = newNode;
    fclose(arquivo);
    return 0;
}

float getTotal(FILE *arquivo)
{
    
    Compras buffer;
    float total = 0.0;
    arquivo = fopen("compras.bin", "rb+");
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
        }else{
            fseek(arquivo, sizeof(Compras) - sizeof(char), SEEK_CUR);
        }    
    }
    fclose(arquivo);
    return total;
}

void printFile()
{
    Compras buffer;
    FILE *arquivo = fopen("compras.bin", "rb");
    fseek(arquivo, 0, SEEK_SET);
    while (1) 
    {
        fread(&buffer.ativo, sizeof(char), 1, arquivo);
        if(feof(arquivo)){break;} 
        if (buffer.ativo != '0') 
        {
            fread(&buffer.nome, sizeof(char), 51, arquivo);
            fread(&buffer.qtd, sizeof(int), 1, arquivo);
            fread(&buffer.valor, sizeof(float), 1, arquivo);
            //print buffer
            printf("Nome: %s\n", buffer.nome);
            printf("Quantidade: %d\n", buffer.qtd);
            printf("Valor: %.2f\n\n", buffer.valor);
        }else{
            fseek(arquivo, sizeof(Compras) - sizeof(char), SEEK_CUR);
        }  
    }
    fclose(arquivo);
}

void printDeletedNode(Node *free)
{
    Node *aux = free;
    printf("RRN Limpos: ");
    while (aux != NULL)
    {
        printf("%d", aux->RRN);
        aux = aux->next;
    }
    printf("\n");
}