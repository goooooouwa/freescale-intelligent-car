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
#define IMAGE_TO_RACE_FACTOR 1 //1.12
#define BAD_POINT 10000
#define ARC 57.2957795
#define MAX_OFFSET 3

int steer_angle = 0;
float average_offset = 0;
float offset[HORIZONTAL_PIXEL_NUM]={0};
float pure_offset[HORIZONTAL_PIXEL_NUM]={0};
int edge_line[HORIZONTAL_PIXEL_NUM]={0};
int offline_counter = 0;
int deflection_counter = 0;



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

    
    //Step 1: 从摄像头获取的赛道图像的下方，向上寻找黑色赛道边界。非边界点为－1
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
    //Sample data: 40 38 38 20 4 -1 -1 -1 -1 -1 6 14 20 25
 

    //Step 2: 求边界点序列中每相邻两点竖直方向的偏移量。无效点为BAD_POINT
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
    //Sample data: X -2 0 -18 -16 X X X X X X 8 6 5
    
    //Step 3: 将所有有效竖向偏移量（去掉BAD_POINT和0）转换为横向偏移量pure_offset，并求和sum。横向偏移量才是赛车需要的角度调整依据。
    //printf("\nhorizontal offset and sum:");
    for(j=0;j<HORIZONTAL_PIXEL_NUM;j++){
        if((offset[j]!= BAD_POINT)&&(offset[j] != 0)){
            //竖向偏移量为0的点不管
            pure_offset[offset_counter] = 1 / offset[j]; 
            sum += pure_offset[offset_counter];
            offset_counter++;
            //printf(" %f (%f) ", offset[j], sum);
        }
    }
    //Sample data:   -2   -18 -16             8 6 5
    
    //Step 4:根据仅含正负横向偏移量的序列pure_offset，判断前方路况，最终求出平均偏移量
    //各种路况的典型数据:
    //直线 ---+++
    //右转弯 ------
    //左转弯 ++++++
    //十字路口 +++---
    //出界 -+--+-++-+
    
    //最先检测是否出界
    /*
    for(j=0;j<offset_counter-1;j++){
	//记录正负交界的次数来判断是否出界
        if(pure_offset[j] * pure_offset[j+1] < 0) deflection_counter++;
	      if(deflection_counter > 10){
		    //与上次转弯同方向，打死急转
	         	if(last_turn > 0) return average_offset = 0.5;
	        	if(last_turn < 0) return average_offset = -0.5;
	      } 
    }  */
    
        //检测是否压线 
    for(j=0,offline_counter=0;j<20;j++){
	      if(edge_line[j] == SAMPLE_COL-1) offline_counter++;
        if(offline_counter > 14) return average_offset = -3;
    } 
    for(j=HORIZONTAL_PIXEL_NUM-20,offline_counter=0;j<HORIZONTAL_PIXEL_NUM;j++){
      	if(edge_line[j] == SAMPLE_COL-1) offline_counter++;
        if(offline_counter > 14) return average_offset = 3;
    }
    
    //检测十字路口
    for(j=0;j<offset_counter-1;j++){
        //检测是否为十字路口（检测出界时已排除多个转折的可能）
        if((pure_offset[j]>0)&&(pure_offset[j+1]<0)){
            //判断哪一条是目标路径（横向偏移量绝对值较小的为目标路径，即前进的直线）
            if(pure_offset[j] - (-1*pure_offset[j+1]) < 0) return average_offset = pure_offset[j];
            else return average_offset = pure_offset[j+1];
        }
    }

    //其他路况均通过求平均值得出平均偏移量值
    average_offset = sum / offset_counter ;
    return average_offset;
}                         


int steerAngle(float average_offset){
    float race_offset = 0; 
    float steer_angle = 0;
    race_offset = IMAGE_TO_RACE_FACTOR * average_offset;
	//caculate angle with math function arctan
	if(race_offset < 0) steer_angle = ARC * (atan(-1 * race_offset));
	else  steer_angle = ARC * (-1 * atan(race_offset));
    //printf("\naverage_offset:%f,steer_angle:%f\n",average_offset,steer_angle);    
    return (int)steer_angle;
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



