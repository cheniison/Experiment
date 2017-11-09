"""
确定情况下的Q值迭代
"""

from dMDP_api import *



Q = [[0 for col in range(len(U))] for row in range(len(X))]

flag = True
while flag == True:
    for x in X:
        for u in U:
            Q(x, u) = p(x, u) + gama * max(Q(f(x, u)))
