#!/usr/bin/env python3
#  -*- coding: utf-8 -*-

import functools
import argparse
from functools import reduce
from datetime import datetime

###########################
# deal with CLI arguments #
###########################

parser = argparse.ArgumentParser(description='This is a word segmentation program.')
parser.add_argument('-i', '--input-file', nargs=1, required=True,
                    help='Path of the file which need to be segmented.')
parser.add_argument('-d', '--dict-file', nargs=1, required=True,
                    help='Path of the dictionary file. First line must be words count and word max length.')
parser.add_argument('-a', '--answer-file', nargs=1, required=True,
                    help='Path of the answer file.')
parser.add_argument('-o', '--out-file', nargs=1, required=True,
                    help='Path of the output file.')
parser.add_argument('-v', '--version', action='version', version='%(prog)s 1.0')
print(parser.parse_args())


#############################
# deal with dictionary file #
#############################

# open dict file
f_words = open('./corpus.dict.txt', 'r', encoding='utf8')

# read dict file
words_num, word_max_length = f_words.readline().split()

# read words
words = set(map(lambda x: x.strip(), f_words.readlines()))

# close dict file
f_words.close()


########################
# deal with input file #
########################


def word_seg(string, dictionary, word_max_len):
    """
    Word segmentation
    :param string: string to be segmentation
    :param dictionary: words set
    :param word_max_len: max word length in words set
    :return: result of segmentation
    """
    res = []
    str_len = len(string)

    i = 0
    while i < str_len:
        j = word_max_len
        while j > 1:
            temp_word = string[i: i + j]
            if temp_word in dictionary:
                break
            j = j - 1
        res.append(string[i: i + j])
        i = i + j

    return res


# open input file
f_input = open('./corpus.sentence.txt', 'r', encoding='utf8')

# get input file lines without "\n"
# format: ['line1', 'line2', ...]
input_lines = list(map(lambda line: line.strip(), f_input.readlines()))

# close input file
f_input.close()

# apply segmentation on each line
# format: [['line1-word1', 'line1-word2', ...], ['line2-word1', ...], ...]
input_lines = list(map(functools.partial(word_seg, dictionary=words, word_max_len=int(word_max_length)), input_lines))

#########################
# deal with output file #
#########################

# generate output content
# [['line1-word1', 'line1-word2', ...], ...] => ['line1-word1 line1-word2 ...\n', ...]
output_content = list(map(lambda line: str(reduce(lambda a, b: a + " " + b, line)) + "\n", input_lines))

# open output file
with open('corpus.out.txt', 'w', encoding='utf8') as f_output:
    # write content into output file
    f_output.writelines(output_content)

#########################
# deal with answer file #
#########################

# open answer file
f_answer = open('./corpus.answer.txt', 'r', encoding='utf8')

# get answer lines list
# format: [['line1-word1', 'line1-word2', ...], ['line2-word1', ...], ...]
answer_lines = list(map(lambda line: line.strip().split(), f_answer.readlines()))


########################
# calculate P / R / F  #
########################


def lcs(list1, list2):
    """
    Longest common subsequence
    :param list1:
    :param list2:
    :return: lcs
    """
    if len(list1) == 0 or len(list2) == 0:
        return []
    if list1[-1] == list2[-1]:
        return lcs(list1[:-1], list2[:-1]) + [list1[-1]]
    else:
        lcs1 = lcs(list1[:-1], list2[:])
        lcs2 = lcs(list1[:], list2[:-1])
        if len(lcs1) > len(lcs2):
            return lcs1
        else:
            return lcs2


def lcs_dp(list1, list2):
    """
    Longest common subsequence using dynamic programming
    :param list1:
    :param list2:
    :return: lcs
    """
    len1 = len(list1)
    len2 = len(list2)

    lcs_arr = {0: {}}  # lcs dp array. size: (len1 + 1) * (len2 + 1)
    for i in range(len2 + 1):
        lcs_arr[0][i] = []

    i = 1
    while i <= len1:
        lcs_arr[i] = {}
        lcs_arr[i][0] = []
        j = 1
        while j <= len2:
            if list1[i - 1] == list2[j - 1]:
                lcs_arr[i][j] = list(lcs_arr[i - 1][j - 1]) + [list1[i - 1]]
            else:
                if len(lcs_arr[i - 1][j]) > len(lcs_arr[i][j - 1]):
                    lcs_arr[i][j] = lcs_arr[i - 1][j]
                else:
                    lcs_arr[i][j] = lcs_arr[i][j - 1]
            j = j + 1
        i = i + 1

    return lcs_arr[len1][len2]


def cal_correct_words(line_src, line_ans):
    """
    Calculate the correct words in list
    :param line_src: the line to be calculated
    :param line_ans: the line to be contrasted
    :return: correct words in line_src
    """
    # return lcs_dp(line_src, line_ans)                           # accurate & fast
    # return list(filter(lambda s: s in line_ans, line_src))    # inaccurate
    res = lcs(line_src, line_ans)                            # too slow
    print(datetime.now())
    print(res)
    return res

def cal_words_num(lines):
    """
    Calculate number of words in lines
    :param lines: lines to be calculate
    :return: number of words
    """
    return sum(map(len, lines))


# get correct words in each line
correct_lines = list(map(cal_correct_words, input_lines, answer_lines))

correct_words_num = cal_words_num(correct_lines)

input_words_num = cal_words_num(input_lines)

answer_words_num = cal_words_num(answer_lines)

# calculate P / R / F
precision = correct_words_num / input_words_num
recall = correct_words_num / answer_words_num
F = precision * recall * 2 / (precision + recall)

print("Precision: %.2f%%\nRecall: %.2f%%\nF measure: %.2f%%" %
      (precision * 100.0, recall * 100.0, F * 100.0))

exit(0)
