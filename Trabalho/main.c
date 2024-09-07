#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX 3 // Grau (Ordem - 1)

struct BTreeNode
{
    int keys[MAX];
    struct BTreeNode *children[MAX + 1];
    int n; // Número de chaves
    bool leaf;
} typedef *BTree;

BTree new_BTree();
struct BTreeNode *new_node_BTree(bool leaf);
void insert_BTree(struct BTreeNode **root, int key);
struct BTreeNode *search_BTree(struct BTreeNode *root, int key);
void split_child_BTree(struct BTreeNode *parent, int i, struct BTreeNode *child);
void insert_non_full_BTree(struct BTreeNode *node, int key);
void print_BTree(struct BTreeNode *root);
void free_BTree(struct BTreeNode *root);
int get_Predecessor(struct BTreeNode *node, int idx);
int get_Sucessor(struct BTreeNode *node, int idx);
void merge(struct BTreeNode *node, int idx);
void fill(struct BTreeNode *node, int idx);
void borrow_prev(struct BTreeNode *node, int idx);
void borrow_next(struct BTreeNode *node, int idx);
void remove_non_leaf(struct BTreeNode *node, int idx);
void remove_leaf(struct BTreeNode *node, int idx);
void remove_key(struct BTreeNode *node, int key);

// nova arvore
BTree new_BTree()
{
    return NULL;
}

// Função para criar um novo nó da árvore 
struct BTreeNode *new_node_BTree(bool leaf)
{
    // Aloca memória para um novo nó da árvore 
    struct BTreeNode *node = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    
    // Define se o nó é uma folha ou não
    node->leaf = leaf;
    
    // Inicializa o número de chaves no nó como 0
    node->n = 0;
    
    // Inicializa todos os ponteiros para os filhos do nó como NULL
    for (int i = 0; i < MAX + 1; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

// busca
struct BTreeNode *search_BTree(struct BTreeNode *root, int key)
{
    int i = 0;
    while (i < root->n && key > root->keys[i]) // encontrar a posição da chave
    {
        i++;
    }
    if (i < root->n && key == root->keys[i]) // se a chave for encontrada
    {
        return root;
    }
    if (root->leaf) // se for folha
    {
        return NULL;
    }
    return search_BTree(root->children[i], key); // recursão
}

// inserção
void insert_BTree(struct BTreeNode **root, int key)
{
    if (*root == NULL)  // se a árvore estiver vazia
    {
        *root = new_node_BTree(true); // Cria um novo nó folha e o define como raiz
        (*root)->keys[0] = key;// Insere a primeira chave no nó
        (*root)->n = 1;// Atualiza o número de chaves no nó
    }
    else
    {
        if (search_BTree(*root, key)) // se a chave já existir
        {
            return;
        }
        if ((*root)->n == MAX) // se o nó estiver cheio
        {
            struct BTreeNode *newRoot = new_node_BTree(false); 
            newRoot->children[0] = *root;
            split_child_BTree(newRoot, 0, *root); // dividir o nó
            insert_non_full_BTree(newRoot, key); // inserir a chave
            *root = newRoot; // atualizar a raiz
        }
        else
        {
            insert_non_full_BTree(*root, key); // inserir a chave
        }
    }
}

// inserção em nó não cheio
void insert_non_full_BTree(struct BTreeNode *node, int key)
{
    // encontrar a posição da chave
    int i = node->n - 1;
    // se for folha
    if (node->leaf)
    {
        while (i >= 0 && key < node->keys[i]) // encontrar a posição correta
        {
            node->keys[i + 1] = node->keys[i]; // mover as chaves maiores para frente
            i--;                              // decrementar
        }
        node->keys[i + 1] = key; // inserir a chave
        node->n++;              // incrementar o número de chaves
    }
    // se não for folha
    else
    {
        while (i >= 0 && key < node->keys[i]) // encontrar a posição correta
        {
            i--; // decrementar
        }
        i++; 
        if (node->children[i]->n == MAX)  // se o filho estiver cheio
        {
            split_child_BTree(node, i, node->children[i]); // dividir o filho
            if (key > node->keys[i]) // se a chave for maior que a chave do meio
            {
                i++; // incrementar
            }
        }
        insert_non_full_BTree(node->children[i], key); // inserir a chave no filho
    }
}

// divisão de nó
void split_child_BTree(struct BTreeNode *parent, int i, struct BTreeNode *child)
{
    // novo nó
    struct BTreeNode *newChild = new_node_BTree(child->leaf);

    // metade das chaves para o novo nó
    newChild->n = (MAX - 1) / 2;

    // transferir metade das chaves para o novo nó
    for (int j = 0; j < (MAX - 1) / 2; j++)
    {
        newChild->keys[j] = child->keys[j + (MAX + 1) / 2];
    }

    // transferir metade dos filhos para o novo nó caso não seja folha
    if (!child->leaf)
    {
        for (int j = 0; j <= (MAX - 1) / 2; j++)
        {
            newChild->children[j] = child->children[j + (MAX + 1) / 2];
        }
    }

    // atualizar o número de chaves do nó (no - novo nó)
    child->n = (MAX - 1) / 2;

    // abrir espaço para o novo filho
    for (int j = parent->n; j >= i + 1; j--)
    {
        parent->children[j + 1] = parent->children[j];
    }

    // ligar o novo filho ao pai
    parent->children[i + 1] = newChild;

    // abrir espaço para a nova chave
    for (int j = parent->n - 1; j >= i; j--)
    {
        parent->keys[j + 1] = parent->keys[j];
    }

    // mover a chave do meio para o pai
    parent->keys[i] = child->keys[(MAX - 1) / 2];
    // atualizar o número de chaves do pai
    parent->n++;
}

// impressão
void print_BTree(struct BTreeNode *root)
{
    int i;
    
    // Verifica se o nó atual não é nulo (caso contrário, a função termina)
    if (root != NULL)
    {
        for (i = 0; i < root->n; i++)// Percorre todas as chaves do nó atual
        {
            print_BTree(root->children[i]);// Primeiro, imprime recursivamente a subárvore à esquerda da chave
            printf("[%d] ", root->keys[i]);// imprime a chave atual
        }
        print_BTree(root->children[i]);//imprime a subárvore à direita da última chave
    }
}

// liberação de memória
void free_BTree(struct BTreeNode *root)
{
    if (root != NULL)
    {
        for (int i = 0; i <= root->n; i++)// Percorre todos os filhos do nó atual
        {
            if (root->children[i] != NULL)// Verifica se o ponteiro para o filho não é nulo
            {
                free_BTree(root->children[i]);// Libera recursivamente a subárvore correspondente ao filho
            }
        }
        free(root);
    }
}

// busca do predecessor
int get_Predecessor(struct BTreeNode *node, int idx)
{
    struct BTreeNode *cur = node->children[idx];// Inicializa um ponteiro para o filho à esquerda da chave no índice idx
    
    while (!cur->leaf)// Percorre até encontrar uma folha
        cur = cur->children[cur->n];// Move para o último filho do nó atual
    return cur->keys[cur->n - 1];// Retorna a última chave do nó folha
}

// busca do sucessor
int get_Sucessor(struct BTreeNode *node, int idx)
{
    struct BTreeNode *cur = node->children[idx + 1];
    while (!cur->leaf)
        cur = cur->children[0];
    return cur->keys[0];
}

// junção de nós
void merge(struct BTreeNode *node, int idx)
{
    // Inicializa ponteiros para o filho e seu irmão à direita
    struct BTreeNode *child = node->children[idx];
    struct BTreeNode *sibling = node->children[idx + 1];

    child->keys[(MAX - 1) / 2] = node->keys[idx];// Move a chave do nó pai para o meio do nó filho

    // Copia todas as chaves do irmão para o nó filho
    for (int i = 0; i < sibling->n; i++)
        child->keys[i + (MAX + 1) / 2] = sibling->keys[i];

    // Se o nó filho não for folha, também copia todos os filhos do irmão para o filho
    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->n; i++)
            child->children[i + (MAX + 1) / 2] = sibling->children[i];
    }

    // Ajusta as chaves do nó pai para remover a chave movida para o nó filho
    for (int i = idx + 1; i < node->n; i++)
        node->keys[i - 1] = node->keys[i];

    // Ajusta os ponteiros dos filhos do nó pai para remover o ponteiro do irmão
    for (int i = idx + 2; i <= node->n; i++)
        node->children[i - 1] = node->children[i];

    child->n += sibling->n + 1;// Atualiza o número de chaves do nó filho
    node->n--;// Reduz o número de chaves no nó pai

    free(sibling);
}

// preenchimento
void fill(struct BTreeNode *node, int idx)
{
    // Verifica se o filho anterior tem chaves suficientes para fazer o empréstimo
    if (idx != 0 && node->children[idx - 1]->n >= (MAX + 1) / 2)
        borrow_prev(node, idx);// Realiza o empréstimo do filho anterior
    else if (idx != node->n && node->children[idx + 1]->n >= (MAX + 1) / 2)
        borrow_next(node, idx);// Realiza o empréstimo do filho seguinte
    else
    {
        if (idx != node->n)
            merge(node, idx);//junta com o filho seguinte existir
        else
            merge(node, idx - 1);//junta com o filho anterior
    }
}

// empréstimo do nó anterior
void borrow_prev(struct BTreeNode *node, int idx)
{
    // Ponteiros para o nó filho e seu irmão anterior
    struct BTreeNode *child = node->children[idx];
    struct BTreeNode *sibling = node->children[idx - 1];
    
    // Move todas as chaves do nó filho uma posição à frente para abrir espaço
    for (int i = child->n - 1; i >= 0; i--)
        child->keys[i + 1] = child->keys[i];

    // Se o nó filho não for folha
    if (!child->leaf)
    {
        for (int i = child->n; i >= 0; i--)
            child->children[i + 1] = child->children[i];
    }

    child->keys[0] = node->keys[idx - 1];// A chave do nó pai em 'idx - 1' é movida para o início das chaves do nó filho

    // Se o nó filho não for uma folha
    if (!child->leaf)
        child->children[0] = sibling->children[sibling->n];//move o último filho do irmão anterior para o primeiro filho do nó filho

    node->keys[idx - 1] = sibling->keys[sibling->n - 1];// A última chave do irmão anterior é movida para o nó pai

    // Atualiza o número de chaves no nó filho e no irmão anterior
    child->n += 1;
    sibling->n -= 1;
}

// empréstimo do próximo nó
void borrow_next(struct BTreeNode *node, int idx)
{
    // Ponteiros para o nó filho e seu irmão próximo (à direita)
    struct BTreeNode *child = node->children[idx];
    struct BTreeNode *sibling = node->children[idx + 1];

    child->keys[(child->n)] = node->keys[idx];// A chave do nó pai em 'idx' é movida para o final das chaves do nó filho

    // Se o nó filho não for uma folha
    if (!(child->leaf))
        child->children[(child->n) + 1] = sibling->children[0];//o primeiro filho do irmão próximo é movido para o final dos filhos do nó filho

    node->keys[idx] = sibling->keys[0];// A primeira chave do irmão próximo é movida para o nó pai

    // As chaves do irmão próximo são deslocadas uma posição à esquerda
    for (int i = 1; i < sibling->n; i++)
        sibling->keys[i - 1] = sibling->keys[i];

    // Se o irmão próximo não for uma folha
    if (!sibling->leaf)
    {
        for (int i = 1; i <= sibling->n; i++)//os ponteiros dos filhos são deslocados uma posição à esquerda
            sibling->children[i - 1] = sibling->children[i];
    }

    child->n += 1;
    sibling->n -= 1;
}

// remoção de nó não folha
void remove_non_leaf(struct BTreeNode *node, int idx)
{
    int key = node->keys[idx];// A chave que precisa ser removida é a chave no índice idx do nó

    // Verifica se o filho à esquerda da chave tem chaves suficientes
    if (node->children[idx]->n >= (MAX + 1) / 2)
    {
        int pred = get_Predecessor(node, idx);// Encontra o predecessor da chave no nó filho
        node->keys[idx] = pred;// Substitui a chave no nó pelo predecessor encontrado
        remove_key(node->children[idx], pred);// Remove a chave predecessor do nó filho
    }
    else if (node->children[idx + 1]->n >= (MAX + 1) / 2)
    {
        int succ = get_Sucessor(node, idx);// Encontra o sucessor da chave no nó filho
        node->keys[idx] = succ;// Substitui a chave no nó pelo sucessor encontrado
        remove_key(node->children[idx + 1], succ);// Remove a chave sucessor do nó filho
    }
    else
    {
        merge(node, idx);// Se ambos os filhos têm menos chaves do que o mínimo necessário, realiza a junção
        remove_key(node->children[idx], key);// Remove a chave original do nó filho após a junção
    }
}

// remoção de nó folha
void remove_leaf(struct BTreeNode *node, int idx)
{
    // Desloca todas as chaves à direita da posição 'idx' uma posição à esquerda
    for (int i = idx + 1; i < node->n; i++)
        node->keys[i - 1] = node->keys[i];
        
    node->n--;// Decrementa o número total de chaves no nó
}

// remoção de chave
void remove_key(struct BTreeNode *node, int key)
{
    int idx = 0;
    
    // Encontra o índice da chave no nó onde a chave pode estar
    while (idx < node->n && node->keys[idx] < key)
        idx++;

    // Se a chave foi encontrada no nó
    if (idx < node->n && node->keys[idx] == key)
    {
        if (node->leaf)// Se o nó é folha, remove a chave do nó folha
            remove_leaf(node, idx);
        else// Caso contrario, remove a chave do nó não folha
            remove_non_leaf(node, idx);
    }
    else
    {
        if (node->leaf)// Se o nó é folha e a chave não foi encontrada
        {
            printf("A chave %d não está na árvore.\n", key);
            return;
        }

        bool flag = ((idx == node->n) ? true : false);// Verifica se a chave deve ser removida do nó filho à esquerda ou à direita

        // Garante que o nó filho em idx tenha o número mínimo de chaves
        if (node->children[idx]->n < (MAX + 1) / 2)
            fill(node, idx);

        // Se a flag indica que a chave deveria estar no nó filho à direita, usa o filho à esquerda
        if (flag && idx > node->n)
            remove_key(node->children[idx - 1], key);
        else
            remove_key(node->children[idx], key);
    }
}

// carregar arvore
void loadTree(FILE *dados, struct BTreeNode **root)
{

    int data;
    int i = 0;
    char linha[250];

    // Lê o arquivo linha por linha
    while (fgets(linha, 250, dados) != NULL)
    {
        
        char *token = strtok(linha, ";"); // id
        data = atoi(token);// Converte o id para um inteiro

        token = strtok(NULL, ";"); // estado
        token = strtok(NULL, ";"); // municipio
        token = strtok(NULL, ";"); // rede
        token = strtok(NULL, ";"); // media_ciencias_natureza
        token = strtok(NULL, ";"); // media_ciencias_humanas
        token = strtok(NULL, ";"); // media_linguagem
        token = strtok(NULL, ";"); // media_matematica
        token = strtok(NULL, ";"); // media_redacao

        insert_BTree(root, data);// Insere a chave (id) na árvore B
        i++;
    }
    printf("Foram inseridos %d registros na arvore\n", i);
}

int main()
{
    BTree root = new_BTree();

    // inserção simples
    int buffer1[] = {15, 10, 20, 5, 12, 25, 30, 8, 18, 28};
    for (int i = 0; i < sizeof(buffer1) / sizeof(buffer1[0]); i++)
    {
        insert_BTree(&root, buffer1[i]);
    }
    printf("Árvore B inserção simples:\n");
    print_BTree(root);
    printf("\n\n");

    // overflow no pai
    root = new_BTree();
    int buffer2[] = {3, 7, 9, 10, 11, 13, 15, 16, 17, 18};
    for (int i = 0; i < sizeof(buffer2) / sizeof(buffer2[0]); i++)
    {
        insert_BTree(&root, buffer2[i]);
    }
    printf("Árvore B overflow no pai:\n");
    print_BTree(root);
    printf("\n\n");

    // inserção em ordem decrescente
    root = new_BTree();
    int buffer3[] = {30, 25, 20, 15, 10, 5};
    for (int i = 0; i < sizeof(buffer3) / sizeof(buffer3[0]); i++)
    {
        insert_BTree(&root, buffer3[i]);
    }
    printf("Árvore B inserção em ordem decrescente:\n");
    print_BTree(root);
    printf("\n\n");

    // chaves duplicadas
    root = new_BTree();
    int buffer4[] = {10, 20, 10, 30, 20, 40};
    for (int i = 0; i < sizeof(buffer4) / sizeof(buffer4[0]); i++)
    {
        insert_BTree(&root, buffer4[i]);
    }
    printf("Árvore B chaves duplicadas:\n");
    print_BTree(root);
    printf("\n\n");

    // remoção de chaves
    int buffer5[] = {10, 20, 5, 6, 12, 30, 7, 17};
    root = new_BTree();
    for (int i = 0; i < sizeof(buffer5) / sizeof(buffer5[0]); i++)
    {
        insert_BTree(&root, buffer5[i]);
    }
    printf("Árvore B:\n");
    print_BTree(root);
    printf("\n");

    remove_key(root, 6);
    remove_key(root, 20);
    printf("\nÁrvore B após remoção de chaves 6 e 20:\n");
    print_BTree(root);
    printf("\n\n");

    // memoria secundaria

    root = new_BTree();
    FILE *dados = fopen("DadosEnemMin.txt", "r");
    if (dados == NULL)
    {
        printf("Erro ao abrir o arquivo de dados\n");
        return 1;
    }

    loadTree(dados, &root);
    print_BTree(root);
    printf("\n\n");

    free_BTree(root);

    return 0;
}