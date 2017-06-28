/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,山外科技
*     All rights reserved.
*     技术讨论：山外论坛 http://www.vcan123.com
*
*     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
*     修改内容时必须保留山外科技的版权声明。
*
* @file       main.c
* @brief      山外K60 平台主程序
* @author     山外科技
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
    //获取图像
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
    
    while(ov7725_eagle_img_flag != IMG_FINISH)           //等待图像采集完毕
    {
      if(ov7725_eagle_img_flag == IMG_FAIL)            //假如图像采集错误，则重新开始采集
      {
        ov7725_eagle_img_flag = IMG_START;           //开始采集图像
        PORTC_ISFR = ~0;                             //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
        enable_irq(PORTC_IRQn);                      //允许PTA的中断
      }
    } 
  }  
}


