
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "rio.h"


void rio_init(rio_t * rio, int fd)
{
    rio->fd = fd;
    rio->ptr = rio->buf;
    rio->cnt = 0;
}

int rio_read(rio_t * rio, void * buf, size_t len)
{
    size_t n, size;

    if (rio->cnt >= len) {
    
        memcpy(buf, rio->ptr, len);
        rio->cnt -= len;
        rio->ptr += len;
        return len;
    
    } else {

        memcpy(buf, rio->ptr, rio->cnt);
        size = rio->cnt;

        if ((n = read(rio->fd, rio->buf, RIO_BUFSIZE)) > 0) {

            rio->cnt = n;
            rio->ptr = rio->buf;
            return size + rio_read(rio, buf + size, len - size);

        } else if (n == 0) {

            return size;

        } else {
            
            fprintf(stderr, "read error\n");
            exit(1);

        }

    }
}

int rio_readline(rio_t * rio, void * buf, size_t len)
{
    char ch;
    size_t i = 0;

    while (rio_read(rio, &ch, 1) == 1) {
        
        if (i < len - 1) {
            *((char *)buf + i) = ch;
        } else {
            break;
        } 

        ++i;

        if (ch == '\n') {
            break;
        }
    }
    *((char *)buf + i) = '\0';

    return i;
}
