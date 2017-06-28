#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"      //IO�ڲ���
#include  "MK60_uart.h"      //����
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"     //�͹��Ķ�ʱ��(��ʱ)
#include  "MK60_pit.h"       //PIT
#include  "MK60_FLASH.h"     //FLASH
#include  "MK60_FTM.h"       //FTM
#include  "MK60_sdhc.h"      //SDHC
#include  "MK60_spi.h"       //SPI
#include  "MK60_adc.h"       //ADC
#include  "MK60_dma.h"       //DMA
#include  "MK60_i2c.h"       //I2C
#include  "MK60_it.h"        //isr



#include  "VCAN_LED.H"          //LED
#include  "VCAN_KEY.H"          //KEY
#include  "VCAN_SCCB.h"         //OV����ͷ��������SCCB������
#include  "VCAN_camera.h"       
#include  "VCAN_OV7725_REG.h"   
#include  "VCAN_NRF24L0.h"           
#include  "ff.h"                //FatFs
#include  "io.h"

#include  "FuzzySet_Speed.h"  //ģ���㷨�趨�ٶ�
#include  "OLED.h"           //OLED0.96��LED
#include  "Servo_Motor.h"    //���ͷ�ļ�
#include  "extern.h"         //ȫ�ֱ�������
#include  "Motor.h"          //�������ͷ�ļ�
#include  "System.h"         //ϵͳ��ʼ��
#include  "lcd.h"
#include "Key.h"
#include "Screen.h"
            

#include  "VCAN_computer.h"     //�๦�ܵ�������


#define  RED1 PTB0_IN
#define  RED2 PTB1_IN
#define  RED3 PTB2_IN
#define  RED4 PTB3_IN


#endif  //__INCLUDE_H__
