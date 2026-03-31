#include "fsl_device_registers.h"       // Device header
#include "zf_common_headfile.h"
#include "encoder.h"
#include "motor.h"
#include "spi.h"

int16_t encoder_data_1 = 0;
int16_t encoder_data_2 = 0;
int16_t encoder_data_3 = 0;
int16_t encoder_data_4 = 0;
int16_t location1=0;
int16_t location2=0;
int16_t location3=0;
int16_t location4=0;

//-----------------------------------------------------------------------------------------------
// 函数简介  初始化四路电机的编码器
// 参数说明  void
// 返回参数  void
// 使用示例  
// 备注信息  
//-----------------------------------------------------------------------------
void my_encoder_init(void)
{
	encoder_quad_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);                     // 初始化编码器模块与引脚 正交解码编码器模式
  encoder_quad_init(ENCODER_2, ENCODER_2_A, ENCODER_2_B);                     // 初始化编码器模块与引脚 正交解码编码器模式
  encoder_quad_init(ENCODER_3, ENCODER_3_A, ENCODER_3_B);                     // 初始化编码器模块与引脚 正交解码编码器模式
  encoder_quad_init(ENCODER_4, ENCODER_4_A, ENCODER_4_B);                     // 初始化编码器模块与引脚 正交解码编码器模式
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 的中断处理函数 这个函数将在 PIT 对应的定时器中断调用 详见 isr.c
// 参数说明     void
// 返回参数     void
// 使用示例     pit_handler(); 
// 备注信息     不能抖车，抖一下编码器方向要反过来
//-------------------------------------------------------------------------------------------------------------------
//void pit_handler_encoder (void)
//{
//    encoder_data_1 = encoder_get_count(ENCODER_1);                              // 获取编码器计数
//    encoder_clear_count(ENCODER_1);                                             // 清空编码器计数

//    encoder_data_2 = -encoder_get_count(ENCODER_2);                              // 获取编码器计数
//    encoder_clear_count(ENCODER_2);                                             // 清空编码器计数
//    
//    encoder_data_3 = encoder_get_count(ENCODER_3);                              // 获取编码器计数
//    encoder_clear_count(ENCODER_3);                                             // 清空编码器计数

//    encoder_data_4 = -encoder_get_count(ENCODER_4);                              // 获取编码器计数
//    encoder_clear_count(ENCODER_4);                                             // 清空编码器计数
//	
//		car_test();
//}

void pit_handler_encoder (void)
{
		static float first_encoder_data_1,last_encoder_data_1,first_encoder_data_2,last_encoder_data_2,first_encoder_data_3,last_encoder_data_3,first_encoder_data_4,last_encoder_data_4;
	
    first_encoder_data_1 = encoder_get_count(ENCODER_1);														// 获取编码器计数
		encoder_data_1=0.3*first_encoder_data_1+0.7*last_encoder_data_1;
    last_encoder_data_1=encoder_data_1;
    encoder_clear_count(ENCODER_1);                                             // 清空编码器计数

    first_encoder_data_2 = -encoder_get_count(ENCODER_2);                              // 获取编码器计数
	  encoder_data_2=0.3*first_encoder_data_2+0.7*last_encoder_data_2;
    last_encoder_data_2=encoder_data_2;
    encoder_clear_count(ENCODER_2);                                             // 清空编码器计数
    
	 
    first_encoder_data_3 = encoder_get_count(ENCODER_3);                              // 获取编码器计数
	  encoder_data_3=0.3*first_encoder_data_3+0.7*last_encoder_data_3;
    last_encoder_data_3=encoder_data_3;
    encoder_clear_count(ENCODER_3);                                             // 清空编码器计数

    first_encoder_data_4 = -encoder_get_count(ENCODER_4);                              // 获取编码器计数
		encoder_data_4=0.3*first_encoder_data_4+0.7*last_encoder_data_4;
    last_encoder_data_4=encoder_data_4;
    encoder_clear_count(ENCODER_4);                                             // 清空编码器计数


  	car_test();
}