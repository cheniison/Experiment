
#ifndef FRAME_H
#define FRAME_H

#include "common.h"

/* 甯уご閮�AABB */
#define HEADER "\125\273"
/* 甯у熬閮�EEEF */
#define FOOTER "\167\357"

/* 甯х被鍨�*/
/* 閿欒甯�*/
#define F_TYPE_ERROR 0
/* 鍛戒护甯�*/
#define F_TYPE_CONTROL 1
/* 鏁版嵁甯�*/
#define F_TYPE_DATA 2
/* 鍙嶉甯�*/
#define F_TYPE_RESPONSE 3

typedef struct {
    uint_8 type;       /* 甯х被鍨�*/
    uint_8 len;        /* 鏁版嵁闀垮害 */
    uint_8  * data;    /* 鏁版嵁 */
    uint_8 psw;        /* 鏍￠獙 */
} Frame;


void recvFrame(Frame *);

void verifyFrame(Frame *);


#endif
