"""
进程调度方式
"""


class PSM(object):

    def __init__(self, name, func):
        self.name = name
        self.func = func


# 最短作业优先（非抢占）
def psm_sjf(ps_list):
    ps = ps_list.pop_min_e_time()
    ps.u_time += ps.e_time
    return ps.e_time, ps


# 时间片轮转（时间片为t）
def psm_rr(ps_list, t):
    ps = ps_list.pop()
    if ps.u_time + t >= ps.e_time:
        t = ps.e_time - ps.u_time
    else:
        ps_list.append(ps)
    ps.u_time += t

    return t, ps


# 优先级调度
def psm_pf(ps_list):
    ps = ps_list.pop_max_priority()
    ps.u_time += ps.e_time
    return ps.e_time, ps