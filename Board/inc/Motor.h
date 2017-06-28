/********************************电机驱动**********************************
*
*作者   ： 沙艺已觉
*文件名 ： Motor.h
*描述   ： 电机驱动头文件
*时间   ： 2015/11/2
说明    ：参考let_it_go 源代码  使用山外V5.3库
*
****************************************************************************/
#ifndef   _MOTOR_H_
#define   _MOTOR_H_

#include "include.h"


typedef struct motor_status
{
    int16 speed;                           //当前速度
    int16 speed_set;                       //设定速度
    int16 last_speed_set;                  //上次速度偏差
    float speed_p;                         //速度控制P参数
    float speed_i;                         //速度控制i参数
    int16  speed_current_error;             //速度当前偏差
    int16  speed_last_error;                //速度上次偏差 
    int16  speed_duty_output;              //速度控制输出占空比  
    int16  speed_filter_error[3];          //速度滤波数组
}motor_status;


extern motor_status motor;

/**************************************************************************
功能：    初始化引脚定义
说明:     直接在这里改IO就可以了 
时间：    2015/11/2
*************************************************************************/
//电机前进的FTM通道
#define         GO_FTM     FTM0
#define         GO_CH      FTM_CH1
//电机后退的FTM通道
#define         BACK_FTM      FTM0
#define         BACK_CH       FTM_CH0

//电机驱动频率
#define         MOTOR_FRE       20000

//电机驱动初始化的占空比   
#define INIT_DUTY   0

#define BM1 gpio_get(PTC4)
#define BM2 gpio_get(PTC5)
#define BM3 gpio_get(PTC6)
#define BM4 gpio_get(PTC7)


void Motor_init(void) ;      //电机驱动初始化
void speed_control();        //速度控制











#endif