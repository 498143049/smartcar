/**********************************ȫ�ֱ�������****************************
*�ļ�����  define.h
*˵����   ȫ�ֱ�������
*ʱ�䣺    2015/11/21
**************************************************************************/


#ifndef _DEFINE_H_
#define _DEFINE_H_

#include "include.h"


uint8 beep_flag = 1;
uint8 a=0;

int8 CarGo = 0 ;                           //��λ
uint8 img_switch_flag = 0;                  //ͼ���ַ�л���־                        
uint8 img_buffer[CAMERA_SIZE];              //����洢����ͼ�������
uint8 img_buffer2[CAMERA_SIZE];             //����洢����ͼ�����һ����    ��ƹ���㷨ʵ�ֽ���洢��
uint8 img_handle[Data_Line][Data_Count];    //����洢��ѹ��Ҫ�����ͼ�������
int16 Lline[CAMERA_H];   //����������
int16 Rline[CAMERA_H];
float Mline[CAMERA_H];

int16 Track_complexity ;                //�������ӳ̶�
int16 Prospect_See ;                    //ǰհ 

uint8 Blackline_flag=0;
uint8 Car_Stop_flag = 0;               //ͣ����־
int16 cartime=0;


uint16 servo_stop_duty ;              //ͣ�����ֹͣ���
uint8 LCDRAM[8][80];

uint8 stop = 0;                      //����һ��ͣ����־
uint8 oled_show_road_flag = 0 ;       //oled�Ƿ�����־
uint8 img_send_flag = 0 ;            //ͼ���Ƿ��͵���λ��
uint8 img_save_flag = 0 ;            //ͼ���Ƿ���sd�������־
uint8 send_osc_flag = 0 ;             //�Ƿ��͵�����ʾ������־

float var[6];
float voltage = 7.2;                          

extern IMG_STATUS_e  ov7725_eagle_img_flag;   //ͼ��״̬  


#endif