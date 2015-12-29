//
//  angle_adjuster.h
//  
//
//  Created by ???�����?��? on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
// input : int steer_angle
// output: word PWMDTY67

#ifndef _angle_adjuster_h
#define _angle_adjuster_h

//HEADERS
#include "init_system.h"
#include "angle_controller.h"

//SETTINGS
#define MAX_ANGLE 55
#define MID_ANGLE 22
#define MID_MIN_ANGLE 18
#define MIN_ANGLE 10 
//PROTOTYPES
void InitPWM(void);
int angleController(void);
void angleAdjuster(int steer_angle);


//UNIT TEST
/*
 int main(void){
    //put your unit test code here...
     InitPWM();
     int steer_angle = angleController();
     angleAdjuster(steer_angle);
 }
 */

//DECLARATIONS
void angleAdjuster(int steer_angle){
    //ֱ�� |steer_angle| < MIN_ANGLE 
    if((steer_angle<MIN_ANGLE)&&(steer_angle>-MIN_ANGLE)){
        //����ֱ��
        PWMDTY01=35;
        //30;
        PWMDTY67 = (152+ 0 )*5;  
    }
    
    //С���� MIN_ANGLE <= |steer_angle| < MID_MIN_ANGLE
    if((steer_angle<MID_MIN_ANGLE && steer_angle>=MIN_ANGLE)||(steer_angle<=-MIN_ANGLE && steer_angle>-MID_MIN_ANGLE)){
        //���ٹ���
        PWMDTY01=40; 
        PWMDTY67 = (152+(int)(0.6*(float)steer_angle))*5;  
        //PWMDTY67 = (int)((152+1.8*(float)steer_angle)*5);  
    }
    //���� MID_MIN_ANGLE <= |steer_angle| < MID_ANGLE
        if((steer_angle<MID_ANGLE && steer_angle>=MID_MIN_ANGLE)||(steer_angle<=-MID_MIN_ANGLE && steer_angle>-MID_ANGLE)){
        //���ٹ���
        PWMDTY01=20; 
        PWMDTY67 = (152+(int)(0.3*(float)steer_angle))*5;  
        //PWMDTY67 = (int)((152+1.8*(float)steer_angle)*5);  
    }
    //���� MID_ANGLE <= |steer_angle| < MAX_ANGLE
    if((steer_angle<MAX_ANGLE && steer_angle>=MID_ANGLE)||(steer_angle<=-MID_ANGLE && steer_angle>-MAX_ANGLE)){
        //����ת�Ǵ󣬳��Ӻ���ǰ�����Ӵ���������ͣ��
        PWMDTY01=40;
        PWMDTY67 = (152+steer_angle)*5;  
    }
    //������� |steer_angle| >= MAX_ANGLE
    if(steer_angle>=MAX_ANGLE){
        //ת�ǳ������Ӽ��ޣ���������ڼ���ֵ
        PWMDTY01=35;
        PWMDTY67 = (152+MAX_ANGLE)*5;  
    }
    if(steer_angle<=-MAX_ANGLE){
        PWMDTY01=35;
        PWMDTY67 = (152+(-MAX_ANGLE))*5;  
    }    
     
}

#endif



