#include "zf_common_headfile.h"
#include "buzzer.h"

/* --------------- 初始化 --------------- */
void buzzer_init(void)
{
	gpio_init(BEEP, GPO, 0, GPO_PUSH_PULL);
}

/* --------------- 开始 --------------- */
void buzzer_on(void)
{
    gpio_set_level(BEEP, GPIO_HIGH);
}

/* --------------- 停止 --------------- */
void buzzer_off(void)
{
    gpio_set_level(BEEP, GPIO_LOW);
}
