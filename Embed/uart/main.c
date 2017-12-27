#include "uart.h"
#include "frame.h"


int color;
int speed;



void dealFrame(const Frame * f)
{
    switch (f->type) {
        case F_TYPE_ERROR:
            break;
        case F_TYPE_CONTROL:
            break;
        case F_TYPE_DATA:
            break;
        case F_TYPE_RESPONSE:
            break;
        default:
            break;
    }
}


/* 中断处理函数 */
void handle()
{
    Frame f;        /* 帧 */

    /* 屏蔽部分中断 */
    

    /* 读入数据 */
    recvFrame(&f);

    /* 校验 */
    verifyFrame(&f);

    /* 处理帧并响应 */
    dealFrame(&f);

    return;
}


int main()
{

    for (;;) {
        /* 小灯闪烁 */ 

        /* 将灯置为亮 */
        setLight(color);

        /* sleep */
        sleep(speed);

        /* 将所有灯置为暗 */
        unsetLight(RED | BLUE | GREEN);

        /* sleep */
        sleep(speed);
    }

    return 0;
}
