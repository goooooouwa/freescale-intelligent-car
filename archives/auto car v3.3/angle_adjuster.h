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
    //直线 |steer_angle| < 15
    if((steer_angle<15)&&(steer_angle>-15)){
        //高速直线
        PWMDTY01=40;
        PWMDTY67 = (152+steer_angle)*5;  
    }
    
    //缓弯 15 <= |steer_angle| < 45
    if((steer_angle<45 && steer_angle>=15)||(steer_angle<=-15 && steer_angle>-45)){
        //低速过弯
        PWMDTY01=35;
        PWMDTY67 = (152+steer_angle)*5;  
        //PWMDTY67 = (int)((152+1.8*(float)steer_angle)*5);  
    }
    
    //急弯 45 <= |steer_angle| < MAX_ANGLE
    if((steer_angle<MAX_ANGLE && steer_angle>=45)||(steer_angle<=-45 && steer_angle>-MAX_ANGLE)){
        //急弯转角大，车子很难前进，加大马力避免停滞
        PWMDTY01=50;
        PWMDTY67 = (152+steer_angle)*5;  
    }
    
    //弯道过急 |steer_angle| >= MAX_ANGLE
    if(steer_angle>=60){
        //转角超过车子极限，限制其等于极限值
        PWMDTY01=70;
        PWMDTY67 = (152+MAX_ANGLE)*5;  
    }
    if(steer_angle<=-50){
        PWMDTY01=70;
        PWMDTY67 = (152+(-MAX_ANGLE))*5;  
    }    
     
}

#endif



