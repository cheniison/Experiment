
#ifndef FRAME_H
#define FRAME_H

/* 帧头部 AABB */
#define HEADER "\125\273"
/* 帧尾部 EEEF */
#define FOOTER "\167\357"

/* 帧类型 */
/* 错误帧 */
#define F_TYPE_ERROR 0
/* 命令帧 */
#define F_TYPE_CONTROL 1
/* 数据帧 */
#define F_TYPE_DATA 2
/* 反馈帧 */
#define F_TYPE_RESPONSE 3

typedef struct {
    uint8_t type;       /* 帧类型 */
    uint8_t len;        /* 数据长度 */
    uint8_t  * data;    /* 数据 */
    uint8_t psw;        /* 校验 */
} Frame;


void recvFrame(Frame *);

void verifyFrame(Frame *);


#endif
