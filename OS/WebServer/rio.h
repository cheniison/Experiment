
#ifndef RIO_H
#define RIO_H

#include <stddef.h>

#define RIO_BUFSIZE 4096

typedef struct {
    int fd;         /* 描述符 */
    size_t cnt;        /* 有效的长度 */
    char * ptr;     /* 有效的地址 */
    char buf[RIO_BUFSIZE];  /* 缓冲区 */
} rio_t;


/* 初始化 */
void rio_init(rio_t * rio, int fd);

/* 读入若干字节 */
int rio_read(rio_t * rio, void * buf, size_t len);

/* 读入一行 */
int rio_readline(rio_t * rio, void * buf, size_t len);

#endif
