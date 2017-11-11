"""
值迭代
"""

from MDP import *


class ValueIteration(MDP):

    def __init__(self, S, A, P, R, gama):
        super().__init__(S, A, P, R, gama)
        self.Q = QValue(self)   # Q值
        self.h = dict()         # 存放结果

    def run(self):
        """
        迭代计算Q值
        :return:
        """
        while True:
            self.Q.clear_change()
            for (cs, a) in self.get_sa_pair():
                tmp_q = 0       # q值
                for ns in self.S:
                    max_q = self.Q.get(ns, self._max_item(self.Q.get(ns)))
                    tmp_q += self.P(cs, a, ns) * (self.R(cs, a, ns) + self.gama * max_q)
                self.Q.assign(cs, a, tmp_q)
            self.Q.print()
            if not self.Q.has_change():
                break

        for s in self.S:
            tmp = self.Q.get(s)
            key = self._max_item(tmp)
            self.h[s] = {"h": key, "V": tmp[key]}

    def get_h(self):
        return self.h

    def _max_item(self, dic):
        tmp = -1
        key = None
        for i in dic:
            if dic[i] > tmp:
               key = i
               tmp = dic[i]
        return key
