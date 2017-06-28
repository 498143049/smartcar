/******************************************************************
*
*作者   ： 沙艺已觉
*文件名 ： extern.h
*描述   ： 全局变量头文件
*时间   ： 2015/11/2
*
*****************************************************************************/
#ifndef  _EXTERN_H_
#define  _EXTERN_H_



extern uint8 host_flag ;                       //前后车标志 0表示前车; 1表示后车
extern int8 CarGo ;                          //档位
extern uint8 beep_flag ;
extern uint8 img_switch_flag;                      //图像地址切换标志 
extern uint8 img_buffer[CAMERA_SIZE];              //图像缓存地址一
extern uint8 img_buffer2[CAMERA_SIZE];             //图像缓存地址二
extern uint8 img_handle[CAMERA_H][CAMERA_W];       //用于图像处理
extern int16 Lline[CAMERA_H];   //左右线数组
extern int16 Rline[CAMERA_H];
extern float Mline[CAMERA_H];

extern int16  Track_complexity ;                //赛道复杂程度
extern int16 Prospect_See ;                    //前瞻 

extern uint8 PID_Control_flag ;         //速度控制PID控制开启标志
extern uint8 Robust_Control_count;      //鲁棒控制计数
extern uint8 Robust_Control_flag;       //鲁棒控制标志
extern uint8 Robust_Control_Sub_flag ;  
extern uint8 Car_Stop_flag;             //停车标志
extern uint8 Blackline_flag;
extern int16 cartime;

extern uint8 LCDRAM[8][80];     //OLED显示

extern uint8 stop ;                     //蓝牙控制停车标志
extern uint8 oled_show_road_flag  ;      //oled是否开启标志
extern uint8 img_send_flag ;            //图像是否发送到上位机
extern uint8 img_save_flag ;            //图像是否用sd卡保存标志
extern uint8 send_osc_flag  ;        //是否发送到虚拟示波器标志

extern float voltage ;

extern uint16 servo_stop_duty ;              //停车舵机停止打角                        








#endif