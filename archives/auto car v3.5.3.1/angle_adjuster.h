//
//  angle_adjuster.h
//  
//
//  Created by ???è°∫?è? on 3/27/12.
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
#define MAX_ANGLE 50
#define MID_MAX_ANGLE 30
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
    //直线 |steer_angle| < MIN_ANGLE 
    if((steer_angle<MIN_ANGLE)&&(steer_angle>-MIN_ANGLE)){
        //高速直线
        PWMDTY01=35;
        //PWMDTY67 = (152+steer_angle)*5;  
        PWMDTY67 = (152+ 0 )*5; 
        //PORTB=~0X01;
    }
    
    //小缓弯 MIN_ANGLE <= |steer_angle| < MID_MIN_ANGLE
    if((steer_angle<MID_MIN_ANGLE && steer_angle>=MIN_ANGLE)||(steer_angle<=-MIN_ANGLE && steer_angle>-MID_MIN_ANGLE)){
        //中速过弯
        PWMDTY01=45; 
        PWMDTY67 = (152+(int)(0.6*(float)steer_angle))*5;  
        PORTB=~0X03;  
    }
    //大缓弯 MID_MIN_ANGLE <= |steer_angle| < MID_ANGLE
        if((steer_angle<MID_ANGLE && steer_angle>=MID_MIN_ANGLE)||(steer_angle<=-MID_MIN_ANGLE && steer_angle>-MID_ANGLE)){
        //低速过弯
        PWMDTY01=25; 
        PWMDTY67 = (152+(int)(0.5*(float)steer_angle))*5;
        //PWMDTY67 = (152+steer_angle)*5;    
        PORTB=~0Xc0; 
    }
    //小弯 MID_ANGLE <= |steer_angle| < MID_MAX_ANGLE
    if((steer_angle<MID_MAX_ANGLE && steer_angle>=MID_ANGLE)||(steer_angle<=-MID_ANGLE && steer_angle>-MID_MAX_ANGLE)){
        //急弯转角大，车子很难前进，加大马力避免停滞
        PWMDTY01=40;
        PWMDTY67 = (152+(int)(1.1*(float)steer_angle))*5;  
        PORTB=~0X0f;
    }
    //大弯 MID_MAX_ANGLE <= |steer_angle| < MAX_ANGLE
    if((steer_angle<MAX_ANGLE && steer_angle>=MID_MAX_ANGLE)||(steer_angle<=-MID_MAX_ANGLE && steer_angle>-MAX_ANGLE)){
        //急弯转角大，车子很难前进，加大马力避免停滞
        PWMDTY01=50;
        PWMDTY67 = (152+(int)(1.4*(float)steer_angle))*5;  
        PORTB=~0Xf0;   
    }
    //弯道过急 |steer_angle| >= MAX_ANGLE
    if(steer_angle>=MAX_ANGLE){
        //转角超过车子极限，限制其等于极限值
        PWMDTY01=25;
        PWMDTY67 = (152+MAX_ANGLE)*5;  
        PORTB=~0Xaa;
    }
    if(steer_angle<=-MAX_ANGLE){
        PWMDTY01=25;
        PWMDTY67 = (152+(-MAX_ANGLE))*5;  
        PORTB=~0Xaa;
    }    
     
}

#endif



