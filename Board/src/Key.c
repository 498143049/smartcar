#include "include.h"
#include "Key.h"

uint8 KeyValue=0;
uint8 KeyValue1=0;
uint8 KeyValue2=0;
uint8 KeyValue3=0;

void Key_Test()
{
  if(!key1)
  {
    KeyValue=1;
  }
  if(!key2)
  {
    KeyValue1=1;
  }
  if(!key3)
  {
    KeyValue2=1;
  }
   if(!key4)
  {
    KeyValue3=1;
  }
  if(1==KeyValue2)
  {
   enable_irq (PIT0_IRQn);    //允许PIT0定时器中断
   enable_irq (PIT2_IRQn); 
  }
}