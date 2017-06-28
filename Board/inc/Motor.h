/********************************�������**********************************
*
*����   �� ɳ���Ѿ�
*�ļ��� �� Motor.h
*����   �� �������ͷ�ļ�
*ʱ��   �� 2015/11/2
˵��    ���ο�let_it_go Դ����  ʹ��ɽ��V5.3��
*
****************************************************************************/
#ifndef   _MOTOR_H_
#define   _MOTOR_H_

#include "include.h"


typedef struct motor_status
{
    int16 speed;                           //��ǰ�ٶ�
    int16 speed_set;                       //�趨�ٶ�
    int16 last_speed_set;                  //�ϴ��ٶ�ƫ��
    float speed_p;                         //�ٶȿ���P����
    float speed_i;                         //�ٶȿ���i����
    int16  speed_current_error;             //�ٶȵ�ǰƫ��
    int16  speed_last_error;                //�ٶ��ϴ�ƫ�� 
    int16  speed_duty_output;              //�ٶȿ������ռ�ձ�  
    int16  speed_filter_error[3];          //�ٶ��˲�����
}motor_status;


extern motor_status motor;

/**************************************************************************
���ܣ�    ��ʼ�����Ŷ���
˵��:     ֱ���������IO�Ϳ����� 
ʱ�䣺    2015/11/2
*************************************************************************/
//���ǰ����FTMͨ��
#define         GO_FTM     FTM0
#define         GO_CH      FTM_CH1
//������˵�FTMͨ��
#define         BACK_FTM      FTM0
#define         BACK_CH       FTM_CH0

//�������Ƶ��
#define         MOTOR_FRE       20000

//���������ʼ����ռ�ձ�   
#define INIT_DUTY   0

#define BM1 gpio_get(PTC4)
#define BM2 gpio_get(PTC5)
#define BM3 gpio_get(PTC6)
#define BM4 gpio_get(PTC7)


void Motor_init(void) ;      //���������ʼ��
void speed_control();        //�ٶȿ���











#endif