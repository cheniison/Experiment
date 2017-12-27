
#include "frame.h"
#include "uart.h"


void recvFrame(Frame * f)
{
    uint8_t ch;
    uint8_t i;
    uint8_t psw = 0;

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

    f->data = malloc(len * sizeof(uint8_t));

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
    uint8_t tmp = 0;
    uint8_t i;

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
