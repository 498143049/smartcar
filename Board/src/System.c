/********************************ϵͳ��ʼ��**********************************
*
*����   �� ɳ���Ѿ�
*�ļ��� �� System.c
*����   �� ϵͳ��ʼ��
*ʱ��   �� 2015/11/9
˵��    ��ʹ��ɽ��V5.3��
*
****************************************************************************/
#include "System.h"
#include "include.h"

void System_init()
{  
        Tft_init();//��ʾ����ʼ��
        LCD_Clear(BLUE);//����
        //��ʼ������ͷ
        camera_init(img_buffer);                                  //�����趨  imgbuff Ϊ�ɼ�������������������
        
        //�����жϷ�����
        NVIC_SetPriorityGrouping(4);            //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�
        NVIC_SetPriority(PIT2_IRQn,0);           //�������ȼ�
        NVIC_SetPriority(PORTC_IRQn,1);         //�������ȼ�
        NVIC_SetPriority(DMA0_IRQn,2);           //�������ȼ�
        NVIC_SetPriority(PIT0_IRQn,3);          //�������ȼ�
         
        set_vector_handler(PORTC_VECTORn ,PORTC_IRQHandler);    //����PORTC���жϷ�����Ϊ PORTC_IRQHandler
        set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //����DMA0���жϷ�����Ϊ DMA0_IRQHandler
        set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);
        set_vector_handler(PIT2_VECTORn,PIT2_IRQHandler);

        disable_irq (PIT0_IRQn);    //����PIT0��ʱ���ж�
        disable_irq (PIT2_IRQn);
        enable_irq (PORTA_IRQn); 
        pit_init_ms(PIT0,10);     //��ʼ��PIT0,��ʱʱ��Ϊ�� 10ms
        pit_init_ms(PIT2,1); 
        Buzzer_init();           //��������ʼ��         
       // Boot_reset();            //boot��ʼ��
        ftm_quad_init(FTM2);     //FTM2  ���������ʼ��       
        Servo_Motor_init();      //�����ʼ��
        Motor_init();            //�����ʼ��	       
        Key_init();              //������ʼ�� 
        BM_init();               //���뿪�س�ʼ��
        RED_init();
}

void ADC_init()
{
 adc_init(ADC0_SE12) ;       //power_adc   
}

float Battery_voltage()
{
        uint8 ad_value;        
        ad_value = adc_once(ADC0_SE12,ADC_8bit);
        voltage = ad_value*0.0258 ; // 0.0258 = (3.3/256)*(200/100)   
        return(voltage);              
}

void Buzzer_init()
{
 gpio_init(PTE0,GPO,0);
}

//void Boot_reset()
//{
//        DELAY_MS(100); 
//        beep(20);
//        DELAY_MS(50);
//        beep(20);    
//        DELAY_MS(100); 
//}
//void beep(uint8 ms)
//{
//        BEEP = 1;
//        DELAY_MS(ms);
//        BEEP = 0;
//}

void Key_init()
{
  gpio_ddr (PTE1, GPI); 
  gpio_ddr (PTE2, GPI);
  gpio_ddr (PTE3, GPI);
  gpio_ddr (PTE4, GPI);
  
  port_init(PTE1,ALT1 |PULLUP);
  port_init(PTE2,ALT1 |PULLUP);
  port_init(PTE3,ALT1 |PULLUP);
  port_init(PTE4,ALT1 |PULLUP);
}


void BM_init()
{
  gpio_ddr (PTC4, GPI); 
  gpio_ddr (PTC5, GPI);
  gpio_ddr (PTC6, GPI);
  gpio_ddr (PTC7, GPI);
  
  port_init(PTC4,ALT1 |PULLUP);
  port_init(PTC5,ALT1 |PULLUP);
  port_init(PTC6,ALT1 |PULLUP);
  port_init(PTC7,ALT1 |PULLUP);
}
//void beepms(uint8 ms)//ʹ���ж�
//{
//    if(beep_flag==1)
//    { 
//        BEEP = 1;
//      //  stop++;
//        beep_flag=0;   
//        pit_init_ms(PIT3,150);
//        enable_irq (PIT3_IRQn);
//    }
//}
void RED_init()
{
  gpio_init(PTB0,GPI,LOW); //���ܺ���
  gpio_init(PTB1,GPI,LOW);
  gpio_init(PTB2,GPI,LOW);
  gpio_init(PTB3,GPI,LOW);
}
/******************************************************
* @author : WWW
* @function name : OLED_Show_Road
* @ data : 2016/1/28
* @function description : oled��ʾ����
******************************************************/
void OLED_Show_Road()
{
    uint8 i,j;
    uint8 left,right;
    uint8 mid;
    uint8 TLline[CAMERA_H];
    uint8 TRline[CAMERA_H];
    uint8 TMline[CAMERA_H]; 
      
    for(i=0;i<60;i++)
    {
        TLline[i]=(uint8)(Lline[i*2]/2);
        TRline[i]=(uint8)(Rline[i*2]/2);
        if( Mline[i*2] < 0)
         TMline[i] = 0 ;
        else if( Mline[i*2] > 159)
          TMline[i] = 79;
        else if( Mline[i*2] > 0 && Mline[i*2] <159 )
          TMline[i] = (uint8)(Mline[i*2]/2);
    }     
    //���������߽�
   for(i=0;i<8;i++)
   {
     LED_Set_Pos(4,i);	     
     LED_WrDat(0xff);	
   }
   for(i=0;i<8;i++)
   {
     LED_Set_Pos(92,i);	     
     LED_WrDat(0xff);	
   }
   //�����ʾ��
   for(i=0;i<8;i++)
   {
     for(j=0;j<80;j++)
     {
       LCDRAM[i][j]=0;
     }
   }
 
   for(i=0;i<60;i++)
   {
     left=TLline[i];
     LCDRAM[i/8][left]=LCDRAM[i/8][left]|(0x01<<(i%8));
   }

   for(i=0;i<60;i++)
   {
     right=TRline[i];
     LCDRAM[i/8][right]=LCDRAM[i/8][right]|(0x01<<(i%8));
   }
   //������
     for(i=0;i<60;i++)
     {
      mid=TMline[i];
       LCDRAM[i/8][mid]=LCDRAM[i/8][mid]|(0x01<<(i%8));
     }   
   for(i=8;i>0;i--)
   {
     LED_Set_Pos(9,i-1);				
     for(j=0;j<80;j++)
     {      
         LED_WrDat(LCDRAM[i-1][j]);	    	
     }
   }
   LED_PrintValueF(96,0,camer.speed_control_error, 0);
}

void show_img()//120*160
{
    uint8 i,j;
     //�����ʾ��
   for(i=0;i<8;i++)
   {
     for(j=0;j<80;j++)
     {
       LCDRAM[i][j]=0;
     }
   }
  for(j=0;j<80;j++)
  {
    for(i=0;i<60;i++)
    {
        if(img_handle[i*2][j*2]==255)
           LCDRAM[i/8][j]=LCDRAM[i/8][j]|(0x01<<(i%8));
    }  
  }
     //��ʾ
   for(i=8;i>0;i--)
   {
     LED_Set_Pos(9,i-1);				
     for(j=0;j<80;j++)
     {      
         LED_WrDat(LCDRAM[i-1][j]);	    	
     }
   }

}

