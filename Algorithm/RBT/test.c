
#include "RBT.h"
#include <stdlib.h>
#include <stdio.h>


void _print(const RBT_Node * root)
{

    if (root == NULL) {
        return;
    }
    
    printf("%d\t%s\n", root->data, root->color == RED ? "RED" : "BLACK");
    _print(root->left);
    _print(root->right);
}

void print(const RBT * tree)
{
    _print(tree->root);
}

int main(int argc, char **argv)
{
    RBT tree;
    int i;

    tree.root = NULL;

    for (i = 1; i < argc; ++i) {
        RBT_insert(&tree, atoi(argv[i]));
    }

    print(&tree);

    RBT_clear(&tree);

    return 0;
}
