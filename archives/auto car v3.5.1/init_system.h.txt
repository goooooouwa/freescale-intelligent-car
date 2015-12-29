//
//  init_system.h
//  
//
//  Created by �?顺发 on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
// 总线脉冲64M
// 初始化时，仅允许2�?通道，即速度和场中断 
// 舵机初始角度�?
// 电机慢速转�?

#ifndef _init_system_h
#define _init_system_h

//HEADERS
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

//SETTINGS

//PROTOTYPES
void InitSystem(void);
void SetBusCLK(void);
void InitPWM(void);
void Init_Timer(void);
void Port_Init(void);

//UNIT TEST
 /*
 void main(void){
    //put your unit test code here...
    int i,j;
    InitSystem();
    //EnableInterrupts;
    while(1){
        //从最左边转到最右边
        for(i=0;i<60;i++){
            for(j=0;j<200;j++){
                if(j == 0)PWMDTY67 = 460 + 10*(i+1);
            } 
        }
        //从最右边转到最左边
        for(i=0;i<60;i++){
            for(j=0;j<200;j++){
                if(j == 0)PWMDTY67 = 1060 - 10*(i+1);
            } 
        }
 
    }
 }
*/

//DECLARATIONS
void InitSystem(void){
    SetBusCLK();
    InitPWM();
    Init_Timer();
    Port_Init();
}

void SetBusCLK(void){   
  CLKSEL=0X00;    //disengage PLL to system
  PLLCTL_PLLON=1;   //turn on PLL
  SYNR =0xc0 | 0x07;        // 64M                
  REFDV=0x80 | 0x01; 
  POSTDIV=0x00;       //pllclock=2*osc*(1+SYNR)/(1+REFDV)=96MHz;
  _asm(nop);          //BUS CLOCK=48M
  _asm(nop);
  while(!(CRGFLG_LOCK==1));   //when pll is steady ,then use it;
  CLKSEL_PLLSEL =1;          //engage PLL to system; 
  }

void InitPWM(void){
 /*****舵机******/
  PWME    =0;    //PWM禁用
  PWMPOL  |= 0X80;  //实现 PWMPPOL7=1�?7通道对外输出波形先是高电平然后再变为低电�?
  //PWMCLK  &= 0X7F;
  PWMCLK  |= 0X80; // 选择时钟 7通道选择SB
  PWMPRCLK |= 0X44;////分频Clock A=bus clock/16=4MHz,Clock B=bus clock/16=4MHz;Bus Clk = 64MHz
  PWMSCLA = 0X04; //时钟SA是通过对PWMSCLA寄存器的设置来对A时钟进行分频而产生的。Clock SA=Clock A /(2*PWMSCLA)=500KHZ
  PWMSCLB = 0X04;  //时钟SB就是通过对PWMSCLB寄存器的设置来对B时钟进行分频而产生的�?
  PWMCAE = 0;
  PWMCTL  |= 0x80;  //通道6�?就串联为同一个输出通道
  PWMPER67  = 10000;//PWM7 用来产生舵机的pwm �?500K/10000=50HZ 
  PWMDTY67=760; //PWM7 用来产生舵机的pwm �? 应该设一个�?
  //PWMPER6  = (5000>>8);
  //PWMPER7  = (5000&0XFF);
  //PTTRR    |= 0X80;   //PWM7输出到PT7
  /********电机*********/

  PWMCLK  |= 0X0A;     //时钟pwm3选择SA与pwm1选择SB的时钟关�?  注意防止发生冲突
  PWMCTL  |= 0X30;     //01 23 级联
  PWMPER01 = 100;      // 5KHZ 需要调�?
  //PWMPER1 = 100;     //周期，此处值给的越�?控制越精�?
  PWMPER23 = 100;      //5KHZ
  //PWMPER3 = 100;     //

  PWMDTY23=100;       
  PWMDTY01=22;

  PWME    |= 0X80+0x08+0x02; 

  }

void Init_Timer(void){
  TIOS = 0X00;            // select all pins as input
  TSCR1 = 0X88;           // enable full timer function
  TCTL3 = 0X06;           // input capture edge selector  �?中断4 下降沿，�?中断5 上升�?
  TFLG1 = 0XFF;           // timer flags cleared                                             
  //TSCR1 = 0X80;

  INT_CFADDR=0xe0;                     // 0xe0 = B11100000 //E对应的中断源
  INT_CFDATA3 |=0x04;                  // timer4   field
  //自己加的一�?
  INT_CFDATA2 |=0x01;                  // timer5   row
  //PERT=0xff;

  TIE   = 0x10;  //初始化时，仅允许4通道，即场中�?
   //TIE=0X10;
  }

void Port_Init(void){
  DDRA=0;//PA输入
  }



#endif



