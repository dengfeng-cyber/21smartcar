#ifndef ENCODER_H
#define ENCODER_H

#include "zf_common_headfile.h"

#define PIT_CH_encoder              (PIT_CH0 )                                 // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用

#define ENCODER_1                   (QTIMER1_ENCODER1)
#define ENCODER_1_A                 (QTIMER1_ENCODER1_CH1_C0)
#define ENCODER_1_B                 (QTIMER1_ENCODER1_CH2_C1)

#define ENCODER_2                   (QTIMER1_ENCODER2)
#define ENCODER_2_A                 (QTIMER1_ENCODER2_CH1_C2)
#define ENCODER_2_B                 (QTIMER1_ENCODER2_CH2_C24)

#define ENCODER_3                   (QTIMER2_ENCODER2)
#define ENCODER_3_A                 (QTIMER2_ENCODER2_CH1_C5)
#define ENCODER_3_B                 (QTIMER2_ENCODER2_CH2_C25)

#define ENCODER_4                   (QTIMER2_ENCODER1)
#define ENCODER_4_A                 (QTIMER2_ENCODER1_CH1_C3)
#define ENCODER_4_B                 (QTIMER2_ENCODER1_CH2_C4)



extern int16_t encoder_data_1;
extern int16_t encoder_data_2;
extern int16_t encoder_data_3;
extern int16_t encoder_data_4;
extern int16_t location_x;
extern int16_t location_y;
extern float encoder_speed1;
extern float encoder_speed2;
extern float encoder_speed3;
extern float encoder_speed4;

void my_encoder_init(void);
void pit_handler_encoder(void);
void get_speed(float speed);

#endif