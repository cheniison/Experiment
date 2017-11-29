#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from PCB import *

FILE_NAME = "JOB1.TXT"
NOPS = PCB("E", 0, 0, 0)         # 空进程


def read_data(file_name):

    f_input = open(file_name, "r", encoding="utf-8")

    ps_num = f_input.readline()

    ps_list = list()

    for line in f_input.readlines():
        name, a_time, e_time, priority = line.strip().split()
        ps_list.append(PCB(name, int(a_time), int(e_time), int(priority)))

    return ps_list, ps_num


def print_res(res):
    string = ""
    for i in res:
        for j in range(0, i[1]):
            string += i[0].name
    print(string)


def sjf(ps_list):
    ps = ps_list.pop_min_e_time()
    ps.u_time += ps.e_time
    return ps.e_time, ps


def rr(ps_list, t):
    ps = ps_list.pop()
    if ps.u_time + t >= ps.e_time:
        t = ps.e_time - ps.u_time
    else:
        ps_list.append(ps)
    ps.u_time += t

    return t, ps

def pf(ps_list):
    ps = ps_list.pop_min_priority()
    ps.u_time += ps.e_time
    return ps.e_time, ps


def schedule(ps_list, result):
    """
    调度函数
    :param ps_list: 可被调度的进程列表
    :return: 调度的时间
    """
    if ps_list.empty():
        result.append([NOPS, 1])
        return 1
    else:
        t, ps = pf(ps_list)
        result.append([ps, t])
        return t


ps_rest, ps_num = read_data(FILE_NAME)
ps_rest = sorted(ps_rest, key=lambda x:x.a_time)    # ps_rest: 还没有到来的进程

ps_list = PCBList()     # ps_list: 已经到来的进程

counter = 0         # 时钟
result = list()     # 调度结果

while True:

    if len(ps_rest) == 0 and ps_list.empty():
        # 当没有进程需要被调度时，退出
        break

    while len(ps_rest) != 0 and ps_rest[0].a_time <= counter:
        # 将到达的进程放到进程队列中
        ps_list.append(ps_rest.pop(0))

    # 调度
    counter += schedule(ps_list, result)

print_res(result)