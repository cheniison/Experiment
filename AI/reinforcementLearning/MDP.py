"""
马尔科夫决策过程
"""


class MDP(object):
    def __init__(self, S, A, P, R, gama):
        """
        构造函数
        :param S: 状态集合
        :param A: 动作集合
        :param P: 转移概率
        :param R: 奖赏函数
        :param gama: 折扣因子
        """
        self.S = S
        self.A = A
        self.P = P
        self.R = R
        self.gama = gama

    def get_sa_pair(self):
        res = list()
        for s in self.S:
            for a in self.A:
                res.append([s, a])
        return res


class QValue(object):
    """
    Q 值
    """

    def __init__(self, mdp):
        """
        初始化 Q值
        :param mdp:
        """
        self.has_changed = True
        self.Q = dict()
        for (s, a) in mdp.get_sa_pair():
            if not s in self.Q:
                self.Q[s] = dict()
            self.Q[s][a] = 0

    def assign(self, s, a, value):
        """
        赋值
        :param s: 某状态
        :param a: 某动作
        :param value: 值
        :return: void
        """
        if self.Q[s][a] != value:
            self.Q[s][a] = value
            self.has_changed = True

    def get(self, s, a=None):
        """
        获得某状态某动作下的Q值
        :param s: 某状态
        :param a: 某动作
        :return: Q 值
        """
        if a is None:
            return self.Q[s]
        else:
            return self.Q[s][a]

    def has_change(self):
        """
        是否有值改变
        :return: True or False
        """
        return self.has_changed

    def clear_change(self):
        """
        设置改变位(将改变位置为True)
        :return:
        """
        self.has_changed = False

    def print(self):
        """
        打印 Q值
        :return:
        """
        print(self.Q)
