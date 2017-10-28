#!/usr/bin/env python3
# -*- coding:utf-8 -*-


INDEX_NAME = "dict.index"       # 索引文件名

TOKEN_TYPE_OPERATOR = 0               # 操作符类型
TOKEN_TYPE_OPERAND = 1               # 操作数类型


class Token(object):

    def __init__(self, token_name, token_type):
        self.token_name = token_name
        self.token_type = token_type

    def is_op(self):
        return self.token_type == TOKEN_TYPE_OPERATOR


    # 若单词在索引表中，返回内容，否则返回set()
    def get_index(self, table):
        if self.token_name in table:
            return table[self.token_name]
        else:
            return set()


# 词法分析
def scan(query):
    words = query.split()
    tokens = list()
    tokens.append(Token("(", TOKEN_TYPE_OPERATOR))        # 默认加上括号
    for word in words:
        word = word.lower()
        if word == "and" or word == "or":
            if tokens[-1].is_op() and tokens[-1].token_name != ")":
                # 前一个是操作符，这个是操作数
                tokens.append(Token(word, TOKEN_TYPE_OPERAND))
            else:
                # 前一个是操作数，这个是操作符
                tokens.append(Token(word, TOKEN_TYPE_OPERATOR))
        elif word == "(":
            tokens.append(Token(word, TOKEN_TYPE_OPERATOR))
        elif word == ")":
            tokens.append(Token(word, TOKEN_TYPE_OPERATOR))
        else:
            if word[0] == "\\":
                # 转义开头的字符
                word = word[1:]
                if (word == ""):
                    raise Exception("不能使用 '\\' 作为操作数")

            tokens.append(Token(word, TOKEN_TYPE_OPERAND))

    tokens.append(Token(")", TOKEN_TYPE_OPERATOR))        # 默认加上括号
    return tokens


# 比较两个运算符的优先级
def cmp_op(op1, op2):
    priority = {
        "(" : 0,
        ")" : 10,
        "and" : 6,
        "or" : 3
    }
    return priority[op1] - priority[op2]

# 将操作符应用到操作数上
def apply_op(op, stack_od):
    if op == "and" and len(stack_od) >= 2:
        stack_od.append(stack_od.pop() & stack_od.pop())
    elif op == "or" and len(stack_od) >= 2:
        stack_od.append(stack_od.pop() | stack_od.pop())
    else:
        raise Exception("未知的运算符，或操作数过少")


# 调度场算法解析
def parse(tokens, table):
    # factor := string ( "and" string )*
    # exp := factor ( "or" factor )*
    
    stack_op = list()       # 操作符栈
    stack_od = list()       # 操作数栈

    for token in tokens:
        if token.is_op():
            # 是操作符
            if token.token_name == "(":
                stack_op.append("(")
            elif token.token_name == ")":
                while True:
                    if len(stack_op) == 0:
                        # 操作符为空，出错
                        raise Exception("括号不匹配")
                    elif stack_op[-1] == "(":
                        stack_op.pop()
                        break
                    else:
                        op = stack_op.pop()
                        apply_op(op, stack_od)
                        
            else:
                while cmp_op(stack_op[-1], token.token_name) >= 0:
                    # 当前的操作符优先级低或相等
                    op = stack_op.pop()
                    apply_op(op, stack_od)
                    
                # 当前操作符优先级比前一个操作符高
                stack_op.append(token.token_name)

        else:
            # 是操作数
            stack_od.append(token.get_index(table))

    if len(stack_od) > 1:
        # 出错
        raise Exception("操作数数量不匹配")
    elif len(stack_od) == 1:
        return stack_od[0]
    else:
        return set()


# 打开索引文件
f_index = open(INDEX_NAME, "r", encoding="utf-8")
# 读入每行数据
lines = list(map(lambda l: l.strip().split(), f_index.readlines()))

# 索引表
index_table = dict()
for line in lines:
    index_table[line[0]] = set(line[2:])


while True:
    query = ""

    # 接受输入
    try:
        query = input()
    except Exception as e:
        print("输入结束")
        exit(0)

    try:
        tokens = scan(query)    # 词法分析
        res = parse(tokens, index_table)    # 解析
        res = sorted(list(map(lambda x: int(x), res)))
        print (res)
    except Exception as e:
        print(e) 

