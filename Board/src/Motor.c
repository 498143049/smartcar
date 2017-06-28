/********************************S-D舵机驱动**********************************
*
*作者   ： 沙艺已觉
*文件名 ： Motor.c
*描述   ： 舵机驱动函数
*时间   ： 2015/11/2
*
****************************************************************************/

#include  "Motor.h"

uint8 s=0;
motor_status motor;
/*************************************************************************
沙艺已觉
函数名称：motor_init()
入口参数：无
出口参数：无
修改时间：2015/11/2
功能说明：电机驱动初始化
其他说明：参考let_it_go源代码
************************************************************************/

void Motor_init(void)
{
  ftm_pwm_init(GO_FTM,GO_CH,MOTOR_FRE,0);    
  ftm_pwm_init(BACK_FTM,BACK_CH,MOTOR_FRE,0);     
}

/*************************************************************************
沙艺已觉
函数名称： void speed_control(void)
入口参数：无
出口参数：无
修改时间：2015/11/2
功能说明：
其他说明：参考let_it_go源代码 使用山外V5.3库
************************************************************************/

void speed_control(void)
{    
  //拨码开关调档
  if((BM1==0)&&(BM2==1)&&(BM3==1)&&(BM4==1))
  {
    UFF = UFF1;
  }
  else if((BM1==1)&&(BM2==0)&&(BM3==1)&&(BM4==1))
  {
    UFF = UFF2;
  }
  else if((BM1==1)&&(BM2==1)&&(BM3==0)&&(BM4==1))
  {
    UFF = UFF3;
  }
  else if((BM1==1)&&(BM2==1)&&(BM3==1)&&(BM4==0))
  {
    UFF = UFF4;
  }
  motor.speed_set = FuzzySet_Speed(Track_complexity, Prospect_See);
  motor.speed_filter_error[2] = motor.speed_filter_error[1];
  motor.speed_filter_error[1] = motor.speed_filter_error[0];
  motor.speed_filter_error[0] = motor.speed_set;
  motor.speed_set = (int16)(0.7*motor.speed_filter_error[0]+0.2*motor.speed_filter_error[1]+0.1*motor.speed_filter_error[2]);
  
  //停车检测
  if(1==Car_Stop_flag)
 // if(1==Blackline_flag)
  {
    s++;
    if(s<10)
    {
      gpio_set (PTE0, 1);
    }
    else
    {
      s=10;
      gpio_set (PTE0, 0);
    }
    if(motor.speed<20)
    {
     ftm_pwm_duty(GO_FTM,GO_CH,0);
     ftm_pwm_duty(BACK_FTM,BACK_CH,0);     
    }
    else
    {
      motor.speed_set=0;
      
      motor.speed_last_error = motor.speed_current_error ;        //更新每次的差值
      motor.speed_current_error = motor.speed_set-motor.speed;   //速度当前差值
      
      motor.speed_duty_output = motor.speed_duty_output+(int16)(motor.speed_p*(motor.speed_current_error-motor.speed_last_error)+motor.speed_i*motor.speed_current_error);      
      
      if(motor.speed_duty_output>=990)
        motor.speed_duty_output = 990;
      else if(motor.speed_duty_output<=-990)
        motor.speed_duty_output = -990; 
      
      if(motor.speed_duty_output>=0)    
      {
        ftm_pwm_duty(GO_FTM,GO_CH,motor.speed_duty_output);
        ftm_pwm_duty(BACK_FTM,BACK_CH,0);      
      }
      else
      {        
        ftm_pwm_duty(GO_FTM,GO_CH,0);
        ftm_pwm_duty(BACK_FTM,BACK_CH,-motor.speed_duty_output);          
      }   
    }
  }
  else
  {
    //发车控制
    if((cartime>300)&&(cartime<=400)&&(motor.speed<80))
    {
      ftm_pwm_duty(GO_FTM,GO_CH,800);
      ftm_pwm_duty(BACK_FTM,BACK_CH,0);   
    }
    else if(cartime>400)
    {
      if(motor.speed<80)
      {
        ftm_pwm_duty(FTM0,FTM_CH0,0); 
        ftm_pwm_duty(FTM0,FTM_CH1,0);
      }
      else
      {
        motor.speed_last_error = motor.speed_current_error ;        //更新每次的差值
        motor.speed_current_error = motor.speed_set-motor.speed;   //速度当前差值
        
        motor.speed_duty_output = motor.speed_duty_output+(int16)(motor.speed_p*(motor.speed_current_error-motor.speed_last_error)+motor.speed_i*motor.speed_current_error);      
        
        if(motor.speed_duty_output>=990)
          motor.speed_duty_output = 990;
        else if(motor.speed_duty_output<=-990)
          motor.speed_duty_output = -990; 
        
        if(motor.speed_duty_output>=0)    
        {
          ftm_pwm_duty(GO_FTM,GO_CH,motor.speed_duty_output);
          ftm_pwm_duty(BACK_FTM,BACK_CH,0);      
        }
        else
        {        
          ftm_pwm_duty(GO_FTM,GO_CH,0);
          ftm_pwm_duty(BACK_FTM,BACK_CH,-motor.speed_duty_output);          
        }   
      }
    }
  }
}

