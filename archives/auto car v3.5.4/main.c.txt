//
//  main.c
//  Auto Car Simulation
//
//  Created by �?顺发 on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

//HEADERS
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "init_system.h"
#include "video_signal_sampling.h"
#include "angle_controller.h"
#include "angle_adjuster.h"

//SETTINGS
//int steer_angle=0;

//PROTOTYPES
//general
void delay(unsigned int num);

//controller
int angleController(void);
//float offsetFinder(void);
//int steerAngle(float average_offset);
//int speedController(void);

//adjuster
void angleAdjuster(int steer_angle);
//void speedAdjuster(uchar speed);

//UNIT TEST
void main(void) {
    //int speed = 0;
    //int steer_angle=0;
    InitSystem();
    EnableInterrupts;
    while(1){
    //wait till one field interrupt happens
    while(FieldINTR_Flag == 0);
        TIE |= 0x20;
    //wait till all row interrupts end
    while(Row_INTR_Flag == 0);
     TIE |= ~0x30;
     steer_angle = angleController();
     angleAdjuster(steer_angle);
     TIE |= 0x10;
     //angleAdjuster(steer_angle);
    //speed = speedController();
    } 
}

//DECLARATIONS

/******************场中�?*********************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED 
interrupt 12 void Field_INTR(void){
    TFLG1_C4F=1;   /*中断标志清除*/
    FieldINTR_Flag=1;
    AD_signal_col=0;
    sample_array_col=0; 
    field_intr_counter++;
    Row_INTR_Flag = 0;
  }
#pragma CODE_SEG DEFAULT

/******************行中�?********************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED 
interrupt 13 void Row_INTR(void){
    TFLG1_C5F=1;    /*中断标志清除*/
    read_row();
    AD_signal_col++; //摄像头行自加
    row_intr_counter++;
    if(AD_signal_col == VERTICAL_PIXEL_NUM) Row_INTR_Flag = 1;
  
}
#pragma CODE_SEG DEFAULT
