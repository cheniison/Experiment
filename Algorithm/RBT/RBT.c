
#include "RBT.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

/* 初始化节点 */
RBT_Node * node_init(type_t obj)
{
    
    RBT_Node * tmp;

    tmp = (RBT_Node *)malloc(sizeof(RBT_Node));
    tmp->data = obj;
    tmp->left = NULL;
    tmp->right = NULL;
    tmp->color = RED;

    return tmp;
}


/* 判断节点是否是红色的 */
static int RBT_is_red(RBT_Node * node)
{
    if (node == NULL || node->color == BLACK) {
        return 0;
    } else {
        return 1;
    }
}


/* 左旋 */
static RBT_Node * RBT_rotate_left(RBT_Node * root)
{
    RBT_Node * right_node = root->right;

    root->right = right_node->left;
    right_node->left = root;

    /* 改变颜色 */
    right_node->color = root->color;
    root->color = RED;

    return right_node;
}


/* 右旋 */
static RBT_Node * RBT_rotate_right(RBT_Node * root)
{
    RBT_Node * left_node = root->left;

    root->left = left_node->right;
    left_node->right = root;

    /* 改变颜色 */
    left_node->color = root->color;
    root->color = RED;

    return left_node;
}


/* 搜索 */
static RBT_Node * _RBT_search(RBT_Node * root, type_t obj)
{
    if (root == NULL) {
        /* 没搜到 */
        return NULL;
    }

    if (root->data == obj) {
        /* 搜到 */
        return root;
    } else if (root->data > obj) {
        /* 搜索左子树 */
        return _RBT_search(root->left, obj);   
    } else {
        /* 搜索右子树 */
        return _RBT_search(root->right, obj);
    }
}


RBT_Node * RBT_search(const RBT * tree, type_t obj)
{
    return _RBT_search(tree->root, obj);
}


/* 插入 */
static RBT_Node * _RBT_insert(RBT_Node * root, type_t obj)
{
    if (root == NULL) {
        return node_init(obj);
    }

    if (root->data == obj) {
        return root;
    } else if (root->data < obj) {
        root->right = _RBT_insert(root->right, obj);
    } else {
        root->left = _RBT_insert(root->left, obj);
    }

    /* 左连接红，左连接的左连接红 */
    if (RBT_is_red(root->left) && RBT_is_red(root->left->left)) {
        root = RBT_rotate_right(root);
    }

    /* 左右连接都为红 */
    if (RBT_is_red(root->left) && RBT_is_red(root->right)) {
        root->left->color = BLACK;
        root->right->color = BLACK;
        root->color = RED;
    }

    /* 右连接红，左连接黑 */
    if (RBT_is_red(root->right) && ! RBT_is_red(root->left)) {
        root = RBT_rotate_left(root);
    }

    /* 其他情况 */
    return root;
}

void RBT_insert(RBT * tree, type_t obj)
{
    tree->root = _RBT_insert(tree->root, obj);
    tree->root->color = BLACK;
}



/* 删除 */

static RBT_Node * _RBT_delete(RBT_Node * root, type_t obj)
{
    return NULL;
}

void RBT_delete(RBT * tree, type_t obj)
{
    if (tree->root == NULL) {
        
        /* 树中没有节点 */
        return ;
    
    } else {

        /* 从根节点开始删除 */
        _RBT_delete(tree->root, obj);

    }
}

/* 清除整棵树 */

static void _RBT_clear(RBT_Node * node)
{
    if (node == NULL)
        return;

    _RBT_clear(node->left);
    _RBT_clear(node->right);
    
    free(node);
}

void RBT_clear(RBT * tree)
{
    _RBT_clear(tree->root);
}


