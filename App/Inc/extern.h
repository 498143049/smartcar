/******************************************************************
*
*����   �� ɳ���Ѿ�
*�ļ��� �� extern.h
*����   �� ȫ�ֱ���ͷ�ļ�
*ʱ��   �� 2015/11/2
*
*****************************************************************************/
#ifndef  _EXTERN_H_
#define  _EXTERN_H_



extern uint8 host_flag ;                       //ǰ�󳵱�־ 0��ʾǰ��; 1��ʾ��
extern int8 CarGo ;                          //��λ
extern uint8 beep_flag ;
extern uint8 img_switch_flag;                      //ͼ���ַ�л���־ 
extern uint8 img_buffer[CAMERA_SIZE];              //ͼ�񻺴��ַһ
extern uint8 img_buffer2[CAMERA_SIZE];             //ͼ�񻺴��ַ��
extern uint8 img_handle[CAMERA_H][CAMERA_W];       //����ͼ����
extern int16 Lline[CAMERA_H];   //����������
extern int16 Rline[CAMERA_H];
extern float Mline[CAMERA_H];

extern int16  Track_complexity ;                //�������ӳ̶�
extern int16 Prospect_See ;                    //ǰհ 

extern uint8 PID_Control_flag ;         //�ٶȿ���PID���ƿ�����־
extern uint8 Robust_Control_count;      //³�����Ƽ���
extern uint8 Robust_Control_flag;       //³�����Ʊ�־
extern uint8 Robust_Control_Sub_flag ;  
extern uint8 Car_Stop_flag;             //ͣ����־
extern uint8 Blackline_flag;
extern int16 cartime;

extern uint8 LCDRAM[8][80];     //OLED��ʾ

extern uint8 stop ;                     //��������ͣ����־
extern uint8 oled_show_road_flag  ;      //oled�Ƿ�����־
extern uint8 img_send_flag ;            //ͼ���Ƿ��͵���λ��
extern uint8 img_save_flag ;            //ͼ���Ƿ���sd�������־
extern uint8 send_osc_flag  ;        //�Ƿ��͵�����ʾ������־

extern float voltage ;

extern uint16 servo_stop_duty ;              //ͣ�����ֹͣ���                        








#endif