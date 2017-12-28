#include "myuart.h"
#include "myframe.h"
#include "mylight.h"
#include "light.h"

int color;
int speed;


void sleep(int t)
{
	int i;
	for (i = 0; i < t; ++i) {

	}
}

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


/* 涓柇澶勭悊鍑芥暟 */
void handle()
{
    Frame f;        /* 甯�*/

    /* 灞忚斀閮ㄥ垎涓柇 */
    

    /* 璇诲叆鏁版嵁 */
    recvFrame(&f);

    /* 鏍￠獙 */
    verifyFrame(&f);

    /* 澶勭悊甯у苟鍝嶅簲 */
    dealFrame(&f);

    return;
}


int main()
{

	light_init(RUN_LIGHT_RED, LIGHT_ON);
	light_init(RUN_LIGHT_BLUE, LIGHT_ON);
	light_init(RUN_LIGHT_GREEN, LIGHT_ON);

    for (;;) {
        /* 灏忕伅闂儊 */

        /* 灏嗙伅缃负浜�*/
        setLight(color);

        /* sleep */
        sleep(speed);

        /* 灏嗘墍鏈夌伅缃负鏆�*/
        unsetLight(RED | BLUE | GREEN);

        /* sleep */
        sleep(speed);
    }

    return 0;
}
