#ifndef _MY_BUZZER_H
#define _MY_BUZZER_H
#include "zf_common_headfile.h"   // 里面已含 zf_driver_gpio.h  zf_driver_pit.h

#define BEEP  (B11)          // 丝印 B11

void buzzer_init(void);     // 上电调用一次
void buzzer_on(void);       // 开始
void buzzer_off(void);      // 立即停
#endif
