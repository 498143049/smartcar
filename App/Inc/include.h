#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"      //IO口操作
#include  "MK60_uart.h"      //串口
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"     //低功耗定时器(延时)
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
#include  "VCAN_SCCB.h"         //OV摄像头配置总线SCCB函数库
#include  "VCAN_camera.h"       
#include  "VCAN_OV7725_REG.h"   
#include  "VCAN_NRF24L0.h"           
#include  "ff.h"                //FatFs
#include  "io.h"

#include  "FuzzySet_Speed.h"  //模糊算法设定速度
#include  "OLED.h"           //OLED0.96寸LED
#include  "Servo_Motor.h"    //舵机头文件
#include  "extern.h"         //全局变量声明
#include  "Motor.h"          //电机驱动头文件
#include  "System.h"         //系统初始化
#include  "lcd.h"
#include "Key.h"
#include "Screen.h"
            

#include  "VCAN_computer.h"     //多功能调试助手


#define  RED1 PTB0_IN
#define  RED2 PTB1_IN
#define  RED3 PTB2_IN
#define  RED4 PTB3_IN


#endif  //__INCLUDE_H__
