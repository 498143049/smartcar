/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_camera.h
 * @brief      摄像头函数接口重定向
 * @author     山外科技
 * @version    v5.2.1
 * @date       2015-04-01
 */


#ifndef _VCAN_CAMERA_H_
#define _VCAN_CAMERA_H_


#define CAMERA_OV7725_EAGLE         2       //山外鹰眼
#define CAMERA_OV7725_WOLF          3       //山外狼眼


#define USE_CAMERA      CAMERA_OV7725_EAGLE   //选择使用的 摄像头

typedef struct
{
    uint8 addr;                  /*寄存器地址*/
    uint8 val;                   /*寄存器值*/
} reg_s;

//定义图像采集状态
typedef enum
{
    IMG_NOTINIT = 0,
    IMG_FINISH,             //图像采集完毕
    IMG_FAIL,               //图像采集失败(采集行数少了)
    IMG_GATHER,             //图像采集中
    IMG_START,              //开始采集图像
    IMG_STOP,               //禁止图像采集
} IMG_STATUS_e;




typedef struct camera_status
{
     float error;     //偏差
     float last_error;//上次偏差
     float curvature; //曲率
     float speed_control_error;   //速度控制偏差    
}camer_status;

extern camer_status camer;


#define VARIANCE_THRESHOLD 4    //跳变点方差阈值
#define LINE_STARTING_POINT  59   //寻线行起始点
#define L_LINE_INITIAL_VALUE  0  //左线初始值
#define R_LINE_INITIAL_VALUE  159  //右线初始值
#define M_LINE_INITIAL_VALUE  80 //中线初始值
#define L_LINE_START  100      //左边线寻线起点
#define R_LINE_START  60     //右边线寻线起点
#define Data_Line 120
#define Data_Count 160


#include  "VCAN_SCCB.h"
#include  "VCAN_OV7725_Eagle.h"

void speed_control_error();
void fill_line(float *line,int x1,int x2,float y1,float y2);
float calculation_abs(float x);
float max_error_point(float x, float y,float z,float average_value);
float calculation_average(float a,float b, float c);
float calculation_variance(float a,float b, float c,float avearge);
void error_requirements_calculation();
void camer_error_calculation();
void least_square_method(int8 *line,int8 raw_start,int8 raw_end);
void servo_err_get();
void ImgFilter(int16 *line);
void Imgfilter_Mline(float *line);
void ImgFilter_range(int16 start,int16 end,int16 *line);
void matchline(int16 start,int16 end,int16* line);//两点连线
float ComSlope_Range(int16 start,int16 end,float * line);
void crossfit(int16* line,int8 point,uint8 d);//一阶拟合
float Slope_Range(int16 D,float * line);
int16 sum(int16 s,int16 e,int16*line);
int16 MMN(int16 *line,uint8 s,uint8 e,uint8 choose);

extern void img_extract(void *dst, void *src, uint32_t srclen);
extern void img_sd_init(void);
extern void img_sd_save(uint8 * imgadrr);
extern void Get_Edge();






#endif


