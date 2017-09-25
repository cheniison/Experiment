#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

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
	char word[MAX_LEN];				/* 单词 */
	size_t num;						/* 词频 */
} WordList;

WordList word_table[TABLE_SIZE];    /* 哈希表 */

WordList words[MAX_WORDS];          /* 预先分配的空间（减少使用 malloc 系统调用） */
size_t witer = 0;                   /* 预先分配的空间最后一个元素的后一位下标 */


/* 哈希函数 */
inline uint64_t hash(const char * str, uint64_t mod)
{
	uint64_t res = 0;

	for (; *str != '\0'; ++str) {
        res = (res + *str) * 26 % mod;
	}

	return res;
}

int main(int argc, char **argv)
{
	FILE * fin;             /* 文件指针 */
	char word[MAX_LEN];     /* 存储一个单词 */
	uint64_t hash_value;    /* 存储 hash 值 */
	size_t i;               /* 临时下标 */
	WordList * wlp;         /* 临时单词结点 */
	clock_t s, e;           /* 记录程序运行时间 */
    int ch;

	s = clock();
	fin = fopen(FILE_NAME, "r");

   /*  while (fscanf(fin, "%s", word) != EOF) { */
        /*  */
        /* [> 过滤，将大写转成小写，去掉非字母字符 <] */
        /* char * alph = word; */
        /* for ( ; *alph != '\0'; ++alph) { */
            /* if (! isalpha(*alph)) { */
                /* *alph = '\0'; */
            /* } else { */
                /* *alph = tolower(*alph); */
            /* } */
        /* } */
    
    i = 0;
 	while ((ch = getc(fin)) != EOF) {
        
        /* 过滤，将大写转成小写，去掉非字母字符 */
        if (! isalpha(ch)) {
            word[i++] = '\0';
            i = 0;
            if (word[0] == '\0') {
                continue;
            }
        } else {
            word[i++] = tolower(ch);
            continue;
        }
        
        /* 求出散列值 */
		hash_value = hash(word, TABLE_SIZE);

        /* 向哈希表中加入单词 */
		wlp = &word_table[hash_value];
		while (wlp->next != NULL) {
			if (strcmp(wlp->next->word, word) == 0) {
				++wlp->next->num;
				break;
			}
			wlp = wlp->next;
		}
		if (wlp->next == NULL) {
            /* wlp->next = (WordList *)malloc(sizeof(WordList)); */
            wlp->next = &words[witer++];
			wlp->next->num = 1;
			wlp->next->next = NULL;
			strcpy(wlp->next->word, word);
		}
	}

	e = clock();

    /* 输出结果 */
	for (i = 0; i < TABLE_SIZE; ++i) {
		wlp = word_table[i].next;
		while (wlp != NULL) {
			printf("%lu\t%s\n", wlp->num, wlp->word);
			wlp = wlp->next;
		}
	}

    /* 释放内存 */
/*     for (i = 0; i < TABLE_SIZE; ++i) { */
        /* wlp = word_table[i].next; */
        /* while (wlp != NULL) { */
            /* WordList * tmp = wlp; */
            /* wlp = wlp->next; */
            /* free(tmp); */
        /* } */
/*     } */

	printf("%f\n", (double)(e - s) / CLOCKS_PER_SEC);
	return 0;
}
