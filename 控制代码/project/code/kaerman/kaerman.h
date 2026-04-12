#ifndef KAERMAN_H
#define KAERMAN_H


typedef struct {
    // ¿¨¶ûÂüºËÐÄ¾ØÕó
    float X;
    float P;
		float Q;
    float R;      // ¹Û²âÔëÉù
		float dt;      
}Kalman_TypeDef;

extern   Kalman_TypeDef kf1;
extern   Kalman_TypeDef kf2;
extern   Kalman_TypeDef kf3;
extern   Kalman_TypeDef kf4;
extern   Kalman_TypeDef kf5;

void Kalman_Init( float dt,Kalman_TypeDef * kf);
void Kalman_Update(Kalman_TypeDef *kf,float measurement);
float  Kalman_Get(Kalman_TypeDef *kf,float data);


#endif
