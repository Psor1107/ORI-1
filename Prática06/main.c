#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

struct node
{
    Escola data;

    int height;
    struct node *right, *left;
} typedef *ArvBin;

ArvBin *new_ArvBin();
void free_node(ArvBin node);
void free_ArvBin(ArvBin *root);
int height(ArvBin node);
int get_fb(ArvBin node);
void LLRotation(ArvBin *root);
void RRRotation(ArvBin *root);
void LRRotation(ArvBin *root);
void RLRotation(ArvBin *root);
int insert_ArvBin(ArvBin *root, Escola data);
ArvBin minNode(ArvBin node);
int remove_ArvBin(ArvBin *root, int id);
Escola *search_ArvBin(ArvBin *root, int id);
void loadTree(FILE *dados, ArvBin *root);

ArvBin *new_ArvBin()
{
    ArvBin *root = (ArvBin *)malloc(sizeof(ArvBin));
    if (root != NULL)
    {
        *root = NULL;
    }
    return root;
}

void free_node(ArvBin node)
{
    if (node == NULL)
    {
        return;
    }
    free_node(node->left);
    free_node(node->right);
    free(node);
    node = NULL;
}

void free_ArvBin(ArvBin *root)
{
    if (root == NULL)
    {
        return;
    }
    free_node(*root);
    free(root);
}

int height(ArvBin node)
{
    if (node == NULL)
    {
        return -1;
    }
    else
    {
        return node->height;
    }
}

int get_fb(ArvBin node)
{
    return labs(height(node->left) - height(node->right));
}

void LLRotation(ArvBin *root)
{
    ArvBin node;
    node = (*root)->left;
    (*root)->left = node->right;
    node->right = *root;
    (*root)->height = fmax(height((*root)->left), height((*root)->right)) + 1;
    node->height = fmax(height(node->left), (*root)->height) + 1;
    *root = node;
}

void RRRotation(ArvBin *root)
{
    ArvBin node;
    node = (*root)->right;
    (*root)->right = node->left;
    node->left = *root;
    (*root)->height = fmax(height((*root)->left), height((*root)->right)) + 1;
    node->height = fmax(height(node->right), (*root)->height) + 1;
    *root = node;
}

void LRRotation(ArvBin *root)
{
    RRRotation(&(*root)->left);
    LLRotation(root);
}

void RLRotation(ArvBin *root)
{
    LLRotation(&(*root)->right);
    RRRotation(root);
}

int insert_ArvBin(ArvBin *root, Escola data)
{
    int res;
    if (*root == NULL)
    {
        ArvBin node = (ArvBin)malloc(sizeof(struct node));
        if (node == NULL)
        {
            return 0;
        }
        node->data = data;
        node->height = 0;
        node->left = NULL;
        node->right = NULL;
        *root = node;
        return 1;
    }
    ArvBin node = *root;
    if (data.id < node->data.id)
    {
        if ((res = insert_ArvBin(&(node->left), data)) == 1)
        {
            if (get_fb(node) >= 2)
            {
                if (data.id < (*root)->left->data.id)
                {
                    LLRotation(root);
                }
                else
                {
                    LRRotation(root);
                }
            }
        }
    }
    else
    {
        if (data.id > node->data.id)
        {
            if ((res = insert_ArvBin(&(node->right), data)) == 1)
            {
                if (get_fb(node) >= 2)
                {
                    if (data.id > (*root)->right->data.id)
                    {
                        RRRotation(root);
                    }
                    else
                    {
                        RLRotation(root);
                    }
                }
            }
        }
        else
        {
            return 0;
        }
    }
    node->height = fmax(height(node->left), height(node->right)) + 1;
    return res;
}

ArvBin minNode(ArvBin node)
{
    ArvBin temp = node;
    ArvBin temp2 = node->left;
    while (temp2 != NULL)
    {
        temp = temp2;
        temp2 = temp2->left;
    }
    return temp;
}

int remove_ArvBin(ArvBin *root, int id)
{
    if (*root == NULL)
    {
        return 0;
    }
    int res;
    if (id < (*root)->data.id)
    {
        if ((res = remove_ArvBin(&(*root)->left, id)) == 1)
        {
            if (get_fb(*root) >= 2)
            {
                if (height((*root)->right->left) <= height((*root)->right->right))
                {
                    RRRotation(root);
                }
                else
                {
                    RLRotation(root);
                }
            }
        }
    }
    if (id > (*root)->data.id)
    {
        if ((res = remove_ArvBin(&(*root)->right, id)) == 1)
        {
            if (get_fb(*root) >= 2)
            {
                if (height((*root)->left->right) <= height((*root)->left->left))
                {
                    LLRotation(root);
                }
                else
                {
                    LRRotation(root);
                }
            }
        }
    }
    if (id == (*root)->data.id)
    {
        if (((*root)->left == NULL || (*root)->right == NULL))
        {
            ArvBin oldNode = (*root);
            if ((*root)->left != NULL)
            {
                *root = (*root)->left;
            }
            else
            {
                *root = (*root)->right;
            }
            free(oldNode);
        }
        else
        {
            ArvBin temp = minNode((*root)->right);
            (*root)->data = temp->data;
            remove_ArvBin(&(*root)->left, (*root)->data.id);
            if (get_fb(*root) >= 2)
            {
                if (height((*root)->left->right) <= height((*root)->left->left))
                {
                    LLRotation(root);
                }
                else
                {
                    LRRotation(root);
                }
            }
        }
        if (*root != NULL)
        {
            (*root)->height = fmax(height((*root)->left), height((*root)->right)) + 1;
        }
        return 1;
    }
    return res;
}

Escola *search_ArvBin(ArvBin *root, int id)
{
    if (root == NULL)
    {
        return 0;
    }
    ArvBin node = *root;
    while (node != NULL)
    {
        if (node->data.id == id)
        {
            return &(node->data);
        }
        if (id > node->data.id)
        {
            node = node->right;
        }
        else
        {
            node = node->left;
        }
    }
    return NULL;
}

void loadTree(FILE *dados, ArvBin *root)
{

    Escola data;
    int i = 0;
    char linha[250];

    while (fgets(linha, 250, dados) != NULL)
    {

        char *token = strtok(linha, ";"); // id
        data.id = atoi(token);

        token = strtok(NULL, ";"); // estado
        strncpy(data.estado, token, sizeof(data.estado));

        token = strtok(NULL, ";"); // municipio
        strncpy(data.municipio, token, sizeof(data.municipio));

        token = strtok(NULL, ";"); // rede
        strncpy(data.rede, token, sizeof(data.rede));

        token = strtok(NULL, ";"); // media_ciencias_natureza
        data.media_ciencias_natureza = atof(token);

        token = strtok(NULL, ";"); // media_ciencias_humanas
        data.media_ciencias_humanas = atof(token);

        token = strtok(NULL, ";"); // media_linguagem
        data.media_linguagem = atof(token);

        token = strtok(NULL, ";"); // media_matematica
        data.media_matematica = atof(token);

        token = strtok(NULL, ";"); // media_redacao
        data.media_redacao = atof(token);

        if (insert_ArvBin(root, data) == 1)
        {
            i++;
        }
    }

    printf("Foram inseridos %d registros na arvore\n", i);
}

void printEscola(Escola *data)
{
    if (data == NULL)
    {
        printf("A Escola não existe.\n");
        return;
    }
    printf("ID: %d\n", data->id);
    printf("Estado: %s\n", data->estado);
    printf("Municipio: %s\n", data->municipio);
    printf("Rede: %s\n", data->rede);
    printf("Media Ciencias da Natureza: %.2f\n", data->media_ciencias_natureza);
    printf("Media Ciencias Humanas: %.2f\n", data->media_ciencias_humanas);
    printf("Media Linguagem: %.2f\n", data->media_linguagem);
    printf("Media Matematica: %.2f\n", data->media_matematica);
    printf("Media Redacao: %.2f\n", data->media_redacao);
}

int main()
{

    ArvBin *root;

    FILE *dados = fopen("DadosEnem.txt", "r");
    if (dados == NULL)
    {
        printf("Erro ao abrir o arquivo de dados\n");
        return 1;
    }

    root = new_ArvBin();

    loadTree(dados, root);

    /* TESTES DAS FUNCOES IMPLEMENTADAS */

    Escola *output;

    output = search_ArvBin(root, 12592);
    printEscola(output);

    if (remove_ArvBin(root, 12592))
    {
        printf("--Escola removida com sucesso--\n");
    }

    output = search_ArvBin(root, 12592);
    printEscola(output);

    /* FIM DOS TESTES DAS FUNCOES IMPLEMENTADAS */

    fclose(dados);
    return 0;
}