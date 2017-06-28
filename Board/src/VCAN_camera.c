#include "include.h"
#include "VCAN_camera.h"
#include "extern.h"

camer_status camer;

FATFS firefs;    //文件系统
FIL   firesrc;  //文件

float Correction_array[120] = {4.03, 4.03, 4.0, 3.97, 3.97, 3.97, 3.94, 3.89, 3.86, 3.86, 3.86, 3.83, 
3.78, 3.75, 3.75, 3.72, 3.72, 3.67, 3.67, 3.64, 3.64, 3.58, 3.58, 3.53, 
3.53, 3.5, 3.47, 3.47, 3.44, 3.42, 3.39, 3.39, 3.33, 3.33, 3.28, 3.28, 
3.25, 3.22, 3.22, 3.17, 3.17, 3.11, 3.11, 3.08, 3.06, 3.03, 3.03, 2.97, 
2.97, 2.94, 2.92, 2.89, 2.86, 2.86, 2.83, 2.81, 2.75, 2.75, 2.72, 2.69, 
2.64, 2.64, 2.64, 2.58, 2.58, 2.53, 2.53, 2.47, 2.47, 2.44, 2.42, 2.39, 
2.36, 2.33, 2.31, 2.28, 2.25, 2.19, 2.19, 2.14, 2.14, 2.11, 2.08, 2.06, 
2.03, 2.0, 1.97, 1.94, 1.94, 1.86, 1.86, 1.81, 1.78, 1.78, 1.72, 1.69, 
1.67, 1.67, 1.61, 1.61, 1.56, 1.56, 1.53, 1.47, 1.44, 1.39, 1.39, 1.39, 
1.33, 1.28, 1.28, 1.25, 1.22, 1.17, 1.17, 1.11, 1.11, 1.06, 1.03, 1.0};


//float Correction_array[120] = { 4.03, 3.82, 3.72, 3.72, 3.54, 3.45, 3.37, 3.37, 3.3, 3.09,
//                                3.09, 3.09, 2.96, 2.84, 2.79, 2.79, 2.69, 2.64, 2.54, 2.54, 
//                                2.50, 2.46, 2.42, 2.38, 2.34, 2.30, 2.23, 2.23, 2.16, 2.13,
//                                2.10, 2.10, 2.04, 2.01, 1.99, 1.96, 1.93, 1.91, 1.88, 1.86,
//                                1.81, 1.81, 1.77, 1.77, 1.77, 1.73, 1.73, 1.69, 1.69, 1.65, 
//                                1.65, 1.63, 1.61, 1.59, 1.58, 1.58, 1.54, 1.54, 1.54, 1.51, 
//                                1.51, 1.48, 1.48, 1.45, 1.45, 1.44, 1.42, 1.41, 1.41, 1.38,
//                                1.38, 1.36, 1.34, 1.34, 1.32, 1.32, 1.31, 1.29, 1.29, 1.27,
//                                1.27, 1.26, 1.25, 1.25, 1.24, 1.23, 1.22, 1.22, 1.21, 1.20,
//                                1.19, 1.19, 1.17, 1.17, 1.15, 1.15, 1.13, 1.13, 1.12, 1.12,
//                                1.11, 1.11, 1.09, 1.09, 1.09, 1.07, 1.07, 1.07, 1.06, 1.05,
//                                1.05, 1.04, 1.04, 1.03, 1.02, 1.01, 1.01, 1.01, 1.00, 1.00};



void img_extract(void *dst, void *src, uint32_t srclen)
{
  uint8_t colour[2] = {255, 0}; //0 和 1 分别对应的颜色
  uint8_t * mdst = dst;
  uint8_t * msrc = src;
  //注：山外的摄像头 0 表示 白色，1表示 黑色
  uint8_t tmpsrc;
  while(srclen --)
  {
    tmpsrc = *msrc++;
    *mdst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
    *mdst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
  }
}

/*********************************************************** 
* @author： VCAN
* @fuction name:　img_sd_init
* @date : 2016/1/18
* @description： SD卡保存图像
***********************************************************/

void img_sd_init(void)
{
  int   fireres;
  char  myfilename[20];
  uint16 imgsize[] = {CAMERA_H, CAMERA_W};
  uint32 mybw;
  uint32 Imag_num = 0;
  
  f_mount(0, &firefs);
  
  do
  {
    Imag_num ++;
    sprintf(myfilename, "0:/Fire%d.sd", Imag_num);
    fireres = f_open( &firesrc , myfilename, FA_CREATE_NEW | FA_WRITE);
    
    if(firefs.fs_type == 0)
    {
      firesrc.fs = 0;
      return;
    }
    
  }
  while(fireres == FR_EXIST);        //如果文件存在，则命名继续加1
  
  if ( fireres == FR_OK )
  {
    fireres = f_write(&firesrc, imgsize, sizeof(imgsize), (uint32 *)&mybw);  //先写入高和宽，方便上位机处理
  }
  else
  {
    f_close(&firesrc);
    firesrc.fs = 0;
  }
}

/*********************************************************** 
* @author： VCAN
* @fuction name: img_sd_save
* @date : 2016/1/18
* @fuction description： SD卡保存图像
***********************************************************/

void img_sd_save(uint8 * imgaddr)
{
#define SD_ONEWRITE_MAX 600   //旧库（5.0之前的）一次写入太大会出错，新库修复了bug，所以新库此值可取比较大。
  
  int   fireres;
  uint32 mybw;
  static uint8 time = 0;
  uint32 size = CAMERA_SIZE;
  
  if(firesrc.fs != 0)
  {
    time ++;
    
    while(size > SD_ONEWRITE_MAX)
    {
      fireres = f_write(&firesrc, imgaddr, SD_ONEWRITE_MAX , (uint32 *)&mybw);  
      imgaddr +=   SD_ONEWRITE_MAX;  
      size -=  SD_ONEWRITE_MAX; 
    }
    
    fireres = f_write(&firesrc, imgaddr, size , (uint32 *)&mybw);  
    
    if(fireres != FR_OK)
    {
      f_close(&firesrc);
      firesrc.fs = 0;
    }
    
    if(time > 30)
    {
      time = 0 ;
      f_sync(&firesrc);
    }
  }
}

/*********************************************************** 
* @author： 沙艺已觉
* @date : 2016/3/5
* @fuction name： Get_Edge
* @fuction description： 采集赛道边缘
***********************************************************/

void Get_Edge(void)
{    
  int16 i,j;    //循环变量  
  uint8 Barrior_Flag=0;
  uint8 nomal_track_flag = 1; //正常赛道
  uint8 l_complete_lost_flag = 0 ;  //左线完全丢线标志
  uint8 r_complete_lost_flag = 0;   //右线完全丢失标志
  uint8 left_status = 0;    // 左线状态  上黑下白为1  上白下黑为2 
  uint8 right_status = 0;   // 右线状态  上黑下白为1  上白下黑为2
  int8 left_jump_point = 0;
  int8 right_jump_point = 0; 
  int8 l_jump_point[5] = {0};
  int8 r_jump_point[5] = {0};
  int8 left_jump_point1=0,right_jump_point1=0;
  int8 left_jump_point2=0,right_jump_point2=0;
  int8 Fjump_end;             //跳变点滤波终止行
  int8 l_jump_count = 0;
  int8 r_jump_count = 0;
  int8 min_jump_count;
  int8 l_cross_turn_point = 0;            //左斜十字拐点位置
  int8 r_cross_turn_point = 0;            //右斜十字拐点位置
  uint8 l_diagonally_flag =1 ;     //左斜入十字前为直道
  uint8 r_diagonally_flag =1;      //右斜入十字前为直道
  float l_First_derivative[118] = {0};   //左线一阶导
  float r_First_derivative[118] = {0};   //右线一阶导
  float l_average[116] = {0};            //左线平均值
  float r_average[116] = {0};            //右线平均值
  float l_variance[116] = {0};           //左线方差
  float r_variance[116] = {0};           //右线方差
  int16 leftline_start = L_LINE_START ;  //左边线寻线起点
  int16 rightline_start = R_LINE_START ; //右边线寻线起点 
  int8 l_jump_derivative ; 
  int8 r_jump_derivative ;
  float range_derivative;
  int8 row ;   //寻线行变量
  
  //寻线
  for(row = 0 ; row <120 ; row ++)
  {
    Lline[row] = L_LINE_INITIAL_VALUE ; 
    Rline[row] = R_LINE_INITIAL_VALUE ;
    Mline[row] = M_LINE_INITIAL_VALUE ;           
  }  
  //寻线
  for(row = 119 ; row >=0 ;row --)
  {
    //寻左线 
    if( l_complete_lost_flag == 0)
    {
      for( i = leftline_start ; i>0 ; i--)
      { 
        if(leftline_start > 162)   //左边线已寻到最右边                       
          break ;
        if((img_handle[row][i+2]+img_handle[row][i+1]) == 510 && (img_handle[row][i] + img_handle[row][i-1]) == 0)
        {
          Lline[row] = i ;
          if(row<118)
          {    
            if(ABS(Lline[row+2] - Lline[row+1]) < 20)
            {
             // leftline_start = i + ABS(Lline[row+2] - Lline[row+1])+5 ;
             leftline_start=(Lline[row+1] +Rline[row+1])/2 ;
            }
            else
            {
              leftline_start = L_LINE_START;
            }
          }
          else
            leftline_start = i + 5;
          break;
        } 
      } 
      if(i == 0)
      {
        if(row < 118)
        {
          if( (Rline[row+2]-Rline[row+1]) >4&& (Rline[row+1] < 60))
          {
            l_complete_lost_flag = 1;
          }
          else
          {
            leftline_start =(int16)((Rline[row+1] + Lline[row+1])/2.0);
          }
        }
      } 
    }
    //    寻右线
    if(r_complete_lost_flag == 0)
    {
      for( j = rightline_start;j<159;j++)
      {
        if(rightline_start <-3) 
          break;
        if((img_handle[row][j-2] + img_handle[row][j-1]) == 510 && (img_handle[row][j] + img_handle[row][j+1]) == 0)
        {
          Rline[row] = j;
          if(row<118)
          {
            if((ABS(Rline[row+2] - Rline[row+1]) < 20))
            {
             // rightline_start = j - ABS(Rline[row+2] - Rline[row+1])-5;
              rightline_start=(Lline[row+1] +Rline[row+1])/2 ;
            }
            else
            {
              rightline_start = R_LINE_START;
            }
          }
          else
            rightline_start = j -5;
          break;
        }
      }    
      if(j == 159)
      {
        if(row < 118)
        {
          if((Lline[row+1]-Lline[row+2])> 4 && (Lline[row+1] >100) )
          {
            r_complete_lost_flag = 1;
          }
          else
          {
            rightline_start = (int16)((Rline[row+1] + Lline[row+1])/2.0);
          }
        }
      }  
    }
  }
  
  ImgFilter(Lline);
  ImgFilter(Rline);
  //求导
  for(row = 117;row>=0;row--)
  {
    l_First_derivative[row] = (Lline[row+2] - Lline[row])/2.0;
    r_First_derivative[row] = (Rline[row+2] - Rline[row])/2.0;
  }
  
  //跳变点判断
  for(row = 115; row>=0;row--)
  {
    l_average[row] = calculation_average(l_First_derivative[row+2],l_First_derivative[row+1],l_First_derivative[row]);   //计算平均值
    
    l_variance[row] = calculation_variance(l_First_derivative[row+2],l_First_derivative[row+1],l_First_derivative[row],l_average[row]);    //计算方差
    if(l_variance[row] > VARIANCE_THRESHOLD)
    {
      if(left_jump_point == 0 )
      {
        left_jump_point = row+2;
      }

      if(l_variance[row+1] < VARIANCE_THRESHOLD && l_jump_count < 5)
      {
        l_jump_point[l_jump_count] = row + 2;
        if(l_jump_count>0&&l_jump_point[l_jump_count-1]-l_jump_point[l_jump_count]<5)
        {
            l_jump_count --;
        }
        l_jump_count ++ ;
      }
    }
    r_average[row] = calculation_average(r_First_derivative[row+2],r_First_derivative[row+1],r_First_derivative[row]);   //计算平均值
    r_variance[row] = calculation_variance(r_First_derivative[row+2],r_First_derivative[row+1],r_First_derivative[row],r_average[row]);   //计算方差
    if(r_variance[row] > VARIANCE_THRESHOLD)
    {
      if(right_jump_point ==0)
      {
        right_jump_point = row+2; 
      }

      if(r_variance[row+1] < VARIANCE_THRESHOLD && r_jump_count < 5)
      {
        r_jump_point[r_jump_count] = row + 2;
        if(r_jump_count>0&&r_jump_point[r_jump_count-1]-r_jump_point[r_jump_count]<5)
        {
            r_jump_count --;
        }
        r_jump_count ++ ;
      }
    }
  }
  
  //停车黑线识别
  if(nomal_track_flag == 1)
  {
    //两边都有一个拐点
       if(r_jump_count==1 && l_jump_count==1)
       {
            if(MMN(Lline,0,110,0)>0&&MMN(Rline,0,110,1)<159)
            {
                nomal_track_flag = 0;
                Prospect_See = 0;
                Track_complexity = 40;
                 //找拐点
                if(left_jump_point> 10 )
                {
                  for(row = 115;row > left_jump_point - 10 ; row --)
                  {
                    if(((l_First_derivative[row+1]*l_First_derivative[row-1])<0))
                    {
                      if(l_cross_turn_point == 0)
                      {
                        l_cross_turn_point = row;
                        break;      //找到拐点跳出循环
                      }
                    }
                  }
                }
                if(right_jump_point> 10)
                {
                    for(row = 115;row > right_jump_point - 10 ; row --)
                    {
                        if(((r_First_derivative[row+1]*r_First_derivative[row-1])<0))
                        {
                          if(r_cross_turn_point == 0)
                          {
                            r_cross_turn_point = row;
                            break;      //找到拐点跳出循环
                          }
                        }
                     }
                }
                if(l_cross_turn_point && r_cross_turn_point && cartime >2800)
                {
                      //起跑
                    Blackline_flag = 1;
                }
            }
       }
  }
  
  /***障碍处理*****/
 if(nomal_track_flag == 1 )
 {
      if((r_jump_count==2 &&l_jump_count==0)||(l_jump_count==2&&r_jump_count==0))
      {  
        if(r_jump_count)    //障碍在右边
        {
          Barrior_Flag=1;
                if(MMN(Lline,r_jump_point[1],r_jump_point[0],0)>0&&MMN(Rline,r_jump_point[1],r_jump_point[0],1)<159)
                {
                    nomal_track_flag = 0;
                    if(r_jump_point[0] <110 || r_jump_point[1] >10)
                    {
                        for(row = r_jump_point[0]+10;row>=(r_jump_point[1]-10);row --)
                        {
                            Rline[row] = 70;
                        }
                    }
                    else if(r_jump_point[0] >=110)
                    {
                        for(row = 119;row>=(r_jump_point[1]-10);row --)
                        {
                            Rline[row] = 70;
                        }
                    }
                    else if(r_jump_point[1] <=10)
                    {
                       for(row =r_jump_point[0]+10;row>=0;row --)
                        {
                            Rline[row] = 70;
                        } 
                    }
                    Prospect_See = 0;
                    Track_complexity = 30;
                }       
        }
        else if(l_jump_point) //障碍在左边
        {
          Barrior_Flag=1;
            if(MMN(Lline,r_jump_point[1],r_jump_point[0],0)>0&&MMN(Rline,r_jump_point[1],r_jump_point[0],1)<159)
            {
                nomal_track_flag = 0;
                if(l_jump_point[0]<110 || l_jump_point[1] >10)
                {
                    for(row = l_jump_point[0]+10;row>(l_jump_point[1]-10);row --)
                    {
                        Lline[row] = 90;
                    }
                }
                else if(l_jump_point[0] >=110)
                {
                    for(row = 119;row>=(l_jump_point[1]-10);row --)
                    {
                        Lline[row] = 90;
                    }
                }
                else if(l_jump_point[1] <=10)
                {
                   for(row =l_jump_point[0]+10;row>=0;row --)
                    {
                        Lline[row] = 90;
                    } 
                }
                Prospect_See = 0;
                Track_complexity = 40;
            }
        }
      }
      
      //停车黑线识别
      else if((r_jump_count==1 &&l_jump_count==0)||(l_jump_count==1&&r_jump_count==0))//一边有一个拐点
      {
          if(r_jump_count) 
          {   
                  if(MMN(Lline,0,110,0)>0&&MMN(Rline,0,110,1)<159)
                  {
                      if(r_jump_point[0] >20)
                      {
                           //找拐点
                          for(row = 115;row > (right_jump_point - 10) ; row --)
                          {
                            if(((r_First_derivative[row+1]*r_First_derivative[row-1])<0))
                            { 
                              if(r_cross_turn_point == 0)
                              {
                                r_cross_turn_point = row;
                                break;      //找到拐点跳出循环
                              }
                            }
                          }
                         // if(r_cross_turn_point != 0 && cartime > 800 && r_jump_point[0] < 50)  // 找到拐点为起跑线
                          if(r_cross_turn_point != 0 && cartime > 2800)  // 找到拐点为起跑线
                          {
//                            if(Barrior_Flag==0)
//                            {
                             Blackline_flag = 1 ;
                           // }
                          }
                          else if(r_cross_turn_point == 0 )  //障碍
                          {
                              nomal_track_flag = 0;
                              Prospect_See = 0;
                              Track_complexity = 40;
//                               for(row = 119 ;row >=  (right_jump_point - 10) ;row--)
//                               {
//                                 Rline[row] = 40;
//                               }
                          }
                      }
                  }
          }
          else if(l_jump_count)
          {
                if(MMN(Lline,0,110,0)>0&&MMN(Rline,0,110,1)<159)
                  {
                      if(l_jump_point[0] >20)
                      {
                           //找拐点
                          for(row = 115;row > left_jump_point - 10 ; row --)
                          {
                            if(((l_First_derivative[row+1]*l_First_derivative[row-1])<0))
                            {
                              if(l_cross_turn_point == 0)
                              {
                                l_cross_turn_point = row;
                                break;      //找到拐点跳出循环
                              }
                            }
                          }
                          if(l_cross_turn_point != 0 && cartime >2800)  // 找到拐点为起跑线
                         // if(r_cross_turn_point != 0 && cartime > 800 && r_jump_point[0] < 50)  // 找到拐点为起跑线
                          {
//                            if(Barrior_Flag==0)
//                            {
                              Blackline_flag = 1 ;
//                            }
                          }
                          else if(l_cross_turn_point == 0 )  //障碍
                          {
                              nomal_track_flag = 0;
                              Prospect_See = 0;
                              Track_complexity = 40;
//                               for(row = 119 ;row >=  (left_jump_point - 10) ;row--)
//                               {
//                                     Lline[row] = 120;
//                               }
                          }
                      }
                  }
          }
      }
 }
  
  //尖角识别
  if(nomal_track_flag==1&&l_jump_count+r_jump_count>2 && l_jump_point[0]==r_jump_point[0])
  {
    if(l_jump_point[1]>r_jump_point[1] && MMN(Lline,l_jump_point[1]+2,l_jump_point[0]-2,0)>MMN(Rline,l_jump_point[1]+2,l_jump_point[0]-2,1))
    {
      nomal_track_flag=0;
      for(row=0;row<120;row++)
        Lline[row]=0;
    }
    else if(l_jump_point[1]<r_jump_point[1] && MMN(Lline,r_jump_point[1]+2,r_jump_point[0]-2,0)>MMN(Rline,r_jump_point[1]+2,r_jump_point[0]-2,1))
    {
      nomal_track_flag=0;
      for(row=0;row<120;row++)
        Rline[row]=159;
    }
  }
  
  
  if(nomal_track_flag == 1)
  {   
    //左斜入十字
    if(right_jump_point && (right_jump_point > left_jump_point))    //左斜入十字 右跳变存在且大于左跳变
    {
      for(row = 115;row > right_jump_point ; row --)
      {
        if(((l_First_derivative[row+2]*l_First_derivative[row-2])<0))
        {
          if(l_cross_turn_point == 0)
          {
            l_cross_turn_point = row;
            break;      //找到拐点跳出循环
          }
        }
      }
      if(l_cross_turn_point != 0)     //找到拐点
      {  
        //                      mexPrintf("l_cross_turn_point%d\n",l_cross_turn_point);
        if(l_diagonally_flag == 1)
        {
          for(row = l_cross_turn_point ; row >=right_jump_point;row--)
          {
            if(Rline[row] != 159)   //左拐点到右跳变    右边丢线
            {
              l_diagonally_flag = 0 ;
            }
          }
        }
        
        if(Rline[119] == 159 && Rline[118] ==159 && Rline[117] ==159 && l_diagonally_flag)
        {
          for(row = 116 ; row > right_jump_point ; row--)
          {
            if(Rline[row] != 159)
            {    
              l_diagonally_flag  = 0 ;
              break;
            }
          }
        }
        else
        {
          for(row = 117 ; row >= l_cross_turn_point ;row --)    //拐点之前 左右斜率乘积
          {
            if((l_First_derivative[row] * r_First_derivative [row] )>1.0)
            {
              l_diagonally_flag  = 0 ;
            }
          }     
        }
        
        if(l_diagonally_flag == 1)
        {
          for(row = l_cross_turn_point ; row >=0; row--)
          {
            Lline[row] = 0; 
          }
          for(row = right_jump_point - 1 ; row >0 ;row --)
          {
            leftline_start = Rline[right_jump_point - 3];
            for(i = 157;i > leftline_start;i --)     //从左跳变找右线
            {
              if((img_handle[row][i+2]+img_handle[row][i+1]) == 510 && (img_handle[row][i] + img_handle[row][i-1]) == 0)
              {
                Lline[row] = i ;
                break;
              }
            }
          }
          
          if(Lline[right_jump_point-2] != 0)
          {
            for(row = right_jump_point + 1; row > 0; row --)
            {
              Rline[row] = 159;
            }
            ImgFilter_range(right_jump_point - 1,0,Lline);
            matchline(l_cross_turn_point+3,right_jump_point-3,Lline);
            nomal_track_flag =0 ;  
            /*  2016、7、8  */
            range_derivative = Slope_Range(l_cross_turn_point,l_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = 120-l_cross_turn_point;
          }
        }
      }
    }
    
// 右斜入十字
    if(left_jump_point && (left_jump_point > right_jump_point))   //右斜入十字  左跳变存在且大于右跳变
    { 
      for(row = 115;row > left_jump_point ; row --)
      {
        if((r_First_derivative[row+2]*r_First_derivative[row-2])<0)
        {
          if(r_cross_turn_point ==0)
          {
            r_cross_turn_point = row;
            break;    //找到拐点跳出循环
          }
        }         
      }
      if(r_cross_turn_point != 0)     //找到拐点
      {
        if(r_diagonally_flag == 1)
        {
          for(row = r_cross_turn_point ; row >=left_jump_point;row --)
          {
            if(Lline[row] != 0)   //右拐点到左跳变    左边丢线
            {
              r_diagonally_flag = 0 ;
            }
          }
        }
        
        if(Lline[119] == 0 && Lline[118] ==0 && Lline[117] == 0 && r_diagonally_flag)
        {
          for(row = 116 ; row > left_jump_point ; row--)
          {
            if(Lline[row] != 0)
            {    
              r_diagonally_flag  = 0 ;
              break;
            }
          }
        }
        else
        {
          for(row = 117 ; row >= r_cross_turn_point ;row --)    //拐点之前 左右斜率乘积
          {
            if((l_First_derivative[row] * r_First_derivative [row] )>1.0)
            {
              r_diagonally_flag  = 0 ;
            }
          }            
        }
        if(r_diagonally_flag == 1)      
        { 
          for(row = r_cross_turn_point ; row >=0; row--)
          {
            Rline[row] = 159; 
          }
          for(row = left_jump_point - 1 ; row >0 ;row --)
          {
            rightline_start = Lline[left_jump_point - 3];
            for(j = 2;j < rightline_start;j ++)     //从左跳变找右线
            {
              if((img_handle[row][j-2] + img_handle[row][j-1]) == 510 && (img_handle[row][j] + img_handle[row][j+1]) == 0)
              {
                Rline[row] = j ;
                break;
              }
            }
          }
          if(Rline[left_jump_point - 2] != 159)
          {
            for(row = left_jump_point +1 ; row>0 ;row --)
            {
              Lline[row] = 0;
            }
            ImgFilter_range(left_jump_point - 1,0,Rline);
            matchline(r_cross_turn_point+3 ,left_jump_point - 3,Rline);  //两点连线
            nomal_track_flag = 0 ;
            /*  2016、7、8  */
            range_derivative = Slope_Range(r_cross_turn_point,r_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = 120-r_cross_turn_point;
          }
        }
      }
    }
  }
  
  if(left_jump_point && right_jump_point && ABS(left_jump_point - right_jump_point)<10 && nomal_track_flag==1)       //正入十字
  {
    if(l_jump_count >= r_jump_count)
      min_jump_count = r_jump_count;
    else
      min_jump_count = l_jump_count;
    if(left_jump_point<115 && left_jump_point > 20)
    {
      for(row = left_jump_point+2 ;row > left_jump_point-3;row--)
      {
        if(Lline[row+3] == 0&& Lline[row+2] ==0 && Lline[row+1] ==0 &&Lline[row-1]!=0 && Lline[row-2]!=0 && Lline[row-3] !=0 )
        {
          left_status  = 1;
          break;
        } 
        else if(Lline[row+3] != 0&& Lline[row+2] !=0 && Lline[row+1] !=0 &&Lline[row-1]==0 && Lline[row-2]==0 && Lline[row-3] ==0)
        {
          left_status = 2;
          break;
        }
      }
    }
    if(right_jump_point <115 && right_jump_point > 20)
    {
      for(row = right_jump_point+2 ; row > right_jump_point-3;row--)
      {
        if(Rline[row+3]==159 && Rline[row+2]==159 && Rline[row+1]==159 && Rline[row-1]!=159 && Rline[row-2]!=159 && Rline[row-3]!=159)
        {
          right_status = 1;
          break;
        }
        else if(Rline[row+3]!=159 && Rline[row+2]!=159 && Rline[row+1]!=159 && Rline[row-1]==159 && Rline[row-2]==159 && Rline[row-3]==159)
        {
          right_status = 2;
          break; 
        }
      }
    }
    if(l_jump_count==1 && r_jump_count ==1)        //左右都只有一个跳变点
    {
      if( left_status == right_status && left_status != 0)
      {
        nomal_track_flag = 0;
        if( left_status == 1 )
        {
          if(left_jump_point >= right_jump_point)
          {
            range_derivative = ComSlope_Range(left_jump_point-5,0+2,l_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = 0;
          }
          else
          {
            range_derivative = ComSlope_Range(right_jump_point-5,0+2,r_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = 0;
          }
        }
        else if(left_status == 2)
        {
          if(left_jump_point >= right_jump_point)
          {
            range_derivative = ComSlope_Range(117-5,right_jump_point+2,r_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = 0;
          }
          else
          {
            range_derivative = ComSlope_Range(117-5,left_jump_point+2,l_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = 0;
          }  
        }
      }
      if(left_jump_point > right_jump_point)//跳变点间滤波
      {
        for(row=left_jump_point;row>right_jump_point;row--)
        {
          Lline[row] = 0;
          Rline[row] = 159;
        } 
      }
      else if(left_jump_point < right_jump_point)
      {
        for(row=right_jump_point;row>left_jump_point;row--)
        {
          Lline[row] = 0;
          Rline[row] = 159;
        }
      }
    }
    else if(min_jump_count == 1 || min_jump_count == 2)
    {
      if(left_status == right_status && left_status != 0)
      {
        nomal_track_flag = 0;
        if(left_status == 1)
        {
          if(l_jump_point[1] >= r_jump_point[1])
          {
            range_derivative = ComSlope_Range(r_jump_point[0]-5,r_jump_point[1]+2,r_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = r_jump_point[1];
          }
          else
          {
            range_derivative = ComSlope_Range(l_jump_point[0]-5,l_jump_point[1]+2,l_First_derivative);
            Track_complexity =  (int16)(range_derivative * range_derivative *10);
            Prospect_See = l_jump_point[1];
          }
          if(left_jump_point > right_jump_point)//跳变点间滤波
          {
            for(row=left_jump_point;row>right_jump_point;row--)
            {
              Lline[row] = 0;
              Rline[row] = 159;
            } 
          }
          else if(left_jump_point < right_jump_point)
          {
            for(row=right_jump_point;row>left_jump_point;row--)
            {
              Lline[row] = 0;
              Rline[row] = 159;
            }
          }
        }
        else if(left_status == 2)
        {
          if(left_jump_point >= right_jump_point)
          {
            range_derivative = ComSlope_Range(r_jump_point[0]-5,r_jump_point[1]+2,r_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = 0;
          }
          else
          {
            range_derivative = ComSlope_Range(l_jump_point[0]-5,l_jump_point[1]+2,l_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = 0;
          }
          if(left_jump_point > right_jump_point)//跳变点间滤波
          {
            Fjump_end = MIN(l_jump_point[1],r_jump_point[1]);
            for(row=left_jump_point;row> Fjump_end;row--)
            {
              Lline[row] = 0;
              Rline[row] = 159;
            } 
          }
          else if(right_jump_point > left_jump_point)
          {
            Fjump_end = MIN(l_jump_point[1],r_jump_point[1]);
            for(row=right_jump_point;row>Fjump_end;row--)
            {
              Lline[row] = 0;
              Rline[row] = 159;
            }
          }
        }
        if(left_status == 1)//前丢线后弯道
        {
          //十字后端补线
          left_jump_point1 = l_jump_point[1];
          right_jump_point1 = r_jump_point[1];
          if(left_jump_point1)
          {
            for(row = left_jump_point1;row>=0;row--)
            {
              Lline[row] = 0;
            }
          }
          if(right_jump_point1)
          {
            for(row = right_jump_point1; row >= 0 ;row--)
            {
              Rline[row] = 159;
            }
          }
          if(left_jump_point1 && Lline[left_jump_point1+1] != 0 && Lline[left_jump_point1+2] !=0)
          {
            l_jump_derivative = Lline[left_jump_point1+1] - Lline[left_jump_point1+2];
            for(row = left_jump_point1;row>=0;row--)
            {
              Lline[row] = Lline[row+1] + l_jump_derivative;
            }
          }
          if(right_jump_point1 && Rline[right_jump_point1+1] !=159 && Rline[right_jump_point1+2] !=159)
          {
            r_jump_derivative = Rline[right_jump_point1+1] - Rline[right_jump_point1+2];
            for(row = right_jump_point1;row>=0;row--)
            {
              Rline[row] = Rline[row+1] + r_jump_derivative;
            }
          }//后端补线
        }
      }
    }
    else if(min_jump_count > 2)
    {
      if(left_status == 2 && right_status == 2)
      {
        nomal_track_flag = 0;
        if(min_jump_count == 3)
        {
          if(l_jump_point[2] >= r_jump_point[2])
          {
            range_derivative = ComSlope_Range(l_jump_point[1]-5,l_jump_point[2]+2,l_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = r_jump_point[2];
          }
          else 
          {
            range_derivative = ComSlope_Range(r_jump_point[1]-5,r_jump_point[2]+2,r_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = l_jump_point[2];
          }
          //十字后端补线
          left_jump_point2 = l_jump_point[2];
          right_jump_point2 = r_jump_point[2];
          if(left_jump_point2)
          {
            for(row = left_jump_point2;row>=0;row--)
            {
              Lline[row] = 0;
            }
          }
          if(right_jump_point2)
          {
            for(row = right_jump_point2; row >= 0 ;row--)
            {
              Rline[row] = 159;
            }
          }
          if(left_jump_point2 && Lline[left_jump_point2+1] != 0 && Lline[left_jump_point2+2] !=0)
          {
            l_jump_derivative = Lline[left_jump_point2+1] - Lline[left_jump_point2+2];
            for(row = left_jump_point2;row>=0;row--)
            {
              Lline[row] = Lline[row+1] + l_jump_derivative;
            }
          }
          if(right_jump_point2 && Rline[right_jump_point2+1] !=159 && Rline[right_jump_point2+2] !=159)
          {
            r_jump_derivative = Rline[right_jump_point2+1] - Rline[right_jump_point2+2];
            for(row = right_jump_point2;row>=0;row--)
            {
              Rline[row] = Rline[row+1] + r_jump_derivative;
            }
          }//后端补线
        }
        
      }
      if(left_jump_point > right_jump_point)//跳变点间滤波
      {
        Fjump_end = MIN(l_jump_point[1],r_jump_point[1]);
        for(row=left_jump_point;row>Fjump_end;row--)
        {
          Lline[row] = 0;
          Rline[row] = 159;
        }
      }
      else if(right_jump_point > left_jump_point)
      {
        Fjump_end = MIN(l_jump_point[1],r_jump_point[1]);
        for(row=right_jump_point;row>Fjump_end;row--)
        {
          Lline[row] = 0;
          Rline[row] = 159;
        }
      }
    }
  }
  
  if(nomal_track_flag  == 1 && (left_jump_point < 60 && right_jump_point <60))  //如果为正常赛道（弯道）则利用赛道边缘最后一个点的斜率对赛道进行补线
  { 
    if(left_jump_point)
    {
      for(row = left_jump_point;row>=0;row--)
      {
        Lline[row] = 0;
      }
    }
    if(right_jump_point)
    {
      for(row = right_jump_point; row >= 0 ;row--)
      {
        Rline[row] = 159;
      }
    }
    if(left_jump_point && Lline[left_jump_point+1] != 0 && Lline[left_jump_point+2] !=0)
    {
      l_jump_derivative = Lline[left_jump_point+5] - Lline[left_jump_point+9];
      for(row = left_jump_point;row>=0;row--)
      {
        Lline[row] = Lline[row+1] + l_jump_derivative;
      }
    }
    if(right_jump_point && Rline[right_jump_point+1] !=159 && Rline[right_jump_point+2] !=159)
    {
      r_jump_derivative = Rline[right_jump_point+5] - Rline[right_jump_point+9];
      for(row = right_jump_point;row>=0;row--)
      {
        Rline[row] = Rline[row+1] + r_jump_derivative;
      }
    }
  }
  
  //弯道近端补线
  else if(nomal_track_flag==1 && (left_jump_point>=60 && Lline[left_jump_point+2]>80 && MMN(Rline,left_jump_point,119,0)==159)||(right_jump_point>=60 && Rline[right_jump_point+2]<80 && MMN(Lline,right_jump_point,119,1)==0))
  {
    if(left_jump_point)
    {
      for(row = left_jump_point;row>=0;row--)
      {
        Lline[row] = 0;
      }
    }
    if(right_jump_point)
    {
      for(row = right_jump_point; row >= 0 ;row--)
      {
        Rline[row] = 159;
      }
    }
    if(left_jump_point && Lline[left_jump_point+1] != 0 && Lline[left_jump_point+2] !=0)
    {
      l_jump_derivative = Lline[left_jump_point+5] - Lline[left_jump_point+9];
      for(row = left_jump_point;row>=0;row--)
      {
        Lline[row] = Lline[row+1] + l_jump_derivative;
      }
    }
    if(right_jump_point && Rline[right_jump_point+1] !=159 && Rline[right_jump_point+2] !=159)
    {
      r_jump_derivative = Rline[right_jump_point+5] - Rline[right_jump_point+9];
      for(row = right_jump_point;row>=0;row--)
      {
        Rline[row] = Rline[row+1] + r_jump_derivative;
      }
    }
  }
  
  for(row = 119 ; row >= 0; row --)
  {
    if(Lline[row] !=0 && Rline[row] != 159)
    {
      Mline[row] = (Lline[row] + Rline[row])/2.0 ;
    } 
    if(Lline[row] != 0 && Rline[row] == 159)
    {
      Mline[row] = (Lline[row] + 72.0);
    }
    if(Lline[row] ==0 && Rline[row] != 159)
    {
      Mline[row] = (Rline[row] - 72.0);
    }
  }
  
  Imgfilter_Mline(Mline);
  
  if(nomal_track_flag == 1)
  {  
    if(left_jump_point || right_jump_point)
    {
      if(left_jump_point > right_jump_point)
      {
        range_derivative = Slope_Range(left_jump_point,l_First_derivative);
        Track_complexity =  (int16)(range_derivative * range_derivative *10);
      }
      else
      {
        range_derivative = Slope_Range(right_jump_point,r_First_derivative);
        Track_complexity =   (int16)(range_derivative * range_derivative *10);
      }
    }
    else 
    {      
      range_derivative =  ABS(Slope_Range(0,l_First_derivative) - Slope_Range(0,r_First_derivative));
      Track_complexity  =  (int16)(range_derivative * range_derivative *10);
    }
    if(left_jump_point && right_jump_point)
    {
      Prospect_See = MIN(left_jump_point,right_jump_point);
    }
    else
    {
      if(left_jump_point || right_jump_point)
        Prospect_See = MAX(left_jump_point,right_jump_point);
      else
        Prospect_See = 0;
    }
  }
  camer_error_calculation();
}


/*********************************************************** 
* @author： 沙艺已觉
* @date : 2016/2/24
* @fuction name： 
* @fuction description：计算偏差
***********************************************************/
void camer_error_calculation()
{
  int8 i;
  float err = 0;
  for(i = 119; i >= 34; i --)
  {
    err += (Mline[i] - 80)*Correction_array[i];
  }
  camer.error = err /85.0;

}

/*********************************************************** 
* @author： WWW
* @date : 2016/2/12
* @fuction name： 
* @fuction description： 用最小二乘法对赛道进行补线
***********************************************************/

void least_square_method(int8 *line,int8 raw_start,int8 raw_end)
{
  uint8 num = 0;
  int8 i;
  double tem=0.0;
  double sumx = 0.0,sumx2 = 0.0, 
  sumy = 0.0,sumxy = 0.0, sumx2y = 0.0;
  double a=0.0,b=0.0,c=0.0;
  for(i = raw_start;i > raw_end ; i-- )
  {
    if(line[i]!=0&&line[i]!=79)
    {       
      sumx += i;//对列点求和
      sumx2 += (i*i);//平方和
      sumy += line[i];
      sumxy += (i*line[i]);
      sumx2y += (line[i]*i*i);
      num++;
    }    
  } 
  //一阶拟合
  c=num*sumx2-sumx*sumx;
  b=(sumy*sumx2-sumxy*sumx)/c;  //截距
  a=(num*sumxy-sumx*sumy)/c;  //斜率  
  
  for(i = raw_start;i>raw_end+1;i--)
  {
    if(line[i]==0||line[i]==79)
    {   
      tem = a*i+b;
      if(tem>0&&tem<79)
        line[i]=(uint8)tem;
    }
  }  
}

/*********************************************************** 
* @author： 沙艺已觉
* @date : 2016/2/24
* @fuction name： 
* @fuction description：计算斜率平均值
***********************************************************/
float calculation_average(float a,float b, float c)
{
  float average_value;
  average_value = (a + b + c)/3;
  return average_value;
}
/*********************************************************** 
* @author： 沙艺已觉
* @date : 2016/2/24
* @fuction name： 
* @fuction description：计算斜率方差
***********************************************************/
float calculation_variance(float a,float b, float c,float average)
{
  float variance;
  variance = ((a - average)*(a - average)+(b - average)*(b - average)+(c - average)*(c - average))/3;
  return variance;
}
/*********************************************************** 
* @author： 沙艺已觉
* @date : 2016/2/24
* @fuction name： 
* @fuction description：计算绝对值
***********************************************************/
float calculation_abs(float x)
{
  if(x >= 0)
    x = x;
  else
    x = -x; 
  return x;   
}

/*********************************************************************
* @author： 廖波
* @date : 2016/4/11
* @fuction name： 
* @fuction description：中位值滤波 对寻线得到的赛道边缘进行滤波 
*********************************************************************/
void ImgFilter(int16 *line)
{
  int8 i,j,k;
  int8 m,n;
  int16 num[5],t;
  for(i=117;i>1;i--)
  {
    k = i+2;
    for(j=0;j<5;j++,k--)
      num[j] = line[k];
    for(m=0;m<5;m++)
    {
      for(n=0;n<4-m;n++)
      {
        if(num[n]>num[n+1])
        {
          t=num[n];
          num[n]=num[n+1];
          num[n+1]=t;
        }
      }
    }
    line[i] = num[2];
  }
}

/*********************************************************** 
* @author： 廖波
* @date : 2016/4/11
* @fuction name： 
* @fuction description：对得到的赛道中线进行滤波
***********************************************************/
void Imgfilter_Mline(float *line)
{
  int8 i,j,k;
  int8 m,n;
  float num[11],t;
  for(i=114;i>4;i--)
  {
    k = i+5;
    for(j=0;j<11;j++,k--)
      num[j] = line[k];
    for(m=0;m<11;m++)
    {
      for(n=0;n<10-m;n++)
      {
        if(num[n]>num[n+1])
        {
          t=num[n];
          num[n]=num[n+1];
          num[n+1]=t;
        }
      }
    }
    line[i] = num[5];
  }
}


/*********************************************************** 
* @author： 廖波
* @date : 2016/4/11
* @fuction name： 
* @fuction description：对采集回来的赛道边缘选取一定的位置进行滤波
***********************************************************/
void ImgFilter_range(int16 start,int16 end,int16 *line)
{
  int8 i,j,k;
  int8 m,n;
  int16 num[3],t;
  for(i=start - 1;i>end;i--)
  {
    k = i+1;
    for(j=0;j<3;j++,k--)
      num[j] = line[k];
    for(m=0;m<3;m++)
    {
      for(n=0;n<2-m;n++)
      {
        if(num[n]>num[n+1])
        {
          t=num[n];
          num[n]=num[n+1];
          num[n+1]=t;
        }
      }
    }
    line[i] = num[1];
  }
}


void matchline(int16 start,int16 end,int16* line)//两点连线
{
  float a0,a1;
  int16 i,divide;
  divide = line[start]-line[end];
  if(divide != 0)
  {
    a1 = (start-end)*1.0/divide;
    a0 = (start-a1*line[start])*1.0;
    if(a1 != 0)
    {
      for(i = start-1;i > end;i --)
        line[i] = (int16)((i-a0)/a1);
    }
  }
}



float Slope_Range(int16 D,float * line)
{
  float MAX,MIN;
  int16 i;
  MAX = line[117];
  MIN = line[117];
  for(i = 116; i>= D;i--)
  {
    if(MAX < line[i])
      MAX = line[i];
    else if(MIN > line[i])
      MIN = line[i];
    
  }
  return (MAX - MIN);
}

float ComSlope_Range(int16 start,int16 end,float * line)
{
  float MAX,MIN;
  int16 i;
  MAX = line[start];
  MIN = line[start];
  for(i = start-1; i>= end;i--)
  {
    if(MAX < line[i])
      MAX = line[i];
    else if(MIN > line[i])
      MIN = line[i];
    
  }
  //    mexPrintf("MAX=%f\tMIN=%f\n",MAX,MIN);
  return (MAX - MIN);
}

void crossfit(int16* line,int8 point,uint8 d)//一阶拟合
{
    uint8 num = 0;
    int8 i;
    int8 row_start,row_end;
    double tem=0.0;
    double sumx = 0.0,sumx2 = 0.0, 
           sumy = 0.0,sumxy = 0.0, sumx2y = 0.0;
    double a=0.0,b=0.0,c=0.0;
    if(d == 1)
    {
        row_start = point+10;
        row_end = point+5;
    }
    else if(d == 2)
    {
        row_start = point-5;
        row_end = point-10;
    }
    if(row_start>119)
        row_start = 119;
    if(row_end<0)
        row_end = 0;
    for(i = row_start;i > row_end ; i-- )
    {
        if(line[i]!=0&&line[i]!=159)
        {       
             sumx += i;//对列点求和
             sumx2 += (i*i);//平方和
             sumy += line[i];
             sumxy += (i*line[i]);
             sumx2y += (line[i]*i*i);
             num++;
        }    
    } 
    //一阶拟合
    c=num*sumx2-sumx*sumx;
    b=(sumy*sumx2-sumxy*sumx)/c;  //截距
    a=(num*sumxy-sumx*sumy)/c;  //斜率  
    if(d == 1)
    {
        for(i=point+5;i>=0;i--)
        {
             tem = a*i+b;
             line[i]=(uint8)tem;
        }
    }
    else if(d == 2)
    {
        for(i=point-5;i<120;i++)
        {
             tem = a*i+b;
             line[i]=(uint8)tem;
        }
    }
}

int16 sum(int16 s,int16 e,int16*line)
{
    int16 sum=0;
    int i;
    for(i=s;i>e;i--)
        sum+=line[i];
    return sum;
}

/****寻出一个数组中的最大最小点120*160****/
int16 MMN(int16 *line,uint8 s,uint8 e,uint8 choose)
{
    int16 temp;
    if(choose==1)//最大值
    {
        temp=0;
        for(;s<e+1;s++)
        {
            if(temp<line[s])
                temp = line[s];
        }
    }
    else
    {
        temp = 159;
        for(;s<e+1;s++)
        {
            if(temp>line[s])
                temp = line[s];
        }
    }
    return temp;
}