
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



/* 翻转颜色 */

static enum Color _RBT_flip_color(enum Color c)
{
    return c == BLACK ? RED : BLACK;
}

static void RBT_flip_colors(RBT_Node * root)
{
    root->color = _RBT_flip_color(root->color);
    root->left->color = _RBT_flip_color(root->left->color);
    root->right->color = _RBT_flip_color(root->right->color);
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


/* 修正节点 */
static RBT_Node * RBT_fix_up(RBT_Node * root)
{
    /* 左连接红，左连接的左连接红，右旋 */
    if (RBT_is_red(root->left) && RBT_is_red(root->left->left)) {
        root = RBT_rotate_right(root);
    }

    /* 左右连接都为红，翻转颜色 */
    if (RBT_is_red(root->left) && RBT_is_red(root->right)) {
        RBT_flip_colors(root);
    }

    /* 右连接红，左连接黑，左旋 */
    if (RBT_is_red(root->right) && ! RBT_is_red(root->left)) {
        root = RBT_rotate_left(root);
    }

    /* 其他情况，直接返回 */
    return root;
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



/*
 * 获得最小节点 
 */

static RBT_Node * _RBT_get_min(RBT_Node * root)
{
    if (root->left == NULL) {
        return root;
    }

    return _RBT_get_min(root->left);
}

RBT_Node * RBT_get_min(const RBT * tree)
{
    if (tree->root == NULL) {
        return NULL;
    }

    return _RBT_get_min(tree->root);
}


/*
 * 插入 
 */

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

    return RBT_fix_up(root);

}

void RBT_insert(RBT * tree, type_t obj)
{
    tree->root = _RBT_insert(tree->root, obj);
    tree->root->color = BLACK;
}


/* 
 * 删除 
 */

static RBT_Node * RBT_move_red_left(RBT_Node * root)
{
    RBT_flip_colors(root);
    if (RBT_is_red(root->right->left)) {
        root->right = RBT_rotate_right(root->right);
        root = RBT_rotate_left(root);
        RBT_flip_colors(root);
    }   
    return root;
}

static RBT_Node * RBT_move_red_right(RBT_Node * root)
{
    RBT_flip_colors(root);
    if (RBT_is_red(root->left->left)) {
        root = RBT_rotate_right(root);
        RBT_flip_colors(root);
    }
    return root;
}

/* 删除最小节点 */

static RBT_Node * _RBT_delete_min(RBT_Node * root)
{
    if (root->left == NULL) {
        /* 是最小节点 */
        free(root);
        return NULL;
    }

    if (!RBT_is_red(root->left) && !RBT_is_red(root->left->left)) {
        root = RBT_move_red_left(root);
    }

    root->left = _RBT_delete_min(root->left);
    
    return RBT_fix_up(root);
}

void RBT_delete_min(RBT * tree)
{
    if (tree->root == NULL) {
        return;
    }

    tree->root = _RBT_delete_min(tree->root);

    if (tree->root != NULL) {
        tree->root->color = BLACK;
    }
}


/* 删除任意节点 */

static RBT_Node * _RBT_delete(RBT_Node * root, type_t obj)
{

    if (obj < root->data) {
        /* 比当前节点小 */
        if (! RBT_is_red(root->left) && ! RBT_is_red(root->left->left)) {
            root = RBT_move_red_left(root);
        }
        root->left = _RBT_delete(root->left, obj);
    } else {
        /* 大于等于当前节点 */
        if (RBT_is_red(root->left)) {
            root = RBT_rotate_right(root);
        }
        if (obj == root->data && root->right == NULL) {
            free(root);
            return NULL;
        }
        if (! RBT_is_red(root->right) && ! RBT_is_red(root->right->left)) {
            root = RBT_move_red_right(root);
        }

        if (obj == root->data) {
            root->data = _RBT_get_min(root->right)->data;
            root->right = _RBT_delete_min(root->right);
        } else {
            root->right = _RBT_delete(root->right, obj);
        }
    }

    return RBT_fix_up(root);
}

void RBT_delete(RBT * tree, type_t obj)
{
    if (RBT_search(tree, obj) == NULL) {
        /* 树中没有这个节点 */
        return ;
    }

    /* 从根节点开始删除 */
    tree->root = _RBT_delete(tree->root, obj);

    if (tree->root != NULL) {
        tree->root->color = BLACK;
    }

}


/* 
 * 清除整棵树 
 */

static void _RBT_clear(RBT_Node * node)
{
    if (node == NULL) {
        return;
    }

    _RBT_clear(node->left);
    _RBT_clear(node->right);
    
    free(node);
}

void RBT_clear(RBT * tree)
{
    _RBT_clear(tree->root);
}


