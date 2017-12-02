#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from PCB import *
from PSM import *
import functools


FILE_NAME = "JOB1.TXT"
NOPS = PCB("E", 0, 0, 0)         # 空进程
psc = NOPS                      # 当前正在执行的进程

def read_data(file_name):

    f_input = open(file_name, "r", encoding="utf-8")

    ps_num = f_input.readline()

    ps_list = list()

    for line in f_input.readlines():
        name, a_time, e_time, priority = line.strip().split()
        ps_list.append(PCB(name, int(a_time), int(e_time), int(priority)))

    return ps_list, ps_num


def print_res(res):
    """
    打印结果
    :param res: 结果集
    :return: void
    """
    string = ""
    detail = list()                 # 每个进程的调度结果，格式（进程对象， 响应时间， 结束时间）
    t = 0                           # 周转时间
    for i in res:
        for j in range(0, i[1]):
            string += i[0].name
        if i[0] == NOPS:
            t += i[1]
            continue
        flag = 0
        for j in detail:
            if j[0] == i[0]:
                t += i[1]
                j[2] = t
                flag = 1
                break
        if flag == 0:
            detail.append([i[0], t - i[0].a_time, t + i[1]])
            t += i[1]

    print(string)
    for i in detail:
        string = ""
        string += i[0].name
        string += " "
        string += "响应时间: "
        string += str(i[1])
        string += " "
        string += "周转时间: "
        string += str(i[2] - i[0].a_time)
        print(string)


def schedule(ps_list, result, psm):
    """
    调度函数
    :param ps_list: 可被调度的进程列表
    :return: 调度的时间
    """
    global psc

    if ps_list.empty():
        result.append([NOPS, 1])
        psc = NOPS
        return 1
    else:
        t, ps = psm(psc, ps_list)
        psc = ps
        result.append([ps, t])
        return t


# 进程调度方式
psms = [PSM("最短作业优先（非抢占）", psm_sjf),
        PSM("时间片轮转（时间片为1）", functools.partial(psm_rr, t=1)),
        PSM("时间片轮转（时间片为2）", functools.partial(psm_rr, t=2)),
        PSM("时间片轮转（时间片为3）", functools.partial(psm_rr, t=3)),
        PSM("优先级调度", psm_pf)]


for psm in psms:

    ps_rest, ps_num = read_data(FILE_NAME)
    ps_rest = sorted(ps_rest, key=lambda x: x.a_time)  # ps_rest: 还没有到来的进程

    ps_list = PCBList()  # ps_list: 已经到来的进程

    counter = 0  # 时钟
    result = list()  # 调度结果

    while True:

        if len(ps_rest) == 0 and ps_list.empty():
            # 当没有进程需要被调度时，退出
            break

        while len(ps_rest) != 0 and ps_rest[0].a_time <= counter:
            # 将到达的进程放到进程队列中
            ps_list.append(ps_rest.pop(0))

        # 调度
        counter += schedule(ps_list, result, psm.func)

    print("===================")
    print(psm.name)
    print_res(result)
