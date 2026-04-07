#ifndef PID_H
#define PID_H

#include "zf_common_headfile.h"

#define pwm_pid_max (3000)

typedef struct
{
	float P;
	float I;
	float D;
}speed_pid,location_pid,angle_pid;


extern speed_pid speed_pid_k1;
extern speed_pid speed_pid_k2;
extern speed_pid speed_pid_k3;
extern speed_pid speed_pid_k4;
extern location_pid location_pid_k;
extern angle_pid angle_pid_k;

void pid_init(speed_pid * speed_pid_k1,speed_pid*speed_pid_k2,speed_pid*speed_pid_k3,speed_pid*speed_pid_k4,location_pid * location_pid_k,angle_pid * angle_pid_k);
int16_t speed_pid_set_1(int16_t target_speed,int16_t encoder,speed_pid k);
int16_t speed_pid_set_2(int16_t target_speed,int16_t encoder,speed_pid k);
int16_t speed_pid_set_3(int16_t target_speed,int16_t encoder,speed_pid k);
int16_t speed_pid_set_4(int16_t target_speed,int16_t encoder,speed_pid k);
int16_t location_pid_set(int16_t target_location,int16_t actual_location,location_pid k);
//int16_t location_pid_set_ybh(float distance,location_pid k);
int16_t angle_pid_set(int16_t target_angle,angle_pid k);

#endif
