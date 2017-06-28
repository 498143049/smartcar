/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_it.c
 * @brief      山外K60 平台中断服务函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */

#include    "MK60_it.h"
#include    "include.h"
#include    "Extern.h"
   
uint16 Wait_Time=0;
extern int speed;  

void  PIT0_IRQHandler(void)  
{
   cartime ++;
   if(cartime > 4000)
     cartime = 4000;
   PIT_Flag_Clear(PIT0);  
}  
 
void PIT2_IRQHandler(void)
{
  if(1==Blackline_flag)
  {
   Wait_Time++;
   if(Wait_Time>=1500)
   {
    Wait_Time=0;
    Blackline_flag=0;
   }
  }
  if((Wait_Time>0)&&(Wait_Time<1500))
  {
    if( ( 1==RED1 || 1==RED2 )&& ( 1==RED4 || 1==RED3 ) )
    {
      Car_Stop_flag=1;
    }
  }
   PIT_Flag_Clear(PIT2); 
}
   

/******************************************************
* @author :沙艺已觉
* @function name : PORTC_Handler()
* @ data : 2016/11/15
* @function description : 场中断服务函数
******************************************************/
void PORTC_IRQHandler(void)
{    
    uint8  n = 0;    //引脚号
    uint32 flag = PORTC_ISFR;
    PORTC_ISFR  = ~0;                                   //清中断标志位

    n = 17;                                             //场中断
    if(flag & (1 << n))                                 //PTC10触发中断
    {
        camera_vsync();
    }    
   
   motor.speed = - ftm_quad_get(FTM2);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
   ftm_quad_clean(FTM2);
    
    //鹰眼直接全速采集，不需要行中断
}

void DMA0_IRQHandler()
{
    camera_dma();
}
