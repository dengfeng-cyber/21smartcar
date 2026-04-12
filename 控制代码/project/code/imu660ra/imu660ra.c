#include "zf_common_headfile.h"
#include "math.h"
#include "imu660ra.h"
#include "spi.h"
#include "kaerman.h"


#define isZero_Bias		1	//是否开启零漂，0-关闭， 1-开启
//float imu660ra_angle=0;
gyro_param_t Gyro_Bias;    //陀螺仪零飘
gyro_param_t Gyro_Angle;   //陀螺仪实时角度
gyro_param_t Acc_Angle;
gyro_param_t Fusion_Angle;	//数据融合后角度
float tra_acc_x, tra_acc_y, tra_acc_z, tra_gyro_x,tra_gyro_y, tra_gyro_z;//转换为度每秒后的角速度
float K = 0.1f;	//互补滤波系数

void imu660_zeroBias()//陀螺仪零漂
{
#if(isZero_Bias)
	if(isZero_Bias)
	{
		Gyro_Bias.Zdata = 0;//初始化
		for (uint16_t i = 0; i < 500; i++)
		{
			imu660ra_get_gyro();//获取角速度
			Gyro_Bias.Zdata += (imu660ra_gyro_transition(imu660ra_gyro_z)*0.005);//累加陀螺仪
			system_delay_ms(5);
		}		
	}
	Gyro_Bias.Zdata /= 500;//取平均数,即零漂值
#endif	

}
//-----------------------------------------------------------------------------------------------
// 函数简介  IMU660RA传感器初始化
// 参数说明  void: 
// 返回参数  void
// 使用示例  my_imu660ra_init
// 备注信息  初始化IMU硬件+角度/零漂结构体参数，为后续角度解算做准备
//-----------------------------------------------------------------------------------------------

void my_imu660ra_init()
{
//	imu660ra_init();
//	
//	imu660ra_gyro_z=0;
	imu660ra_init();
	Gyro_Angle.Zdata = 0;
	Gyro_Angle.Ydata = 0;
	Gyro_Bias.Zdata = 0;
	Acc_Angle.Xdata = 0;
	Acc_Angle.Ydata = 0;
	Acc_Angle.Zdata = 0;
	Fusion_Angle.Ydata = 0;
}

//-----------------------------------------------------------------------------------------------
// 函数简介  获取角度
// 参数说明  void
// 返回参数  void
// 使用示例  
// 备注信息  
//-----------------------------------------------------------------------------
//void imu660ra_read_angle()
//{
//	static float AngleAcc,AngleGyro,Alpha;
//	static int16_t z;
//	
//	z+=imu660ra_gyro_z;
//	z-=0.03;
//	AngleAcc=atan2(imu660ra_acc_y,imu660ra_acc_x)/3.14159*180;
//	AngleGyro=imu660ra_angle+z*time;
//	Alpha=0.001;
//	imu660ra_angle=(Alpha*AngleAcc+(1-Alpha)*AngleGyro)/3.3;
//	printdata_3(AngleAcc,AngleGyro,imu660ra_angle);
//}
//-----------------------------------------------------------------------------------------------
// 函数简介  IMU660RA传感器数据采集
// 参数说明  void
// 返回参数  void
// 使用示例  
// 备注信息  
//-----------------------------------------------------------------------------
void pit_handler_imu660ra()
{
//	imu660ra_get_acc();
//	imu660ra_get_gyro();
//	imu660ra_read_angle();
	static float a=0;
	imu660ra_get_acc();
	imu660ra_get_gyro();      
  tra_gyro_z = imu660ra_gyro_transition(imu660ra_gyro_z);//转换为度每秒后的角速度
	tra_gyro_y = imu660ra_gyro_transition(imu660ra_gyro_y);
	a=tra_gyro_z*0.005-Gyro_Bias.Zdata;
	Gyro_Angle.Zdata += Kalman_Get(&kf5,a);
	Gyro_Angle.Ydata += (tra_gyro_y*0.005-Gyro_Bias.Ydata);
//	printdata_2(tra_gyro_z,Gyro_Angle.Zdata);
}