/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.c
 * @brief      ɽ��K60 ƽ̨�жϷ�����
 * @author     ɽ��Ƽ�
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
* @author :ɳ���Ѿ�
* @function name : PORTC_Handler()
* @ data : 2016/11/15
* @function description : ���жϷ�����
******************************************************/
void PORTC_IRQHandler(void)
{    
    uint8  n = 0;    //���ź�
    uint32 flag = PORTC_ISFR;
    PORTC_ISFR  = ~0;                                   //���жϱ�־λ

    n = 17;                                             //���ж�
    if(flag & (1 << n))                                 //PTC10�����ж�
    {
        camera_vsync();
    }    
   
   motor.speed = - ftm_quad_get(FTM2);          //��ȡFTM �������� ��������(������ʾ������)
   ftm_quad_clean(FTM2);
    
    //ӥ��ֱ��ȫ�ٲɼ�������Ҫ���ж�
}

void DMA0_IRQHandler()
{
    camera_dma();
}
