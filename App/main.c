/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,ɽ��Ƽ�
*     All rights reserved.
*     �������ۣ�ɽ����̳ http://www.vcan123.com
*
*     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
*     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
*
* @file       main.c
* @brief      ɽ��K60 ƽ̨������
* @author     ɽ��Ƽ�
* @version    v5.3
* @date       2015-04-07
*/

#include "include.h"
#include "define.h"  

extern uint8 s;
int speed;
void  main(void)
{   
  System_init(); 
  // servos.direction_Threshold = 2 ;
  servos.direction_p = 0.4;  //0.4(100) 0.37(85)
  servos.direction_d = 1.4;  //2.0(100) 2.1(85)
  speed = 300;
  
  motor.speed_set =110;
  
  motor.speed_p =9.7 ;//9.7  11
  motor.speed_i =0.4;  //0.4  0.3
  motor.speed_filter_error[0] = 0;
  motor.speed_filter_error[1] = 0;
  motor.speed_filter_error[2] = 0;
  
  while(1)
  {   
    //��ȡͼ��
    camera_get_img(); 
    
    if(img_switch_flag != 0)
    {
      img_extract(img_handle, img_buffer,CAMERA_SIZE); 
      if(img_save_flag == 1)
        img_sd_save(img_buffer);           
    }
    else
    {
      img_extract(img_handle, img_buffer2,CAMERA_SIZE);                     
    }   
    
    Key_Test();
    ScreenShow();
    Get_Edge();    //3.9ms 
    direction_control();
    speed_control();
    
    while(ov7725_eagle_img_flag != IMG_FINISH)           //�ȴ�ͼ��ɼ����
    {
      if(ov7725_eagle_img_flag == IMG_FAIL)            //����ͼ��ɼ����������¿�ʼ�ɼ�
      {
        ov7725_eagle_img_flag = IMG_START;           //��ʼ�ɼ�ͼ��
        PORTC_ISFR = ~0;                             //д1���жϱ�־λ(����ģ���Ȼ�ص���һ���жϾ����ϴ����ж�)
        enable_irq(PORTC_IRQn);                      //����PTA���ж�
      }
    } 
  }  
}


