#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from functools import reduce
from operator import attrgetter

# 数据文件名
file_name = "../data.txt"
# 最小支持度
minimum_support = 3 / 10


# 节点
class Node(object):
    def __init__(self, item, support=0, next_node=None, children_node=None):
        if children_node is None:
            children_node = list()
        self.item = item
        self.support = support
        self.next_node = next_node
        self.children_node = children_node


# 根据交易构造项头表和fp树
def generate_fp_tree(transaction):
    # 项目及其次数
    items = dict()
    for trans in transaction:
        for item in trans:
            if item in items:
                items[item] += 1
            else:
                items[item] = 1

    # 1-频繁集
    fi_1 = set(map(lambda k: k[0], filter(lambda k: k[1] >= minimum_num, items.items())))

    # 构造项头表
    header_table = list(map(lambda k: Node(k, items[k]), fi_1))

    # 将项头表排序
    header_table.sort(key=attrgetter("support"), reverse=True)

    fp_tree = Node("")

    # 返回项目在项头表中的位置
    def item_index_in_header_table(item):
        i = len(header_table) - 1
        while i >= 0:
            if header_table[i].item == item:
                return i
            i -= 1
        return -1

    # 构造 FP-树
    for trans in transaction:
        # 选出 1-频繁子集
        trans = list(set(filter(lambda item: item in fi_1, trans)))
        # 按照项头表的顺序排序
        trans.sort(key=item_index_in_header_table)

        # 当前的节点
        cur_node = fp_tree
        for item in trans:
            flag = 0
            for child in cur_node.children_node:
                # 遍历当前节点孩子节点
                if child.item == item:
                    # 找到了项目
                    flag = 1
                    cur_node = child  # 将当前节点设置为此节点
                    cur_node.support += 1  # 支持度加一
                    break
            if flag == 0:
                # 没找到项目
                tmp = Node(item, 1)  # 新建节点
                cur_node.children_node.append(tmp)  # 将节点加到孩子中
                cur_node = tmp  # 将当前节点设置为此节点

                # 让项头表中的节点的 next 指向新节点
                for n in header_table:
                    if n.item == item:
                        node_link = n
                        while node_link.next_node is not None:
                            node_link = node_link.next_node
                        node_link.next_node = cur_node
                        break
    return header_table, fp_tree


# 判断fp-tree是否是单路径
def is_single_path(tree):
    length = len(tree.children_node)
    if length == 0:
        return True
    elif length == 1:
        return is_single_path(tree.children_node[0])
    else:
        return False


# 两个节点的组合
def union_node(na, nb):
    if na.item == "":
        # na是空集
        if not isinstance(nb.item, list):
            nb = Node([nb.item], nb.support)
        # 返回nb
        return Node(nb.item, nb.support)
    if nb.item == "":
        # nb是空集
        if not isinstance(na.item, list):
            na = Node([na.item], na.support)
        # 返回na
        return Node(na.item, na.support)
    if not isinstance(na.item, list):
        na = Node([na.item], na.support)
    if not isinstance(nb.item, list):
        nb = Node([nb.item], nb.support)

    # 支持度取最小的
    return Node(na.item + nb.item, min(na.support, nb.support))


# 一条单路径上所有的组合
def all_combination(single_path_tree):
    if len(single_path_tree.children_node) == 0:
        return [Node(single_path_tree.item, single_path_tree.support)]
    else:
        ac = all_combination(single_path_tree.children_node[0])
        return list(map(lambda n: union_node(n, single_path_tree), ac)) + ac \
               + [Node(single_path_tree.item, single_path_tree.support)]


# 根据项头表，fp树和模式，构造条件fp树
# 返回新的项头表和条件fp树
def generate_condition_fp_tree(header_table, tree, pattern_num):

    # 根据fp树生成交易
    def generate_trans(root):
        res = list()
        for c in root.children_node:
            if c.item == header_table[pattern_num].item:
                res.extend([[root.item]] * c.support)
            else:
                tmp = generate_trans(c)
                res += list(map(lambda i: i + [root.item], tmp))
        return res

    tmp = list(reduce(lambda x, y: x + y, map(generate_trans, tree.children_node)))
    return generate_fp_tree(tmp)


# fp-growth 挖掘算法
def fp_growth(header_table, tree, pattern, min_support):
    res = list()
    if is_single_path(tree):
        # 是单路径
        if len(tree.children_node) != 0:
            # 树不是空的
            ac = all_combination(tree.children_node[0])  # 求出所有组合
            res = list(map(lambda n: union_node(n, pattern), ac))  # 和模式进行组合
        else:
            # 树是空的
            res = []
    else:
        # 不是单路径
        i = len(header_table) - 1
        while i >= 0:
            # 产生一个新的模式
            pattern_i = union_node(pattern, header_table[i])
            pattern_i.support = header_table[i].support
            res += [Node(pattern_i.item, pattern_i.support)]
            header_table_i, tree_i = generate_condition_fp_tree(header_table, tree, i)  # 构造条件fp树
            res += fp_growth(header_table_i, tree_i, pattern_i, min_support)    # 对条件fp树进行挖掘
            i -= 1
    res = list(filter(lambda r: r.support >= min_support, res))
    return res


# 打开文件
f_data = open(file_name, "r", encoding="utf-8")

# 读出交易数和项目种数
trans_nums, item_nums = f_data.readline().split()

# 最小的出现次数
minimum_num = minimum_support * int(trans_nums)

# 读出所有的交易
all_trans = list(map(lambda s: s.strip().split(), f_data.readlines()))

# 根据数据，产生项头表和原始fp树
ht, ft = generate_fp_tree(all_trans)

# 对原始fp树挖掘
res = fp_growth(ht, ft, Node(""), minimum_num)

# 输出
for p in res:
    print(set(p.item), '\t', p.support / int(trans_nums))
