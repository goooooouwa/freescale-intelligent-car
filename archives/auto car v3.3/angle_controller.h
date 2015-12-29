//
//  main.c
//  angle controller
//
//  Created by ???è°∫?è? on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
// input : unsigned char AD_Sample[SAMPLE_COL][HORIZONTAL_PIXEL_NUM]
// output: int steer_angle

#ifndef _angle_controller_h
#define _angle_controller_h

//HEADERS
//#include <stdio.h>
#include <math.h>
#include "video_signal_sampling.h"
//optional
//#include "simulator.h"


//SETTINGS
#define IMAGE_TO_RACE_FACTOR 1
#define BAD_POINT 10000
#define ARC 57.2957795
#define MAX_OFFSET 3

int steer_angle = 0;
float average_offset = 0;
float offset[HORIZONTAL_PIXEL_NUM]={0};
int edge_line[HORIZONTAL_PIXEL_NUM]={0};



//PROTOTYPES

//int initAD_Sample(int a);
int angleController(void);
float offsetFinder(void);
int steerAngle(float average_offset);

//UNIT TEST
/*
 void main(void){
 //put your unit test code here...
 int k=0;
 //initAD_Sample(4);
 
 while(1){
 steer_angle = angleController(); 
 
 k++;    
 }
 
 
 }
 */

//DECLARATIONS
int angleController(void){ 
    int steer_angle =0;
    float average_offset = 0;
    average_offset = offsetFinder();   
    steer_angle = steerAngle(average_offset);
    return steer_angle;
}

float offsetFinder(void){
    int i,j;
    int offset_counter = 0;
    int bottom_edge_counter = 0;
    float sum = 0;
    
    //从摄像头获取的赛道图像的下方，向上寻找黑色赛道边界
    //printf("\nedge_line:");
    for(j=0;j<HORIZONTAL_PIXEL_NUM;j++){
        for(i=0;i<SAMPLE_COL;i++){
            if(AD_Sample[SAMPLE_COL-1-i][j] == 1){
                edge_line[j] = SAMPLE_COL-1-i;
                //如果边界点位于赛道图像下半部分，则bottom_edge_counter加一
                if(2*edge_line[j] > VERTICAL_PIXEL_NUM)bottom_edge_counter++;
                break;
            }
            //如果未找到黑色赛道边界，就赋值－1
            if((i==SAMPLE_COL-1)&&(AD_Sample[SAMPLE_COL-1-i][j] == 0))edge_line[j] = -1;
        }
        //printf(" %i ",edge_line[j]);
    }
    
    //当黑色边界一半以上位于赛道图像下半部分时，说明赛车快出界了，需要往一个方向打死，转急弯回轨道。
    /*if(2*bottom_edge_counter > HORIZONTAL_PIXEL_NUM) {
	//根据上一次平均偏移量的正负取急转弯方向
        if(average_offset > 0)average_offset = MAX_OFFSET;
        else average_offset = - MAX_OFFSET;
        
        return average_offset;
    }*/
    //求边界点序列中每相邻两点竖直方向的偏移量
    //printf("\nvert offset:");
    for(j=0;j<HORIZONTAL_PIXEL_NUM;j++){
        if(edge_line[j] != -1){            
            if((j != 0)&&(edge_line[j-1] != -1 )) offset[j] = edge_line[j] - edge_line[j-1];
            
            //每段横向坐标连续的边界点序列的第一个点的偏移量无效
            else offset[j] = BAD_POINT;
        }
        //未找到黑色赛道边界的点无效
        else offset[j] = BAD_POINT;
        //printf(" %f ",offset[j]);
    }
    //offset[HORIZONTAL_PIXEL_NUM]={X,-1,-2,-4,-5...-8,X,X,XX,XX,X,X,X,5,4,3,2,1}
    
    //将所有有效偏移量（竖向偏移量）转换为横向偏移量，并求和。横向偏移量才是赛车需要的角度调整依据。
    //printf("\nhorizontal offset and sum:");
    for(j=0;j<HORIZONTAL_PIXEL_NUM;j++){
        if((offset[j]!= BAD_POINT)&&(offset[j] != 0)){
            //竖向偏移量为0的点不管
            offset[j] = 1 / offset[j]; 
            sum += offset[j];
            offset_counter++;
            //printf(" %f (%f) ", offset[j], sum);
        }
    }
    
    //所有有效偏移量（已转换为横向偏移量）的平均偏移量值
    average_offset = sum / offset_counter ;
    return average_offset;
}


int steerAngle(float average_offset){
    //float race_offset = 0; 
    float steer_angle = 0;
	//caculate angle with math function arctan
	if(average_offset < 0) steer_angle = ARC * (atan(-1 * average_offset));
	else  steer_angle = ARC * (-1 * atan(average_offset));
    //printf("\naverage_offset:%f,steer_angle:%f\n",average_offset,steer_angle);
    return (int)(IMAGE_TO_RACE_FACTOR * steer_angle);
	//return (int)steer_angle;
}


#endif






/*
 float offsetFinder(void){
 //float average_offset = 0;
 float sum = 0;
 //float offset[SAMPLE_COL];
 int i, j;
 int left_edge = 0, right_edge = HORIZONTAL_PIXEL_NUM;
 //float mid_line[SAMPLE_COL];
 for(i=0;i<SAMPLE_COL;i++){
 //right edge detector
 for(j=(HORIZONTAL_PIXEL_NUM/2);j<HORIZONTAL_PIXEL_NUM;j++){
 if(AD_Sample[i][j] == 1){
 right_edge = j;
 break;
 }
 }
 //left edge detector
 for(j=(HORIZONTAL_PIXEL_NUM/2);j>=0;j--){
 if(AD_Sample[i][j] == 1){
 left_edge = j;
 break;
 }
 }
 //middle point and offset value per line
 mid_line[i] = (left_edge + right_edge)/(2.0);
 if(i == 0) offset[i] = 0;
 else offset[i] = mid_line[i] - mid_line[i-1];
 //printf("(%d,%d),(%d,%d),mid[%d]:%f,offset[%d]:%f\n",i,left_edge,i,right_edge,i,mid_line[i],i,offset[i]);
 }
 //average offset value
 for(i=0;i<SAMPLE_COL;i++){
 sum += offset[i];
 }
 average_offset = sum / (SAMPLE_COL - 1);
 //printf("average_offset:%f\n",average_offset);    
 return average_offset;
 }
 */



