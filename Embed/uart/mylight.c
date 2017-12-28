
#include "mylight.h"
#include "light.h"

void setLight(int color)
{
    if (color & RED) {
        /* 绾㈢伅浜�*/
    	light_control(RUN_LIGHT_RED, LIGHT_ON);
    }

    if (color & BLUE) {
        /* 钃濈伅浜�*/
    	light_control(RUN_LIGHT_BLUE, LIGHT_ON);
    }

    if (color & GREEN) {
        /* 缁跨伅浜�*/
    	light_control(RUN_LIGHT_GREEN, LIGHT_ON);
    }

}


void unsetLight(int color)
{
    if (color & RED) {
        /* 绾㈢伅浜�*/
    	light_control(RUN_LIGHT_RED, LIGHT_OFF);
    }

    if (color & BLUE) {
        /* 钃濈伅浜�*/
    	light_control(RUN_LIGHT_BLUE, LIGHT_OFF);
    }

    if (color & GREEN) {
        /* 缁跨伅浜�*/
    	light_control(RUN_LIGHT_GREEN, LIGHT_OFF);
    }

}
