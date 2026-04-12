#ifndef IMU660RA_H
#define IMU660RA_H

#define time 0.005;

typedef struct {
    float Xdata;
    float Ydata;
    float Zdata;
} gyro_param_t;

extern gyro_param_t Gyro_Bias, Gyro_Angle, Acc_Angle, Fusion_Angle;
extern float imu660ra_angle;

void imu660_zeroBias();
void my_imu660ra_init();
void pit_handler_imu660ra();
void imu660ra_read_angle();

#endif