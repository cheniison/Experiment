"""
进程控制类
"""


"""
PCB
进程控制块
"""
class PCB(object):
    def __init__(self, name, a_time, e_time, priority):
        self.name = name            # 进程名
        self.u_time = 0             # 已经被调度的时间
        self.a_time = a_time        # 进程到来的时间
        self.e_time = e_time        # 预计需要被调度的时间
        self.priority = priority    # 优先级


"""
进程控制块队列
"""
class PCBList(object):
    def __init__(self):
        self.pcb_list = list()

    def append(self, pcb):
        self.pcb_list.append(pcb)

    def pop(self):
        return self.pcb_list.pop(0)

    def pop_min_e_time(self):
        index = 0
        for i in range(0, len(self.pcb_list)):
            if self.pcb_list[i].e_time < self.pcb_list[index].e_time:
                index = i
        return self.pcb_list.pop(index)

    def pop_max_priority(self):
        index = 0
        for i in range(0, len(self.pcb_list)):
            if self.pcb_list[i].priority < self.pcb_list[index].priority:
                index = i
        return self.pcb_list.pop(index)

    def empty(self):
        return len(self.pcb_list) == 0
