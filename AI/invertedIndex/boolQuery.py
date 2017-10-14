#/usr/bin/env python3
# -*- coding:utf-8 -*-

# 索引文件名
index_name = "dict.index"
# 打开索引文件
f_index = open(index_name, "r", encoding="utf-8")
# 读入每行数据
lines = list(map(lambda l: l.strip().split(), f_index.readlines()))

# 索引表
index_table = dict()
for line in lines:
    index_table[line[0]] = set(line[2:])


while True:
    query = input().split()
    w1 = query[0]
    w2 = query[2]
    op = query[1]
    if op == 'and':
        if not w1 in index_table or not w2 in index_table:
            print("空")
        else:
            print(index_table[w1] & index_table[w2])
