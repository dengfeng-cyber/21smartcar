#ifndef  _MY_MOTOR_H
#define  _MY_MOTOR_H

#include "zf_common_headfile.h"

/*------------------------------------------------
											参数
--------------------------------------------------*/

#define PIT_CH_location              (PIT_CH2 )   
#define MAX_DUTY            (3000)                                               // 最大 MAX_DUTY% 占空比
#define MOTORLB_DIR               (C9)
#define MOTORLB_PWM               (PWM2_MODULE1_CHA_C8 )

#define MOTORRB_DIR               (C7 )
#define MOTORRB_PWM               (PWM2_MODULE0_CHA_C6)

#define MOTORLA_DIR               (D2 )
#define MOTORLA_PWM               (PWM2_MODULE3_CHB_D3)

#define MOTORRA_DIR               (C10 )
#define MOTORRA_PWM               (PWM2_MODULE2_CHB_C11)

extern float distance;
extern float angle;
extern float actual_location;
extern float actual_angle;
extern float speed_x;
extern float speed_y;
extern float target_lication_x;
extern float target_lication_y;
extern uint8_t loacation_move[100];
extern int8_t loacation_dir[2];

/*------------------------------------------------
										函数声明
--------------------------------------------------*/

void my_motor_init(void);
void my_motor_test(void);
void my_motor_speed(int16_t my_motor_type,int16_t my_motor_duty);
void my_car_posture(int16_t V_X, int16_t V_Y, int16_t w);
void car_test();
void all_round_contorl(int16_t target_move_speed, float target_move_angle);
void motor_control();
void speed_set(int16_t my_motor_type,int16_t v,int16_t speed);
void pit_handler_location (void);
void set_position();
void set_position_x(int16_t target,int16_t ancutal,int16_t speed);
void set_position_y(int16_t target,int16_t ancutal,int16_t speed);

#endif
