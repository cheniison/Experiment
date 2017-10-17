#ifndef _RBT_H
#define _RBT_H

#define type_t int

enum Color {RED, BLACK};

/* 红黑树节点 */
typedef struct RBT_Node {
    enum Color color;           /* 节点的颜色 */
    struct RBT_Node * left;     /* 左孩子 */
    struct RBT_Node * right;    /* 右孩子 */
    type_t data;                /* 节点数据 */
} RBT_Node;

/* 红黑树结构 */
typedef struct {
    struct RBT_Node * root;     /* 根节点 */
    int size;                   /* 树大小 */
} RBT;

/* 搜索 */
RBT_Node * RBT_search(const RBT * tree, type_t obj);

/* 插入 */
void RBT_insert(RBT * tree, type_t obj);

/* 删除 */
void RBT_delete(RBT * tree, type_t obj);

/* 清除整棵树 */
void RBT_clear(RBT * tree);

#endif
