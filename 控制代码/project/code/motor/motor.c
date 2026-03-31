#include "fsl_device_registers.h"       // Device header
#include "zf_common_headfile.h"
#include "math.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "spi.h"

//-----------------------------------------------------------------------------------------------
// 函数简介  初始化四路电机
// 参数说明  void
// 返回参数  void
// 使用示例  
// 备注信息  
//-----------------------------------------------------------------------------------------------	
void my_motor_init(void)
{
    gpio_init(MOTORLB_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTORLB_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0
    
    gpio_init(MOTORRB_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTORRB_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(MOTORLA_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTORLA_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(MOTORRA_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTORRA_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

}

//void my_motor_test(void)
//{
//			uint8 duty=10;
//        if(duty >= 0)                                                           // 正转
//        {
//            gpio_set_level(MOTORLB_DIR, GPIO_LOW);                                         // DIR输出高电平
//            pwm_set_duty(MOTORLB_PWM, duty * (PWM_DUTY_MAX / 100));                   // 计算占空比

//            gpio_set_level(MOTORRB_DIR, GPIO_HIGH);                                         // DIR输出高电平
//            pwm_set_duty(MOTORRB_PWM, duty * (PWM_DUTY_MAX / 100));                   // 计算占空比

//            gpio_set_level(MOTORLA_DIR, GPIO_LOW);                                         // DIR输出高电平
//            pwm_set_duty(MOTORLA_PWM, duty * (PWM_DUTY_MAX / 100));                   // 计算占空比

//            gpio_set_level(MOTORRA_DIR, GPIO_HIGH);                                         // DIR输出高电平
//            pwm_set_duty(MOTORRA_PWM, duty * (PWM_DUTY_MAX / 100));                   // 计算占空比
//        }
//        else                                                                    // 反转
//        {
//            gpio_set_level(MOTORLB_DIR, GPIO_HIGH);                                          // DIR输出低电平
//            pwm_set_duty(MOTORLB_PWM, (-duty) * (PWM_DUTY_MAX / 100));                // 计算占空比
//            
//            gpio_set_level(MOTORRB_DIR, GPIO_LOW);                                          // DIR输出低电平
//            pwm_set_duty(MOTORRB_PWM, (-duty) * (PWM_DUTY_MAX / 100));                // 计算占空比
//            
//            gpio_set_level(MOTORLA_DIR, GPIO_HIGH);                                          // DIR输出低电平
//            pwm_set_duty(MOTORLA_PWM, (-duty) * (PWM_DUTY_MAX / 100));                // 计算占空比
//            
//            gpio_set_level(MOTORRA_DIR, GPIO_LOW);                                          // DIR输出低电平
//            pwm_set_duty(MOTORRA_PWM, (-duty) * (PWM_DUTY_MAX / 100));                // 计算占空比

//        }
//}


//-----------------------------------------------------------------------------------------------
// 函数简介  设置指定电机的转速和转向
// 参数说明  my_motor_type: 电机编号(1-4) 
// 参数说明	 my_motor_duty: 占空比(-100~100)，正为正转，负为反转
// 返回参数  void
// 使用示例  
// 备注信息  LB,LA 低电平正转 RB,RA高电平正转
// 备注信息  要加限幅，不然占空比给的太高，超过PWM_DUTY_MAX，会导致spi无法正常打印数据
//-----------------------------------------------------------------------------------------------	

void my_motor_speed(int16_t my_motor_type,int16_t my_motor_duty)
{
	my_motor_duty=func_limit(my_motor_duty,MAX_DUTY);//限幅
	
        if(my_motor_duty >= 0)                                                    // 正转
        {
					switch(my_motor_type)
					{
						case 1:
							gpio_set_level(MOTORLB_DIR, GPIO_LOW);                                         // DIR输出低电平
							pwm_set_duty(MOTORLB_PWM, my_motor_duty);                   // 计算占空比
						break;
						
						case 2:
							gpio_set_level(MOTORRB_DIR, GPIO_HIGH);                                         // DIR输出高电平
							pwm_set_duty(MOTORRB_PWM, my_motor_duty);                   // 计算占空比
						break;
						
						case 3:
							gpio_set_level(MOTORLA_DIR, GPIO_LOW);                                         // DIR输出低电平
							pwm_set_duty(MOTORLA_PWM, my_motor_duty);                   // 计算占空比
						break;
						
						case 4:
							gpio_set_level(MOTORRA_DIR, GPIO_HIGH);                                         // DIR输出高电平
							pwm_set_duty(MOTORRA_PWM, my_motor_duty);                   // 计算占空比
						break;
					}
				}
        else                                                           // 反转
        {
					switch(my_motor_type)
					{
						case 1:
							gpio_set_level(MOTORLB_DIR, GPIO_HIGH);                                          // DIR输出高电平
							pwm_set_duty(MOTORLB_PWM, (-my_motor_duty));                // 计算占空比
						break;
						
						case 2:
							gpio_set_level(MOTORRB_DIR, GPIO_LOW);                                          // DIR输出低电平
							pwm_set_duty(MOTORRB_PWM, (-my_motor_duty));                // 计算占空比
						break;
						
						case 3:
							gpio_set_level(MOTORLA_DIR, GPIO_HIGH);                                          // DIR输出高电平
							pwm_set_duty(MOTORLA_PWM, (-my_motor_duty));                // 计算占空比
						break;
						
						case 4:
							gpio_set_level(MOTORRA_DIR, GPIO_LOW);                                          // DIR输出低电平
							pwm_set_duty(MOTORRA_PWM, (-my_motor_duty));                // 计算占空比
						break;
					}
        }
        
//     system_delay_ms(10);
    }
	
int16_t v1=0,v2=0,v3=0,v4=0;
int16_t speed1=0,speed2=0,speed3=0,speed4=0;
		
//-----------------------------------------------------------------------------------------------
// 函数简介  麦克纳姆轮解算
// 参数说明  v_x: x方向的速度
// 参数说明  v_y: y方向的速度
// 参数说明  w:   车身角速度
// 返回参数  void
// 使用示例  
// 备注信息  将v_x, v_y,w这三个参数传入即可解算出四个轮子的转速
//-----------------------------------------------------------------------------------------------	

void my_car_posture(int16_t V_X, int16_t V_Y, int16_t w)
{
	const float Lx = 0.089;  // 轮子中心到车体中心的X方向距离（单位：米）
	const float Ly = 0.1; // 轮子中心到车体中心的Y方向距离
	const float L = Lx + Ly;   // 合成距离，用于旋转分量计算
	
	v1 = V_X - V_Y - w * L;  // FR轮
	v2 = V_X + V_Y + w * L;  // FL轮
	v3 = V_X + V_Y - w * L;  // RL轮
	v4 = V_X - V_Y + w * L;  // RR轮
//	printdata_2(v1,v2);
}

float actual_location;
float actual_angle;

void car_test()
{
	float speed_x=0,speed_y=0;
	
//	static float x=1;
//	float speed=100*sinf(x)+200;
//	x+=0.1;
	float move_speed=0; 
	int16_t move_angle;
	move_angle=angle_pid_set(0,angle_pid_k);
	move_speed=location_pid_set(100,encoder_data_1,location_pid_k);
	
	speed_x = move_speed * cosf(ANGLE_TO_RAD(move_angle));
	speed_y = move_speed * sinf(ANGLE_TO_RAD(move_angle));
	
//	printdata_2(0,move_angle);
	
	my_car_posture(speed_x, speed_y,move_angle);
	
	speed1=speed_pid_set_1(v1,encoder_data_1,speed_pid_k1);
	speed2=speed_pid_set_2(v2,encoder_data_2,speed_pid_k2);
	speed3=speed_pid_set_3(v3,encoder_data_3,speed_pid_k3);
	speed4=speed_pid_set_4(v4,encoder_data_4,speed_pid_k4);

	speed_set(1,v1,speed1);
	speed_set(2,v2,speed2);
	speed_set(3,v3,speed3);
	speed_set(4,v4,speed4);
	
//printdata_4(v1,v2,v3,v4);
//	printdata_5(encoder_data_1,encoder_data_2,encoder_data_3,encoder_data_4,200);
}

void speed_set(int16_t my_motor_type,int16_t v,int16_t speed)
{
	switch(my_motor_type)
	{
	case 1:
			if(v>0)
		{
			my_motor_speed(1,speed+800);//640
		}
		else if(v==0)
		{

			my_motor_speed(1,0);
		}
		else if(v<0)
		{
			my_motor_speed(1,speed-800);
		}
		break;
		
	case 2:
			if(v>0)
		{
				my_motor_speed(2,speed+800);//500
		}
		else if(v==0)
		{
			my_motor_speed(2,0);
		}
		else if(v<0)
		{
			my_motor_speed(2,speed-800);
		}
			break;
			
	case 3:
			if(v>0)
		{
				my_motor_speed(3,speed+800);
		}
		else if(v==0)
		{
			my_motor_speed(3,0);
		}
		else if(v<0)
		{
			my_motor_speed(3,speed-800);
		}
			break;
		
	case 4:
			if(v>0)
		{
				my_motor_speed(4,speed+800);
		}
		else if(v==0)
		{
			my_motor_speed(4,0);
		}
		else if(v<0)
		{
			my_motor_speed(4,speed-800);
		}
			break;
		}
}

//-----------------------------------------------------------------------------------------------
// 函数简介  全向运动控制
// 参数说明  target_move_speed: 目标移动速度 
// 参数说明  target_move_angle: 目标移动角度
// 返回参数  void
// 使用示例  
// 备注信息  
//-----------------------------------------------------------------------------------------------	

void all_round_contorl(int16_t target_move_speed, float target_move_angle)
{
	float move_speed_y = 0, move_speed_x = 0,move_speed=0,move_angle=0;

//	move_speed=location_pid_set(10,19,location_pid_k);
	move_angle=angle_pid_set(target_move_angle,angle_pid_k);
	
	move_speed_x = move_speed * cosf(ANGLE_TO_RAD(move_angle));
	move_speed_y = move_speed * sinf(ANGLE_TO_RAD(move_angle));
	
//	printdata_2(move_speed_x,move_speed_y);
	
	my_car_posture(move_speed_x, move_speed_y,0);
	
	speed1=speed_pid_set_1(v1,encoder_data_1,speed_pid_k1);
	speed2=speed_pid_set_2(v2,encoder_data_2,speed_pid_k2);
	speed3=speed_pid_set_3(v3,encoder_data_3,speed_pid_k3);
	speed4=speed_pid_set_4(v4,encoder_data_4,speed_pid_k4);
	
//		printdata_2(speed1,speed2);
	
	my_motor_speed(1,speed1);
	my_motor_speed(2,speed2);
	my_motor_speed(3,speed3);
	my_motor_speed(4,speed4);
}
















