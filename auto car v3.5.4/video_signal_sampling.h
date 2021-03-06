//
//  video_signal_sampling.h
//  
//
//  Created by �?顺发 on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
// input : bit PORTA_PA0 [VERTICAL_PIXEL_NUM][HORIZONTAL_PIXEL_NUM]
// output: unsigned char AD_SAMPLE[SAMPLE_COL][HORIZONTAL_PIXEL_NUM]

#ifndef _video_signal_sampling_h
#define _video_signal_sampling_h

//HEADERS
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "init_system.h"

//SETTINGS
#define VERTICAL_PIXEL_NUM  260
#define HORIZONTAL_PIXEL_NUM  92
#define SAMPLE_COL  76

unsigned char sample_array_col=0;
unsigned int AD_signal_col=0; //输入信号的当前行�?
unsigned int row_intr_counter=0,field_intr_counter=0;
unsigned char FieldINTR_Flag=0;//场脉冲标�?
unsigned char Row_INTR_Flag=0; //一场的行脉冲结束标�?

unsigned char AD_Sample[SAMPLE_COL][HORIZONTAL_PIXEL_NUM] = {0};
/*
unsigned char AD_Sample[SAMPLE_COL][HORIZONTAL_PIXEL_NUM] = { 
                                                {0,1,0,1,0,0,0,0,1,0},
                                                {0,0,0,0,1,1,0,0,0,0},
                                                {0,0,0,0,0,0,1,0,0,0},                                              
                                                {0,0,0,0,0,0,0,0,0,0},
                                                {0,0,0,0,0,0,0,0,0,0},
                                                {0,0,0,0,0,0,0,1,0,0},
                                                {0,0,0,0,0,0,0,0,0,0},
                                                {0,0,0,0,0,0,0,0,1,0},
                                                {0,0,0,0,0,0,0,0,0,0},
                                                {1,0,0,0,0,0,0,0,0,1}, 
                                              };
*/
/*unsigned int get_n[]={ 147,150,153,156,159,162,165,168,
                       170,172,174,176,178,180,182,184,186,188,
                       190,192,194,196,198,200,202,204,206,208,
                       210,211,212,242,243,245,246,247,248,249,250};
*/
/*unsigned int get_n[]={ 29,41,52,62,71,79,86,92,98,
                       103,108,112,116,120,124,128,132,135,138,
                       141,144,147,150,153,156,159,162,165,168,
                       170,172,174,176,178,180,182,184,186,188,
                       190,192,194,196,198,200,202,204,206,208,
                       210,211,212,242,243,245,246,247,248,249,250};
 */
 
//PROTOTYPES
void SetBusCLK(void);
void Init_Timer(void);
void Port_Init(void);
//UNIT TEST
 /*
 int main(void){
    //put your unit test code here...
     SetBusCLK();
     Init_Timer();
     Port_Init();
     EnableInterrupts;
     while(1){
         while(FieldINTR_Flag == 0);
         TIE |= 0x20;
         while (Row_INTR_Flag == 0);
         TIE |= ~0x20;
         //sample once.
     }
 }

 */
//DECLARATIONS
void delay(unsigned int num)   
 { 
    unsigned int i,j; 
   for(i=0;i<num;i++) 
      for(j=0;j<68;j++);
 }

void read_row(void){
	unsigned char pixel_counter=0;

	if((AD_signal_col >= VERTICAL_PIXEL_NUM-2*SAMPLE_COL)&&(AD_signal_col < VERTICAL_PIXEL_NUM)&&(AD_signal_col%2 == 0)) {  //定距采集图像的判断条�?
    delay(1);   //黑线中心矫正，确保不采集到消隐信�?

    for(pixel_counter=0;pixel_counter<=HORIZONTAL_PIXEL_NUM;pixel_counter++){  //读取一行的数据   
      AD_Sample[sample_array_col][pixel_counter]=PORTA_PA0; //采集数据的接�?
    }

   sample_array_col++; //二维数组行自�?
  }
}


#endif
