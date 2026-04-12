#include "zf_common_headfile.h"
#include "pid.h"
#include "math.h"
#include "encoder.h"
#include "imu660ra.h"
#include "spi.h"

speed_pid speed_pid_k1,speed_pid_k2,speed_pid_k3,speed_pid_k4;
location_pid location_pid_x_k,location_pid_y_k;
angle_pid angle_pid_k;
//-----------------------------------------------------------------------------------------------
// 函数简介  PID控制器参数初始化
// 参数说明  speed_pid_k: 指向速度环PID参数结构体的指针
// 参数说明  location_pid_k: 指向位置环PID参数结构体的指针
// 参数说明  angle_pid_k: 指向角度环PID参数结构体的指针
// 返回参数  void
// 使用示例  
// 备注信息  
//-----------------------------------------------------------------------------
void pid_init(speed_pid * speed_pid_k1,speed_pid*speed_pid_k2,speed_pid*speed_pid_k3,speed_pid*speed_pid_k4,location_pid * location_pid_x_k,location_pid * location_pid_y_k,angle_pid * angle_pid_k)
{
	//速度环1
	speed_pid_k1->P = -1.5;//-1.5
	speed_pid_k1->I = -0.4;//-0.4
	speed_pid_k1->D = 0;//0
	//速度环2
	speed_pid_k2->P = -1.3;//-0.064;//-0.08
	speed_pid_k2->I = -0.4;//-0.08;//-0.13
	speed_pid_k2->D =0;//0
	//速度环3
	speed_pid_k3->P = -1.3;//-0.1;//-0.09
	speed_pid_k3->I = -0.3;//-0.5;//-0.12
	speed_pid_k3->D =0;
	//速度环4
	speed_pid_k4->P = -1.5;//-0.1;//-0.08
	speed_pid_k4->I = -0.3;//-0.5;//-0.128
	speed_pid_k4->D =0;
	
	
	//位置环
	location_pid_x_k->P = 0.01;//0.01
	location_pid_x_k->I = 0.0001;//0.0001
	location_pid_x_k->D = 0.6;//0.6
	
	location_pid_y_k->P =0.01;//0.01
	location_pid_y_k->I = 0.0001;//0.0001
	location_pid_y_k->D = 0.01;//0.01
	
	//角度环
	angle_pid_k->P =2; //1.9
	angle_pid_k->I = 0;
	angle_pid_k->D = 8;//9
	
}  

//-----------------------------------------------------------------------------------------------
// 函数简介  速度环
// 参数说明  target_speed: 速度目标值
// 参数说明  speed: 速度实际反馈值
// 参数说明  k: 速度环PID参数结构体
// 返回参数  速度环输出的PWM值
// 使用示例  
// 备注信息  采用位置式PID算法，作为控制内环，输出限幅通过func_limit实现，速度环无d，加d增加抖动
//-----------------------------------------------------------------------------
int16_t speed_pid_set_1(int16_t target_speed,int16_t speed,speed_pid k)
{
	static float err1 = 0, last_err1 = 0,integral1=0,pwm_out1=0;
		err1 =  speed - target_speed ;
	integral1 += err1;
	 pwm_out1 = k.P*err1 +k.D*(err1 - last_err1) + k.I*integral1;
	last_err1 = err1;
//	printdata_3(encoder_data_1,pwm_out1,150);
	integral1 = func_limit(integral1,8000); //积分限幅
	return pwm_out1;
}

int16_t speed_pid_set_2(int16_t target_speed,int16_t speed,speed_pid k)
{
	static float err2 = 0, last_err2 = 0,integral2=0,pwm_out2=0;
		err2 = speed - target_speed ;
		integral2 += err2;	
	 pwm_out2 = k.P*err2 +k.D*(err2 - last_err2) + k.I*integral2;
	last_err2 = err2;
//	printdata_3(encoder_data_2,pwm_out2,150);
  integral2 = func_limit(integral2,9000); //积分限幅
	return pwm_out2;
}

int16_t speed_pid_set_3(int16_t target_speed,int16_t speed,speed_pid k)
{
	static float err3 = 0, last_err3 = 0,integral3=0,pwm_out3=0;
		err3 = speed - target_speed ;
		integral3 += err3;
	 pwm_out3 = k.P*err3 +k.D*(err3 - last_err3) + k.I*integral3;
	last_err3 = err3;
//	printdata_3(encoder_data_3,pwm_out3,150);
	integral3 = func_limit(integral3,9000); //积分限幅
	return pwm_out3;
}

int16_t speed_pid_set_4(int16_t target_speed,int16_t speed,speed_pid k)
{
	static float err4 = 0, last_err4 = 0,integral4=0,pwm_out4=0;
		err4 = speed - target_speed ;
	  integral4 += err4;
	 pwm_out4 = k.P*err4 +k.D*(err4 - last_err4) + k.I*integral4;
	last_err4 = err4;
//	printdata_3(encoder_data_4,pwm_out4,150);
	integral4 = func_limit(integral4,9500); //积分限幅
	return pwm_out4;
}



//-----------------------------------------------------------------------------------------------
// 函数简介  位置环
// 参数说明  distance:距离
// 参数说明  k: 位置环PID参数结构体
// 返回参数  位置环输出（作为速度环的目标值）
// 使用示例  
// 备注信息  输出作为速度环的输入，属于控制外环
//-----------------------------------------------------------------------------

int16_t location_pid_set_x(float target_location,int16_t actual_location,location_pid k)
{
	static float err_x = 0, last_err_x = 0,output_location_x=0,integral_x=0;
	err_x = target_location - actual_location;
	integral_x+=err_x;
	output_location_x = (int16_t)(k.P * err_x + k.D * (err_x - last_err_x))+k.I*integral_x;
	last_err_x = err_x;
	integral_x = func_limit(integral_x,2000);
//	printdata_3(target_location,actual_location,output_location_x);
//	if (err_x == func_limit(err_x,20))
//	{
//		output_location_x=0;
//	}
	return output_location_x*0.5;
}

int16_t location_pid_set_y(float target_location,int16_t actual_location,location_pid k)
{
	static float err_y = 0, last_err_y = 0,output_location_y=0,integral_y=0;
	err_y = target_location - actual_location;
	integral_y+=err_y;
	output_location_y = (int16_t)(k.P * err_y + k.D * (err_y - last_err_y))+k.I*integral_y;
	last_err_y = err_y;
	integral_y = func_limit(integral_y,2000);
//		if (err_y == func_limit(err_y,20))
//	{
//		output_location_y=0;
//	}
//	printdata_3(target_location,actual_location,output_location_y);
	return output_location_y*0.5;
}

//-----------------------------------------------------------------------------------------------
// 函数简介  角度环
// 参数说明  target_angle: 目标角度
// 参数说明  k: 角度环PID参数结构体
// 返回参数  角度环输出（用于调整车身旋转速度）
// 使用示例  
// 备注信息  从IMU660RA读取当前角度，处理角度环360°循环误差，输出作为速度环的输入
//-----------------------------------------------------------------------------
int16_t angle_pid_set(int16_t target_angle,angle_pid k)
{
	static float angle_err, last_angle_err = 0,output_angle=0,integral;
	
	angle_err =Gyro_Angle.Zdata - target_angle;
	integral+=angle_err;
//	if(angle_err > 180)angle_err -= 360;
//	if(angle_err < -180)angle_err += 360;
	output_angle = k.P *angle_err + k.D * (angle_err - last_angle_err)+ k.I*integral;
	last_angle_err = angle_err;
	integral = func_limit(integral,2000); //积分限幅
//	printdata_2(Gyro_Angle.Zdata,output_angle);
	return output_angle;
}

//int16_t speed_pid_set_1(int16_t target_speed,int16_t speed,speed_pid k)
//{
//		static float speed_error[3] = {0};
//		static float 	pwm_out=0;
//		
//   	speed_error[0] = target_speed -speed ;	
//		pwm_out += k.P * (speed_error[0] - speed_error[1]) + k.I * speed_error[0] + k.D * (speed_error[0] - 2 * speed_error[1] + speed_error[2]); 
//		speed_error[2] = speed_error[1];  
//		speed_error[1] = speed_error[0];
//		pwm_out = func_limit(pwm_out, pwm_pid_max);
////    printdata_4(encoder_data_1,encoder_data_2,encoder_data_3,target_speed);
//		return pwm_out;
//}

//int16_t speed_pid_set_2(int16_t target_speed,int16_t speed,speed_pid k)
//{
//		static float speed_error[3] = {0};
//		static float 	pwm_out=0;
//		
//   	speed_error[0] = target_speed -speed ;	
//		pwm_out += k.P * (speed_error[0] - speed_error[1]) + k.I * speed_error[0] + k.D * (speed_error[0] - 2 * speed_error[1] + speed_error[2]); 
//		speed_error[2] = speed_error[1];  
//		speed_error[1] = speed_error[0];
//		pwm_out = func_limit(pwm_out, pwm_pid_max);
////    printdata_4(encoder_data_1,encoder_data_2,encoder_data_3,target_speed);
//		return pwm_out;
//}

//int16_t speed_pid_set_3(int16_t target_speed,int16_t speed,speed_pid k)
//{
//		static float speed_error[3] = {0};
//		static float 	pwm_out=0;
//		
//   	speed_error[0] = target_speed -speed ;	
//		pwm_out += k.P * (speed_error[0] - speed_error[1]) + k.I * speed_error[0] + k.D * (speed_error[0] - 2 * speed_error[1] + speed_error[2]); 
//		speed_error[2] = speed_error[1];  
//		speed_error[1] = speed_error[0];
//		pwm_out = func_limit(pwm_out, pwm_pid_max);
////    printdata_4(encoder_data_1,encoder_data_2,encoder_data_3,target_speed);
//		return pwm_out;
//}

//int16_t speed_pid_set_4(int16_t target_speed,int16_t speed,speed_pid k)
//{
//		static float speed_error[3] = {0};
//		static float 	pwm_out=0;
//		
//   	speed_error[0] = target_speed -speed ;	
//		pwm_out += k.P * (speed_error[0] - speed_error[1]) + k.I * speed_error[0] + k.D * (speed_error[0] - 2 * speed_error[1] + speed_error[2]); 
//		speed_error[2] = speed_error[1];  
//		speed_error[1] = speed_error[0];
//		pwm_out = func_limit(pwm_out, pwm_pid_max);
////    printdata_4(encoder_data_1,encoder_data_2,encoder_data_3,target_speed);
			    //积分限幅
//    if(Pwm[motor_num-1]> 8000)Pwm[motor_num-1]= 8000;
//    if(Pwm[motor_num-1]<-8000)Pwm[motor_num-1]=-8000;
//		return pwm_out;
//}


//int16_t location_pid_set_ybh(float distance,location_pid k)
//{
//	static float err = 0, last_err = 0,output_location=0,integral=0,target=0;
//	err = distance; 
//	integral+=err;
//	output_location = (int16_t)(k.P * err +  k.I*integral+k.D * (err - last_err));
//	last_err = err;
//	
////printdata_2(10,output_location);
//	return output_location;
//}