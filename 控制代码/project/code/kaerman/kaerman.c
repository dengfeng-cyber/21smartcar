#include "kaerman.h"
#include "encoder.h"
//卡尔曼只负责输出平滑曲线，滤波作用

 Kalman_TypeDef kf1,kf2,kf3,kf4,kf5;


// 卡尔曼初始化，dt则是多久调用一次
// 曲线更平滑（噪声更小）增加R减小Q；想要跟得上变化减小R增大Q
void Kalman_Init( float dt,Kalman_TypeDef * kf)
{
    kf->X = 0;
    kf->P = 1;
    kf->Q = 0.1;
    kf->R = 3;
		kf->dt=dt;
}

//------------------------------------------------------------------------------------------
// 函数简介
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------
void Kalman_Update(Kalman_TypeDef *kf,float measurement)
{
		kf->P = kf->P + kf->Q;
      // 卡尔曼增益
    float K = kf->P / (kf->P + kf->R);
    // 状态更新
    kf->X = kf->X+ K * (measurement - kf->X);
    // 协方差更新
    kf->P = (1.0f - K) * kf->P;
}
//获得滤波
float  Kalman_Get(Kalman_TypeDef *kf,float data)
{
	Kalman_Update(kf,data);
	
	return kf->X;
}

