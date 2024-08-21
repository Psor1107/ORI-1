#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ORDER 3

struct BTreeNode {
    int keys[MAX_ORDER-1]; 
    int num_keys;
    bool is_leaf;
    struct BTreeNode *children[MAX_ORDER];
}typedef *BTree;

BTree new_BTree()
{
    return NULL;
}

struct BTreeNode *new_node_BTree(bool is_leaf)
{
    struct BTreeNode *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    newNode->num_keys = 0;
    newNode->is_leaf = is_leaf;
    for (int i = 0; i < MAX_ORDER; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

void split_child_BTree(struct BTreeNode *parent, int i) {
    
    struct BTreeNode *child = parent->children[i];
    struct BTreeNode *new_node = new_node_BTree(child->is_leaf);
    
    int median = MAX_ORDER / 2;

    // -move metade das chaves pro novo nó
    new_node->num_keys = median;
    for (int j = 0; j < median; j++) {
        new_node->keys[j] = child->keys[j + median];
    }

    // -se não for folha -> move a mesma metade dos ponteiros dos filhos pro novo nó
    if (!child->is_leaf) {
        for (int j = 0; j <= median; j++) {
            new_node->children[j] = child->children[j + median];
        }
    }

    child->num_keys = median - 1;

    // -move os ponteiros dos filhos antigos do pai pra direita pro novo filho
    for (int j = parent->num_keys; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    // -linka o novo filho
    parent->children[i + 1] = new_node;

    // -como o novo filho é o do meio move a chave do meio pro pai
    //
    // -fazendo isso aqui da pra garantir que vai manter ordenado
    // exemplo:   
    //            5
    //           / \
    //          10  20
    //          
    //             5
    //           / | \
    //          10 12 20
    //
    // -quando a gente cortar no meio...
    //             
    //            5 12
    //           /    \
    //          10    20
    // 
    // ...lembrando que 5 e 12 são do mesmo "bloco"...
    for (int j = parent->num_keys - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    // -move a chave do meio pro pai
    // -ja que ela ja foi "deletada"...
    parent->keys[i] = child->keys[median - 1];
    parent->num_keys++;
}



void insert_non_full_BTree(struct BTreeNode **root, int key)
{
    int i = (*root)->num_keys - 1;
    
    if ((*root)->is_leaf) {
        while (i >= 0 && (*root)->keys[i] > key) {
            (*root)->keys[i + 1] = (*root)->keys[i];
            i--;
        }
        (*root)->keys[i + 1] = key;
        (*root)->num_keys++;
    } else {
        while (i >= 0 && (*root)->keys[i] > key) {
            i--;
        }
        i++;
        
        if ((*root)->children[i]->num_keys == MAX_ORDER - 1) {
            split_child_BTree(*root, i);
            if ((*root)->keys[i] < key) {
                i++;
            }
        }
        insert_non_full_BTree(&(*root)->children[i], key);
    }
}

void insert_BTree(struct BTreeNode **root, int key)
{
    if (*root == NULL)
    {
        *root = new_node_BTree(true);
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
    } 
    else 
    {
        if ((*root)->num_keys == MAX_ORDER - 1)
        {
            struct BTreeNode *new_root = new_node_BTree(false);
            new_root->children[0] = *root;
            split_child_BTree(new_root, 0);
            *root = new_root;
        }   
        insert_non_full_BTree(root, key);
    }
}

void free_BTree(struct BTreeNode *root)
{
    int i = 0;
    if (root)
    {
        for (i = 0; i < root->num_keys; i++)
        {
            free_BTree(root->children[i]);
        }
        free_BTree(root->children[i]);
        free(root);
    }
}

void print_BTree(struct BTreeNode *root)
{
    int i;
    if (root != NULL)
    {
        for (i = 0; i < root->num_keys; i++)
        {
            print_BTree(root->children[i]);
            printf("[%d] ", root->keys[i]);
        }
        print_BTree(root->children[i]);
    }
}

int main() {
    
    BTree root = new_BTree();
    insert_BTree(&root, 15);
    insert_BTree(&root, 10);
    insert_BTree(&root, 20);
    insert_BTree(&root, 5);
    insert_BTree(&root, 12);
    insert_BTree(&root, 25);
    insert_BTree(&root, 30);
    insert_BTree(&root, 8);
    insert_BTree(&root, 18);
    insert_BTree(&root, 28);

    print_BTree(root);
    
    free_BTree(root);
    printf("\n");

    root = new_BTree();
    insert_BTree(&root, 50);
    insert_BTree(&root, 30);
    insert_BTree(&root, 70);
    insert_BTree(&root, 20);
    insert_BTree(&root, 40);
    insert_BTree(&root, 60);
    insert_BTree(&root, 80);
    insert_BTree(&root, 10);
    insert_BTree(&root, 25);
    insert_BTree(&root, 35);
    insert_BTree(&root, 45);
    insert_BTree(&root, 55);
    insert_BTree(&root, 65);
    insert_BTree(&root, 75);

    print_BTree(root);
    
    return 0;
}