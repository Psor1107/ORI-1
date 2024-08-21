#include <stdio.h>
#include <stdlib.h>

#define MAX 3
#define MIN 2

struct BTreeNode {
    int keys[MAX + 1], count;
    struct BTreeNode* children[MAX + 1];
};

int main() {
    struct BTreeNode *root = NULL;
    return 0;
}