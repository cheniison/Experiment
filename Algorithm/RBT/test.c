
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
    char ch;
    int num;

    tree.root = NULL;

    while (scanf("%c%d", &ch, &num) != EOF) {
        if (ch == 'i') {
            RBT_insert(&tree, num);
        } else if (ch == 'd') {
            RBT_delete(&tree, num);
        } else {
            printf("未知操作: ch = %c num = %d\n", ch, num);
            getchar();
            continue;
        }
        print(&tree);
        printf("\n");
        getchar();
    }

    RBT_clear(&tree);

    return 0;
}
