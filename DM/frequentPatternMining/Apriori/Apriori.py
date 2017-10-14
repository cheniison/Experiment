#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import functools
from functools import reduce

# 数据文件名
file_name = "../data.txt"
# 最小支持度
minimum_support = 3 / 10


# 项集
class Itemset(object):
    
    def __init__(self, items = None, support = 0):
        if items == None:
            items = set()
        self.items = set(items)
        self.support = support

    def setSupport(self, support):
        self.support = support

    def setItems(self, items):
        self.items = items

    def getSupport(self):
        return self.support

    def getItems(self):
        return self.items


# 从候选集中过滤出频繁集
def filter_frequent_items(cs, all_trans, min_sup):
    
    # 求出项集支持度
    def cal_support(itemset):
        num = 0
        for trans in all_trans:
            # 判断项集是否是交易的子集
            if set(itemset).difference(set(trans)) == set():
                # 是子集
                num += 1
        return num / len(all_trans)
        
    res = list()
    for itemset in cs:
        # 计算支持度
        support = cal_support(itemset.getItems())
        if support >= min_sup:
            res.append(Itemset(itemset.getItems(), support))

    return res


# 根据频繁集产生候选集
def generate_candidate_set(fi):

    # 候选集的长度
    itemset_len = len(fi[0].getItems()) + 1
    res = []
    i = len(fi) - 1
    while i >= 0:
        j = i - 1
        while j >= 0:
            # 取两项集的并集
            itemset = set(fi[i].getItems()).union(set(fi[j].getItems()))
            if len(itemset) == itemset_len and not set(itemset) in res:
                res.append(set(itemset))
            j -= 1
        i -= 1

    return map(lambda i: Itemset(i), res)


# 打开文件
f_data = open(file_name, "r", encoding="utf8")

# 读出交易数和项目种数
trans_nums, item_nums = f_data.readline().split()

# 读出所有的交易
all_trans = list(map(lambda s: s.strip().split(), f_data.readlines()))

# 1-候选集
cs = list(map(lambda i: Itemset([i]), set(reduce(lambda a, b: a + b, all_trans))))

# 项目种数错误
if len(cs) != int(item_nums):
    print ("项目种数出错\n")
    exit(1)

res = []

while True:

    # 求出 k-频繁集
    fi = filter_frequent_items(cs, all_trans, minimum_support)

    if (fi == list()):
        # 频繁集为空，结束
        break

    # 将 k-频繁集 加入到结果集
    res += fi

    # 求出 k+1-候选集
    cs = generate_candidate_set(fi)
    

# 输出结果
for i in res:
    print (i.getItems(), '\t', i.getSupport())


