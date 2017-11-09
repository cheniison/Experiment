"""
deterministic MDP API
确定情况下API
"""

X = list(0,1,2,3,4,5)       # 状态空间
U = list(-1, 1)             # 动作空间


def f(x, u):
    """
    迁移函数
    """
    if x == 0 or x == 5:
        return x
    else:
        return x + u


def p(x, u):
    """
    奖励函数
    """
    if x == 4 and u == 1:
        return 5
    elif x == 1 and u == -1:
        return 1
    else:
        return 0
