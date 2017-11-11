"""
策略迭代
"""

from MDP import *


class PolicyIteration(MDP):
    def __init__(self, S, A, P, R, gama):
        super().__init__(S, A, P, R, gama)
        self._init_h()  # 存放结果
        self.Q = None

    def run(self):
        num = 0
        while True:
            """
            迭代计算Q值
            """
            num += 1
            print("第", num, "次迭代")
            self.Q = QValue(self)  # Q值
            while True:
                self.Q.clear_change()
                for (cs, a) in self.get_sa_pair():
                    tmp_q = 0  # q值
                    for ns in self.S:
                        tmp_q += self.P(cs, a, ns) * (self.R(cs, a, ns) + self.gama * self.Q.get(ns, self.h[ns]))
                    self.Q.assign(cs, a, tmp_q)
                self.Q.print()
                if not self.Q.has_change():
                    break

            """
            计算h值
            """
            flag = False
            for s in self.S:
                key = self._max_item(self.Q.get(s))
                if self.h[s] != key:
                    self.h[s] = key
                    flag = True
            print("h值: ")
            print(self.h)
            print("")
            if flag == False:
                # h值不变，迭代完成
                break

        for s in self.S:
            tmp = self.Q.get(s)
            key = self._max_item(tmp)
            self.h[s] = {"h": key, "V": tmp[key]}

    def _init_h(self):
        self.h = dict()
        for s in self.S:
            self.h[s] = self.A[1]

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
