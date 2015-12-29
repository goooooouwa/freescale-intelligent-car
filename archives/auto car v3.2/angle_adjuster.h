//
//  angle_adjuster.h
//  
//
//  Created by Âæ?È°∫Âèë on 3/27/12.
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
        PWMDTY01=30;
        PWMDTY67 = (152+steer_angle)*5;  
    }
    
    //缓弯 15 <= |steer_angle| < 45
    if((steer_angle<45 && steer_angle>=15)||(steer_angle<=-15 && steer_angle>-45)){
        //低速过弯
        PWMDTY01=25;
        PWMDTY67 = (152+steer_angle)*5;  
        //PWMDTY67 = (int)((152+1.8*(float)steer_angle)*5);  
    }
    
    //急弯 45 <= |steer_angle| < 60
    if((steer_angle<60 && steer_angle>=45)||(steer_angle<=-45 && steer_angle>-60)){
        //急弯转角大，车子很难前进，加大马力避免停滞
        PWMDTY01=60;
        PWMDTY67 = (152+steer_angle)*5;  
    }
    
    //弯道过急 |steer_angle| >= 60
    if(steer_angle>=60){
        //转角超过车子极限，限制其等于极限值
        PWMDTY01=60;
        PWMDTY67 = (152+60)*5;  
    }
    if(steer_angle<=-60){
        PWMDTY01=60;
        PWMDTY67 = (152+(-60))*5;  
    }    
     
}

#endif



