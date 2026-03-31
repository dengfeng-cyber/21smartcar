#include "zf_common_headfile.h"
#include "pid.h"
#include "math.h"
#include "encoder.h"
#include "imu660ra.h"
#include "spi.h"


//-----------------------------------------------------------------------------------------------
// 函数简介  初始化WiFi并建立TCP连接
// 参数说明  void
// 返回参数  void
// 使用示例  
// 备注信息  
//-----------------------------------------------------------------------------------------------
void connect()
{
	
 while(wifi_spi_init(WIFI_SSID_TEST, WIFI_PASSWORD_TEST))
    {
		printf("\r\n connect wifi failed. \r\n");
        system_delay_ms(100);                                                   // 初始化失败 等待 100ms
    }
	 if(1 != WIFI_SPI_AUTO_CONNECT)                                              // 如果没有开启自动连接 就需要手动连接目标 IP
    {
        while(wifi_spi_socket_connect(                                          // 向指定目标 IP 的端口建立 TCP 连接
            "TCP",                                                              // 指定使用TCP方式通讯
            WIFI_SPI_TARGET_IP,                                                 // 指定远端的IP地址，填写上位机的IP地址
            WIFI_SPI_TARGET_PORT,                                               // 指定远端的端口号，填写上位机的端口号，通常上位机默认是8080
            WIFI_SPI_LOCAL_PORT))                                               // 指定本机的端口号
        {
            // 如果一直建立失败 考虑一下是不是没有接硬件复位
            printf("\r\n Connect TCP Servers error, try again.");
            system_delay_ms(100);                                               // 建立连接失败 等待 100ms
        }
    }
}  

//-----------------------------------------------------------------------------------------------
// 函数简介  通过WiFi SPI向上位机发送目标值/实际值数据
// 参数说明  void
// 返回参数  target: 目标值  anctual: 实际值
// 使用示例  
// 备注信息  
//-----------------------------------------------------------------------------------------------

 void printdata_8(float data0,float data1,float data2,float data3,float data4,float data5,float data6,float data7)
 {
	  // 逐飞助手初始化 数据传输使用高速WIFI SPI
	  seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIFI_SPI);
	 
	  // 设置本次需要发送几个通道的数据
			seekfree_assistant_oscilloscope_data.channel_num = 8;
			seekfree_assistant_oscilloscope_data.data[0] = data0;
			seekfree_assistant_oscilloscope_data.data[1] = data1;
		 seekfree_assistant_oscilloscope_data.data[2] = data2;
		 seekfree_assistant_oscilloscope_data.data[3] = data3;
		 seekfree_assistant_oscilloscope_data.data[4] = data4;
		 seekfree_assistant_oscilloscope_data.data[5] = data5;
		 seekfree_assistant_oscilloscope_data.data[6] = data6;
		 seekfree_assistant_oscilloscope_data.data[7] = data7;
		
		 seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);  
		 system_delay_ms(20);
	 
 }
	void printdata_7(float data0,float data1,float data2,float data3,float data4,float data5,float data6)
	{
		printdata_8(data0,data1,data2,data3,data4,data5,data6,0);
	}
	
	void printdata_6(float data0,float data1,float data2,float data3,float data4,float data5)
	{
		printdata_8(data0,data1,data2,data3,data4,data5,0,0);
	}
	
	  void printdata_5(float data0,float data1,float data2,float data3,float data4)
	{
		printdata_8(data0,data1,data2,data3,data4,0,0,0);
	}
	
	  void printdata_4(float data0,float data1,float data2,float data3)
	{
		printdata_8(data0,data1,data2,data3,0,0,0,0);
	}
	
	  void printdata_3(float data0,float data1,float data2)
	{
		printdata_8(data0,data1,data2,0,0,0,0,0);
	}
	
	  void printdata_2(float data0,float data1)
	{
		printdata_8(data0,data1,0,0,0,0,0,0);
	}
	
	  void printdata_1(float data0)
	{
		printdata_8(data0,0,0,0,0,0,0,0);
	}