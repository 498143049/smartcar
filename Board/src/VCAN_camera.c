#include "include.h"
#include "VCAN_camera.h"
#include "extern.h"

camer_status camer;

FATFS firefs;    //�ļ�ϵͳ
FIL   firesrc;  //�ļ�

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
  uint8_t colour[2] = {255, 0}; //0 �� 1 �ֱ��Ӧ����ɫ
  uint8_t * mdst = dst;
  uint8_t * msrc = src;
  //ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
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
* @author�� VCAN
* @fuction name:��img_sd_init
* @date : 2016/1/18
* @description�� SD������ͼ��
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
  while(fireres == FR_EXIST);        //����ļ����ڣ�������������1
  
  if ( fireres == FR_OK )
  {
    fireres = f_write(&firesrc, imgsize, sizeof(imgsize), (uint32 *)&mybw);  //��д��ߺͿ�������λ������
  }
  else
  {
    f_close(&firesrc);
    firesrc.fs = 0;
  }
}

/*********************************************************** 
* @author�� VCAN
* @fuction name: img_sd_save
* @date : 2016/1/18
* @fuction description�� SD������ͼ��
***********************************************************/

void img_sd_save(uint8 * imgaddr)
{
#define SD_ONEWRITE_MAX 600   //�ɿ⣨5.0֮ǰ�ģ�һ��д��̫�������¿��޸���bug�������¿��ֵ��ȡ�Ƚϴ�
  
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
* @author�� ɳ���Ѿ�
* @date : 2016/3/5
* @fuction name�� Get_Edge
* @fuction description�� �ɼ�������Ե
***********************************************************/

void Get_Edge(void)
{    
  int16 i,j;    //ѭ������  
  uint8 Barrior_Flag=0;
  uint8 nomal_track_flag = 1; //��������
  uint8 l_complete_lost_flag = 0 ;  //������ȫ���߱�־
  uint8 r_complete_lost_flag = 0;   //������ȫ��ʧ��־
  uint8 left_status = 0;    // ����״̬  �Ϻ��°�Ϊ1  �ϰ��º�Ϊ2 
  uint8 right_status = 0;   // ����״̬  �Ϻ��°�Ϊ1  �ϰ��º�Ϊ2
  int8 left_jump_point = 0;
  int8 right_jump_point = 0; 
  int8 l_jump_point[5] = {0};
  int8 r_jump_point[5] = {0};
  int8 left_jump_point1=0,right_jump_point1=0;
  int8 left_jump_point2=0,right_jump_point2=0;
  int8 Fjump_end;             //������˲���ֹ��
  int8 l_jump_count = 0;
  int8 r_jump_count = 0;
  int8 min_jump_count;
  int8 l_cross_turn_point = 0;            //��бʮ�ֹյ�λ��
  int8 r_cross_turn_point = 0;            //��бʮ�ֹյ�λ��
  uint8 l_diagonally_flag =1 ;     //��б��ʮ��ǰΪֱ��
  uint8 r_diagonally_flag =1;      //��б��ʮ��ǰΪֱ��
  float l_First_derivative[118] = {0};   //����һ�׵�
  float r_First_derivative[118] = {0};   //����һ�׵�
  float l_average[116] = {0};            //����ƽ��ֵ
  float r_average[116] = {0};            //����ƽ��ֵ
  float l_variance[116] = {0};           //���߷���
  float r_variance[116] = {0};           //���߷���
  int16 leftline_start = L_LINE_START ;  //�����Ѱ�����
  int16 rightline_start = R_LINE_START ; //�ұ���Ѱ����� 
  int8 l_jump_derivative ; 
  int8 r_jump_derivative ;
  float range_derivative;
  int8 row ;   //Ѱ���б���
  
  //Ѱ��
  for(row = 0 ; row <120 ; row ++)
  {
    Lline[row] = L_LINE_INITIAL_VALUE ; 
    Rline[row] = R_LINE_INITIAL_VALUE ;
    Mline[row] = M_LINE_INITIAL_VALUE ;           
  }  
  //Ѱ��
  for(row = 119 ; row >=0 ;row --)
  {
    //Ѱ���� 
    if( l_complete_lost_flag == 0)
    {
      for( i = leftline_start ; i>0 ; i--)
      { 
        if(leftline_start > 162)   //�������Ѱ�����ұ�                       
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
    //    Ѱ����
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
  //��
  for(row = 117;row>=0;row--)
  {
    l_First_derivative[row] = (Lline[row+2] - Lline[row])/2.0;
    r_First_derivative[row] = (Rline[row+2] - Rline[row])/2.0;
  }
  
  //������ж�
  for(row = 115; row>=0;row--)
  {
    l_average[row] = calculation_average(l_First_derivative[row+2],l_First_derivative[row+1],l_First_derivative[row]);   //����ƽ��ֵ
    
    l_variance[row] = calculation_variance(l_First_derivative[row+2],l_First_derivative[row+1],l_First_derivative[row],l_average[row]);    //���㷽��
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
    r_average[row] = calculation_average(r_First_derivative[row+2],r_First_derivative[row+1],r_First_derivative[row]);   //����ƽ��ֵ
    r_variance[row] = calculation_variance(r_First_derivative[row+2],r_First_derivative[row+1],r_First_derivative[row],r_average[row]);   //���㷽��
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
  
  //ͣ������ʶ��
  if(nomal_track_flag == 1)
  {
    //���߶���һ���յ�
       if(r_jump_count==1 && l_jump_count==1)
       {
            if(MMN(Lline,0,110,0)>0&&MMN(Rline,0,110,1)<159)
            {
                nomal_track_flag = 0;
                Prospect_See = 0;
                Track_complexity = 40;
                 //�ҹյ�
                if(left_jump_point> 10 )
                {
                  for(row = 115;row > left_jump_point - 10 ; row --)
                  {
                    if(((l_First_derivative[row+1]*l_First_derivative[row-1])<0))
                    {
                      if(l_cross_turn_point == 0)
                      {
                        l_cross_turn_point = row;
                        break;      //�ҵ��յ�����ѭ��
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
                            break;      //�ҵ��յ�����ѭ��
                          }
                        }
                     }
                }
                if(l_cross_turn_point && r_cross_turn_point && cartime >2800)
                {
                      //����
                    Blackline_flag = 1;
                }
            }
       }
  }
  
  /***�ϰ�����*****/
 if(nomal_track_flag == 1 )
 {
      if((r_jump_count==2 &&l_jump_count==0)||(l_jump_count==2&&r_jump_count==0))
      {  
        if(r_jump_count)    //�ϰ����ұ�
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
        else if(l_jump_point) //�ϰ������
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
      
      //ͣ������ʶ��
      else if((r_jump_count==1 &&l_jump_count==0)||(l_jump_count==1&&r_jump_count==0))//һ����һ���յ�
      {
          if(r_jump_count) 
          {   
                  if(MMN(Lline,0,110,0)>0&&MMN(Rline,0,110,1)<159)
                  {
                      if(r_jump_point[0] >20)
                      {
                           //�ҹյ�
                          for(row = 115;row > (right_jump_point - 10) ; row --)
                          {
                            if(((r_First_derivative[row+1]*r_First_derivative[row-1])<0))
                            { 
                              if(r_cross_turn_point == 0)
                              {
                                r_cross_turn_point = row;
                                break;      //�ҵ��յ�����ѭ��
                              }
                            }
                          }
                         // if(r_cross_turn_point != 0 && cartime > 800 && r_jump_point[0] < 50)  // �ҵ��յ�Ϊ������
                          if(r_cross_turn_point != 0 && cartime > 2800)  // �ҵ��յ�Ϊ������
                          {
//                            if(Barrior_Flag==0)
//                            {
                             Blackline_flag = 1 ;
                           // }
                          }
                          else if(r_cross_turn_point == 0 )  //�ϰ�
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
                           //�ҹյ�
                          for(row = 115;row > left_jump_point - 10 ; row --)
                          {
                            if(((l_First_derivative[row+1]*l_First_derivative[row-1])<0))
                            {
                              if(l_cross_turn_point == 0)
                              {
                                l_cross_turn_point = row;
                                break;      //�ҵ��յ�����ѭ��
                              }
                            }
                          }
                          if(l_cross_turn_point != 0 && cartime >2800)  // �ҵ��յ�Ϊ������
                         // if(r_cross_turn_point != 0 && cartime > 800 && r_jump_point[0] < 50)  // �ҵ��յ�Ϊ������
                          {
//                            if(Barrior_Flag==0)
//                            {
                              Blackline_flag = 1 ;
//                            }
                          }
                          else if(l_cross_turn_point == 0 )  //�ϰ�
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
  
  //���ʶ��
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
    //��б��ʮ��
    if(right_jump_point && (right_jump_point > left_jump_point))    //��б��ʮ�� ����������Ҵ���������
    {
      for(row = 115;row > right_jump_point ; row --)
      {
        if(((l_First_derivative[row+2]*l_First_derivative[row-2])<0))
        {
          if(l_cross_turn_point == 0)
          {
            l_cross_turn_point = row;
            break;      //�ҵ��յ�����ѭ��
          }
        }
      }
      if(l_cross_turn_point != 0)     //�ҵ��յ�
      {  
        //                      mexPrintf("l_cross_turn_point%d\n",l_cross_turn_point);
        if(l_diagonally_flag == 1)
        {
          for(row = l_cross_turn_point ; row >=right_jump_point;row--)
          {
            if(Rline[row] != 159)   //��յ㵽������    �ұ߶���
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
          for(row = 117 ; row >= l_cross_turn_point ;row --)    //�յ�֮ǰ ����б�ʳ˻�
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
            for(i = 157;i > leftline_start;i --)     //��������������
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
            /*  2016��7��8  */
            range_derivative = Slope_Range(l_cross_turn_point,l_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = 120-l_cross_turn_point;
          }
        }
      }
    }
    
// ��б��ʮ��
    if(left_jump_point && (left_jump_point > right_jump_point))   //��б��ʮ��  ����������Ҵ���������
    { 
      for(row = 115;row > left_jump_point ; row --)
      {
        if((r_First_derivative[row+2]*r_First_derivative[row-2])<0)
        {
          if(r_cross_turn_point ==0)
          {
            r_cross_turn_point = row;
            break;    //�ҵ��յ�����ѭ��
          }
        }         
      }
      if(r_cross_turn_point != 0)     //�ҵ��յ�
      {
        if(r_diagonally_flag == 1)
        {
          for(row = r_cross_turn_point ; row >=left_jump_point;row --)
          {
            if(Lline[row] != 0)   //�ҹյ㵽������    ��߶���
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
          for(row = 117 ; row >= r_cross_turn_point ;row --)    //�յ�֮ǰ ����б�ʳ˻�
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
            for(j = 2;j < rightline_start;j ++)     //��������������
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
            matchline(r_cross_turn_point+3 ,left_jump_point - 3,Rline);  //��������
            nomal_track_flag = 0 ;
            /*  2016��7��8  */
            range_derivative = Slope_Range(r_cross_turn_point,r_First_derivative);
            Track_complexity =   (int16)(range_derivative * range_derivative *10);
            Prospect_See = 120-r_cross_turn_point;
          }
        }
      }
    }
  }
  
  if(left_jump_point && right_jump_point && ABS(left_jump_point - right_jump_point)<10 && nomal_track_flag==1)       //����ʮ��
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
    if(l_jump_count==1 && r_jump_count ==1)        //���Ҷ�ֻ��һ�������
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
      if(left_jump_point > right_jump_point)//�������˲�
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
          if(left_jump_point > right_jump_point)//�������˲�
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
          if(left_jump_point > right_jump_point)//�������˲�
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
        if(left_status == 1)//ǰ���ߺ����
        {
          //ʮ�ֺ�˲���
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
          }//��˲���
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
          //ʮ�ֺ�˲���
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
          }//��˲���
        }
        
      }
      if(left_jump_point > right_jump_point)//�������˲�
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
  
  if(nomal_track_flag  == 1 && (left_jump_point < 60 && right_jump_point <60))  //���Ϊ���������������������������Ե���һ�����б�ʶ��������в���
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
  
  //������˲���
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
* @author�� ɳ���Ѿ�
* @date : 2016/2/24
* @fuction name�� 
* @fuction description������ƫ��
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
* @author�� WWW
* @date : 2016/2/12
* @fuction name�� 
* @fuction description�� ����С���˷����������в���
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
      sumx += i;//���е����
      sumx2 += (i*i);//ƽ����
      sumy += line[i];
      sumxy += (i*line[i]);
      sumx2y += (line[i]*i*i);
      num++;
    }    
  } 
  //һ�����
  c=num*sumx2-sumx*sumx;
  b=(sumy*sumx2-sumxy*sumx)/c;  //�ؾ�
  a=(num*sumxy-sumx*sumy)/c;  //б��  
  
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
* @author�� ɳ���Ѿ�
* @date : 2016/2/24
* @fuction name�� 
* @fuction description������б��ƽ��ֵ
***********************************************************/
float calculation_average(float a,float b, float c)
{
  float average_value;
  average_value = (a + b + c)/3;
  return average_value;
}
/*********************************************************** 
* @author�� ɳ���Ѿ�
* @date : 2016/2/24
* @fuction name�� 
* @fuction description������б�ʷ���
***********************************************************/
float calculation_variance(float a,float b, float c,float average)
{
  float variance;
  variance = ((a - average)*(a - average)+(b - average)*(b - average)+(c - average)*(c - average))/3;
  return variance;
}
/*********************************************************** 
* @author�� ɳ���Ѿ�
* @date : 2016/2/24
* @fuction name�� 
* @fuction description���������ֵ
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
* @author�� �β�
* @date : 2016/4/11
* @fuction name�� 
* @fuction description����λֵ�˲� ��Ѱ�ߵõ���������Ե�����˲� 
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
* @author�� �β�
* @date : 2016/4/11
* @fuction name�� 
* @fuction description���Եõ����������߽����˲�
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
* @author�� �β�
* @date : 2016/4/11
* @fuction name�� 
* @fuction description���Բɼ�������������Եѡȡһ����λ�ý����˲�
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


void matchline(int16 start,int16 end,int16* line)//��������
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

void crossfit(int16* line,int8 point,uint8 d)//һ�����
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
             sumx += i;//���е����
             sumx2 += (i*i);//ƽ����
             sumy += line[i];
             sumxy += (i*line[i]);
             sumx2y += (line[i]*i*i);
             num++;
        }    
    } 
    //һ�����
    c=num*sumx2-sumx*sumx;
    b=(sumy*sumx2-sumxy*sumx)/c;  //�ؾ�
    a=(num*sumxy-sumx*sumy)/c;  //б��  
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

/****Ѱ��һ�������е������С��120*160****/
int16 MMN(int16 *line,uint8 s,uint8 e,uint8 choose)
{
    int16 temp;
    if(choose==1)//���ֵ
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