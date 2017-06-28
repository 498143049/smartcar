/********************************S-D�������**********************************
*����   �� ɳ���Ѿ�
*�ļ��� �� Servo_Motor.c
*����   �� �������ͷ�ļ�
*ʱ��   �� 2015/10/30
*ʹ��˵���� 
              ��ɫ        GND
              ��ɫ        VCC
              ��ɫ        PWM
����Ƶ�ʣ�   300HZ    
*   
***************************************************************************/
#ifndef  _SERVO_MOTOR_H_
#define  _SERVO_MOTOR_H_

#include "include.h"



typedef struct servos_status
{
    float direction_p;                         //�������P����
    float direction_d;                         //�������D����
    int16 PID ;
    float direction_last_error;                //�����ϴ�ƫ��
    float diretion_current_error;             //����ǰƫ��
    int16 direction_duty_output;                  //����������ռ�ձ�
    
}servos_status;

extern servos_status servos;


//���²���ΪFTM_PRECISON 1000u    ]��ռ�ձȾ���Ϊ10000ʱ����
#define   Servo_freq    300        //�������Ƶ��Ϊ100HZ
#define   Servo_ftm     FTM1       //FTMģ��
#define   Servo_CH      FTM_CH1        //ͨ�� 
#define   Servo_mid     420    //��ֵ  //  440  470   
#define   Servo_lmax    310       //��С�� 325 355
#define   Servo_rmax    540     //����ܵ� 555 585


void Servo_Motor_init(void);
void direction_control();


#endif  