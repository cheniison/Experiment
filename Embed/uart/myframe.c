
#include "myframe.h"
#include "myuart.h"
#include <stdlib.h>

void recvFrame(Frame * f)
{
    uint_8 ch;
    uint_8 i;
    uint_8 psw = 0;

    /* 读入头部 */
    while((ch = readByte()) != HEADER[0])
        ;

    ch = readByte();

    /* 读入类型 */
    ch = readByte();
    f->type = ch;
    psw ^= ch;

    /* 读入长度 */
    ch = readByte();
    f->len = ch;
    psw ^= ch;

    f->data = malloc(f->len * sizeof(uint_8));

    /* 读入数据 */
    for (i = 0; i < f->len; ++i) {
        ch = readByte();
        f->data[i] = ch;
        psw ^= ch;
    }

    /* 读入校验 */
    ch = readByte();
    f->psw = ch;
    
    return;
}


void verifyFrame(Frame * f)
{
    uint_8 tmp = 0;
    uint_8 i;

    tmp ^= f->type;
    tmp ^= f->len;

    for (i = 0; i < f->len; ++i) {
        tmp ^= f->data[i];
    }

    tmp ^= f->psw;

    if (tmp != 0) {
        f->type = F_TYPE_ERROR;
    }

    return;
}
