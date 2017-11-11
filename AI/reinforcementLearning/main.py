#!/usr/bin/env python3


from ValueIteration import *
from PolicyIteration import *


S = [0, 1, 2, 3, 4, 5]      # 状态
A = [-1, 1]                 # 动作
gama = 0.5                  # 折扣因子

"""
确定情况下的概率和奖赏函数
"""


def dP(cs, a, ns):
    if cs == 0 or cs == 5 or cs + a != ns:
        return 0
    else:
        return 1


def dR(cs, a, ns):
    if cs == 1 and a == -1:
        return 1
    elif cs == 4 and a == 1:
        return 5
    else:
        return 0


"""
随机情况下的概率和奖赏函数
"""


def aP(cs, a, ns):
    if cs == 0 or cs == 5:
        return 0
    elif cs + a == ns:
        return 0.8
    elif cs - a == ns:
        return 0.05
    elif cs == ns:
        return 0.15
    else:
        return 0


def aR(cs, a, ns):
    if cs == 0 or cs == 5:
        return 0
    elif ns == 0:
        return 1
    elif ns == 5:
        return 5
    else:
        return 0


"""
值迭代
"""

print("\n")
print("===============================")
print("\n")

print("确定情况下的值迭代:")
print("Q值变化过程:")
vi = ValueIteration(S, A, dP, dR, gama)
vi.run()
print("结果:")
print(vi.get_h())

print("\n")
print("===============================")
print("\n")

print("随机情况下的值迭代:")
print("Q值变化过程:")
vi = ValueIteration(S, A, aP, aR, gama)
vi.run()
print("结果:")
print(vi.get_h())

"""
策略迭代
"""

print("\n")
print("===============================")
print("\n")

print("确定情况下的策略迭代:")
print("h值变化过程:")
pi = PolicyIteration(S, A, dP, dR, gama)
pi.run()
print("结果:")
print(pi.get_h())

print("\n")
print("===============================")
print("\n")

print("随机情况下的策略迭代:")
print("h值变化过程:")
pi = PolicyIteration(S, A, aP, aR, gama)
pi.run()
print("结果:")
print(pi.get_h())
