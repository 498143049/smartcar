/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_camera.h
 * @brief      ����ͷ�����ӿ��ض���
 * @author     ɽ��Ƽ�
 * @version    v5.2.1
 * @date       2015-04-01
 */


#ifndef _VCAN_CAMERA_H_
#define _VCAN_CAMERA_H_


#define CAMERA_OV7725_EAGLE         2       //ɽ��ӥ��
#define CAMERA_OV7725_WOLF          3       //ɽ������


#define USE_CAMERA      CAMERA_OV7725_EAGLE   //ѡ��ʹ�õ� ����ͷ

typedef struct
{
    uint8 addr;                  /*�Ĵ�����ַ*/
    uint8 val;                   /*�Ĵ���ֵ*/
} reg_s;

//����ͼ��ɼ�״̬
typedef enum
{
    IMG_NOTINIT = 0,
    IMG_FINISH,             //ͼ��ɼ����
    IMG_FAIL,               //ͼ��ɼ�ʧ��(�ɼ���������)
    IMG_GATHER,             //ͼ��ɼ���
    IMG_START,              //��ʼ�ɼ�ͼ��
    IMG_STOP,               //��ֹͼ��ɼ�
} IMG_STATUS_e;




typedef struct camera_status
{
     float error;     //ƫ��
     float last_error;//�ϴ�ƫ��
     float curvature; //����
     float speed_control_error;   //�ٶȿ���ƫ��    
}camer_status;

extern camer_status camer;


#define VARIANCE_THRESHOLD 4    //����㷽����ֵ
#define LINE_STARTING_POINT  59   //Ѱ������ʼ��
#define L_LINE_INITIAL_VALUE  0  //���߳�ʼֵ
#define R_LINE_INITIAL_VALUE  159  //���߳�ʼֵ
#define M_LINE_INITIAL_VALUE  80 //���߳�ʼֵ
#define L_LINE_START  100      //�����Ѱ�����
#define R_LINE_START  60     //�ұ���Ѱ�����
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
void matchline(int16 start,int16 end,int16* line);//��������
float ComSlope_Range(int16 start,int16 end,float * line);
void crossfit(int16* line,int8 point,uint8 d);//һ�����
float Slope_Range(int16 D,float * line);
int16 sum(int16 s,int16 e,int16*line);
int16 MMN(int16 *line,uint8 s,uint8 e,uint8 choose);

extern void img_extract(void *dst, void *src, uint32_t srclen);
extern void img_sd_init(void);
extern void img_sd_save(uint8 * imgadrr);
extern void Get_Edge();






#endif


