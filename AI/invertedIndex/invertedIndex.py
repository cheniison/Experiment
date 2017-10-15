#!/usr/bin/env python3
# -*- coding:utf-8 -*-

from functools import reduce

# 应该读入的文件
files_name = list(map(lambda n: "files/d" + str(n) + ".txt" , range(1,11)))


class Word(object):

    def __init__(self, word = "", frequency = 0, fds = []):
        self.word = word
        self.frequency = frequency
        self.fds = fds

    def increment_frequency(self):
        self.frequency += 1

    def append_fds(self, fd):
        self.fds.append(fd)

# 索引表
index_table = dict()
fd = 0
for file_name in files_name:
    # 打开文件
    f_input = open(file_name, "r", encoding = "utf-8")
    # 增加当前文件的 ID
    fd += 1
    # 读入行
    lines = list(map(lambda l: l.strip().split(), f_input.readlines()))
    # 关闭文件
    f_input.close()
    # 合并到单词
    words = set(reduce(lambda x,y: x+y, lines))

    for word in words:
        if word in index_table:
            index_table[word].increment_frequency()
            index_table[word].append_fds(fd)
        else:
            index_table[word] = Word(word, 1, [fd])

# 将结果写入结果文件
output_file = "dict.index"

# 打开结果文件
f_output = open(output_file, "w", encoding = "utf-8")

for w in index_table.items():
    s = w[1].word + "\t" + str(w[1].frequency) + '\t'
    for fd in w[1].fds:
        s += str(fd) + " "
    s  += "\n"
    f_output.write(s)
