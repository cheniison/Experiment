
# invertedIndex
倒排索引与布尔查询

---------


## 环境
+ python 3.5.2


## 构建及运行

### Windows
运行示例程序，键入以下命令

    python invertedIndex.py
    python boolQuery.py

### Linux
运行事例程序，键入以下命令

    python3 invertedIndex.py
    python3 boolQuery.py

或者

    chmod +x invertedIndex.py && ./invertedIndex.py
    chmod +x boolQuery.py && ./boolQuery.py


## 注意
+ 请先运行 invertedIndex.py 以生成必要的倒排索引文件
+ 使用调度场算法解析查询，支持括号，支持使用'\\'转义第一个字符

## 例子

    a and b             # 查询 a 和 b 的索引的交集
    a or b              # 查询 a 和 b 的索引的并集
    a or b and c        # or 的优先级比 and 低，等价于查询 a or ( b and c )
    ( a or b ) and c    # 使用括号，优先计算括号内部的表达式
    \( and a            # 转义第一个字符，等价于查询 ( and a
    \\( and a           # 转义第一个字符，等价于查询 \( and a
    \ab and a           # 转义第一个字符，等价于查询 ab and a
    a\b and a           # 不转义其他字符，等价于查询 a\b and a
    \ and a             # 出错
