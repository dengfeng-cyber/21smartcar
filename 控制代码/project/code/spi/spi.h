#ifndef SPI_H
#define SPI_H

#include "zf_common_headfile.h"

#define WIFI_SSID_TEST          "ybh"
#define WIFI_PASSWORD_TEST      "han92400" 

//#define WIFI_SSID_TEST          "lzc"
//#define WIFI_PASSWORD_TEST      "LZC20060804" 

 void printdata_8(float data0,float data1,float data2,float data3,float data4,float data5,float data6,float data7);
 void printdata_7(float data0,float data1,float data2,float data3,float data4,float data5,float data6);
 void printdata_6(float data0,float data1,float data2,float data3,float data4,float data5);
 void printdata_5(float data0,float data1,float data2,float data3,float data4);
 void printdata_4(float data0,float data1,float data2,float data3);
 void printdata_3(float data0,float data1,float data2);
 void printdata_2(float data0,float data1);
 void printdata_1(float data0);
 void connect();


#endif