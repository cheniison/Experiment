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
	int fin;
	char word[MAX_LEN];
	char * article, * atmp;
	uint64_t hash_value;
	size_t i;
	WordList * wlp;
	struct stat fstatus;
	clock_t s, e;
	int ch;

	s = clock();

	fin = open(FILE_NAME, O_RDONLY);

	if (fstat(fin, &fstatus) < 0) {
		return 1;
	}

	article = (char *)mmap(0, fstatus.st_size, PROT_READ, MAP_PRIVATE, fin, 0);
	atmp = article;

	i = 0;
 	while ((ch = *atmp++) != '\0') {
        
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
		
		hash_value = hash(word, TABLE_SIZE);
		wlp = &word_table[hash_value];
		while (wlp->next != NULL) {
			if (strcmp(wlp->next->word, word) == 0) {
				++wlp->next->num;
				break;
			}
			wlp = wlp->next;
		}
		if (wlp->next == NULL) {
			wlp->next = &words[witer++];
			wlp->next->num = 1;
			wlp->next->next = NULL;
			strcpy(wlp->next->word, word);
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
	munmap(article, fstatus.st_size);

	printf("%f\n", (double)(e - s) / CLOCKS_PER_SEC);
	return 0;
}
