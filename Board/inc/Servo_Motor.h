/********************************S-D舵机驱动**********************************
*作者   ： 沙艺已觉
*文件名 ： Servo_Motor.c
*描述   ： 舵机驱动头文件
*时间   ： 2015/10/30
*使用说明： 
              黑色        GND
              红色        VCC
              白色        PWM
驱动频率：   300HZ    
*   
***************************************************************************/
#ifndef  _SERVO_MOTOR_H_
#define  _SERVO_MOTOR_H_

#include "include.h"



typedef struct servos_status
{
    float direction_p;                         //方向控制P参数
    float direction_d;                         //方向控制D参数
    int16 PID ;
    float direction_last_error;                //方向上次偏差
    float diretion_current_error;             //方向当前偏差
    int16 direction_duty_output;                  //方向控制输出占空比
    
}servos_status;

extern servos_status servos;


//以下参数为FTM_PRECISON 1000u    ]即占空比精度为10000时数据
#define   Servo_freq    300        //舵机驱动频率为100HZ
#define   Servo_ftm     FTM1       //FTM模块
#define   Servo_CH      FTM_CH1        //通道 
#define   Servo_mid     420    //中值  //  440  470   
#define   Servo_lmax    310       //最小到 325 355
#define   Servo_rmax    540     //最大能到 555 585


void Servo_Motor_init(void);
void direction_control();


#endif  