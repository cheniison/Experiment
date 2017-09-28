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


typedef struct s_trie {
	struct s_trie * children[26];
	size_t num;					/* 词频 */
} Trie;

Trie words[MAX_WORDS * 2];      /* 预先分配的空间（减少使用 malloc 系统调用） */
size_t witer = 0;               /* 预先分配的空间最后一个元素的后一位下标 */
Trie word_tree;					/* 字典树 */
Trie * cur;						/* 字典树当前结点 */

void print(Trie * cur, char * word, size_t i)
{
	if (cur == NULL) return;

	if (cur->num != 0) {
		word[i] = '\0';
		printf("%lu\t%s\n", cur->num, word);
	}

	for (size_t j = 0; j < 26; ++j) {
		word[i] = j + 'a';
		print(cur->children[j], word, i + 1);
	}
}


int main(int argc, char **argv)
{
	int fin;             	/* 文件描述符 */
	char word[MAX_LEN];     /* 存储一个单词 */
	char * article;
    size_t index, len;
	struct stat fstatus;
	clock_t s, e;           /* 记录程序运行时间 */
    int ch;


	s = clock();
	fin = open(FILE_NAME, O_RDONLY);

	if (fstat(fin, &fstatus) < 0) {
		return 1;
	}

    len = fstatus.st_size;
	/* 使用存储映射 */
	article = (char *)mmap(0, len, PROT_READ, MAP_PRIVATE, fin, 0);

	cur = &word_tree;
 	for (index = 0; index <= len; ++index) {
        ch = article[index];
		
        /* if (! isalpha(ch)) { */
		if (! ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))) {
			if (&word_tree != cur) {
				++cur->num;
		   		cur = &word_tree;
			}
        } else {
			/* ch = tolower(ch); */
			if (ch <= 'Z') {
				ch +=  'a' - 'A';
			} 
			
			if (cur->children[ch - 'a'] == NULL) {
				cur->children[ch - 'a'] = words + witer++;
			}
			
			cur = cur->children[ch - 'a'];
        }
	}

	/* 解除映射区 */
	munmap(article, len);

	e = clock();

	/* 输出结果 */
	print(&word_tree, word, 0);

	printf("cpu时间（不含输出的时间）：%f\n", (double)(e - s) / CLOCKS_PER_SEC);
	return 0;
}
