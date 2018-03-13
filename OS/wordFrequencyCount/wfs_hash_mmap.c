#define _GNU_SOURCE     /* 防止fmemopen未定义 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef FILE_NAME
#define FILE_NAME "essay.txt"   /* 文件名 */
#endif

#ifndef MAX_LEN
#define MAX_LEN 50              /* 单词最长长度 */
#endif

#ifndef MAX_WORDS
#define MAX_WORDS 50000         /* 最多的单词种数 */
#endif

#ifndef TABLE_SIZE
#define TABLE_SIZE 4096         /* 为了提高求模速度，使用2的次方 */
#endif

typedef struct s_wordlist {
	struct s_wordlist * next;
	char word[MAX_LEN];
	size_t num;
} WordList;

WordList word_table[TABLE_SIZE];
WordList words[MAX_WORDS];
size_t witer = 0;

/* 哈希函数 */
inline uint64_t hash(const char * str, size_t len, uint64_t mod)
{
	uint64_t res = 0;
    size_t i = 0;

	for (; i < len; ++i) {
        res = (res * 27 + str[i]) % mod;
	}

	return res;
}

int main(int argc, char **argv)
{
	int fin;
	char * article;
	uint64_t hash_value;
	size_t i, j, index, len;
	WordList * wlp;
	struct stat fstatus;
	clock_t s, e;
	int ch;

	s = clock();

	fin = open(FILE_NAME, O_RDONLY);

	if (fstat(fin, &fstatus) < 0) {
		return 1;
	}

    len = fstatus.st_size;
    /* 存储映射 */
	article = (char *)mmap(0, len, PROT_READ, MAP_PRIVATE, fin, 0);
    /* 改变VMA标志 */
    madvise((void *)article, len, MADV_SEQUENTIAL);

	i = 0;
 	for (index = 0; index <= len; ++index) {
        
        ch = article[index];
        
        /* if (! isalpha(ch)) { */
        if (! ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))) {
            words[witer].word[i++] = '\0';
            if (i != 1) {
                /* 非字母单词，单词结束 */
                /* 计算单词哈希值 */
                hash_value = hash(words[witer].word, i - 1, TABLE_SIZE);
                wlp = &word_table[hash_value];
                while (wlp->next != NULL) {
                    /* 比较两字符串是否相等 */
                    for (j = 0; j < i; ++j) {
                        if (wlp->next->word[j] != words[witer].word[j]) {
                            break;
                        }
                    }
                    if (j == i) {
                        /* 相等，增加计数 */
                        ++wlp->next->num;
                        break;
                    }
                    wlp = wlp->next;
                }
                if (wlp->next == NULL) {
                    /* 新单词，分配新的结点 */
                    wlp->next = &words[witer++];
                    wlp->next->num = 1;
                    wlp->next->next = NULL;
                }

            }
            i = 0;
        } else {
            if (ch <= 'Z') {
                ch += 'a' - 'A';
            }
             
            words[witer].word[i++] = ch;
		}
	}

	e = clock();
	for (i = 0; i < TABLE_SIZE; ++i) {
		wlp = word_table[i].next;
		while (wlp != NULL) {
			printf("%lu\t%s\n", wlp->num, wlp->word);
			wlp = wlp->next;
		}
	}

	/* 解除映射区 */
	munmap(article, len);

	printf("%f\n", (double)(e - s) / CLOCKS_PER_SEC);
	return 0;
}
