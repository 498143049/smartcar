#include "include.h"

extern uint8 img_handle[CAMERA_H][CAMERA_W];  
extern uint8 KeyValue;

void ScreenShow(void)
{
  if(KeyValue==1)
  {
   LCD_img();
   CenterShow();//œ‘ æ÷–œﬂ
   LeftShow();  //œ‘ æ◊Û±ﬂ‘µ£®∫Ï£©
   RightShow(); //œ‘ æ”“±ﬂ‘µ£®ª∆£©
   LCD_ShowNum(0,110,(int32)(RED1),2);
   LCD_ShowNum(20,110,(int32)(RED2),2);
   LCD_ShowNum(40,110,(int32)(RED3),2);
   LCD_ShowNum(60,110,(int32)(RED4),2);
   //LCD_ShowNum(0,110,(int32)(Blackline_flag),3);
//   LCD_ShowNum(30,110,(int32)(Mline[85]),3);
//   LCD_ShowNum(60,110,(int32)(Mline[119]),3);
   LCD_ShowNum(120,110,(int32)(motor.speed_set),4);
  } 
}


void LCD_img(void)
{
  int i,j;
  Address_set(0,0,OV7725_EAGLE_W-1,10);
  for(i=0;i<10;i++)
  {
    for(j=0;j<CAMERA_W;j++)
    {
      if(img_handle[i][j]==255)
      {
        LCD_Write_Data2(WHITE);
      }
      if(img_handle[i][j]==0)
      {
        LCD_Write_Data2(BLACK);
      }
    }
  }
  
  Address_set(0,20,OV7725_EAGLE_W-1,105);
  for(i=35;i<120;i++)
  {
    for(j=0;j<CAMERA_W;j++)
    {
      if(img_handle[i][j]==255)
      {
        LCD_Write_Data2(WHITE);
      }
      if(img_handle[i][j]==0)
      {
        LCD_Write_Data2(BLACK);
      }
    }
  }
}

/***************÷–œﬂœ‘ æ*********************/
void CenterShow(void)
{
  uint8 i;
  uint16 H;
  uint16 W;
  for(i=0;i<85;i++)
  {
    if((Mline[i+35]>=0)&&(Mline[i+35]<=159))
    {
      W=(uint16)(Mline[i+35]);
      H=20+i;
      LCD_Fill(W,H,W,H,BLUE);
    }
    else if(Mline[i+35]<0)
    {
     W=0;
     H=20+i;
     LCD_Fill(W,H,W,H,BLUE);
    }
    else if(Mline[i+35]>159)
    {
     W=159;
     H=20+i;
     LCD_Fill(W,H,W,H,BLUE);
    }
  }
}
/****************◊Û±ﬂ‘µœ‘ æ*********************/
void LeftShow(void)
{
  uint8 i;
  uint16 H;
  uint16 W;
  for(i=0;i<85;i++)
  {
    if((Lline[i+35]>=0)&&(Lline[i+35]<=159))
    {
      W=(uint16)(Lline[i+35]);
      H=20+i;
      LCD_Fill(W,H,W,H,RED);
    }
    else if(Lline[i+35]<0)
    {
     W=0;
     H=20+i;
     LCD_Fill(W,H,W,H,RED);
    }
    else if(Lline[i+35]>159)
    {
     W=159;
     H=20+i;
     LCD_Fill(W,H,W,H,RED);
    }
  }
}



/****************”“±ﬂ‘µœ‘ æ*********************/
void RightShow(void)
{
  uint8 i;
  uint16 H;
  uint16 W;
  for(i=0;i<85;i++)
  {
    if((Rline[i+35]>=0)&&(Rline[i+35]<=159))
    {
      W=(uint16)(Rline[i+35]);
      H=20+i;
      LCD_Fill(W,H,W,H,YELLOW);
    }
    else if(Rline[i+35]<0)
    {
     W=0;
     H=20+i;
     LCD_Fill(W,H,W,H,YELLOW);
    }
    else if(Rline[i+35]>159)
    {
     W=159;
     H=20+i;
     LCD_Fill(W,H,W,H,YELLOW);
    }
  }
}