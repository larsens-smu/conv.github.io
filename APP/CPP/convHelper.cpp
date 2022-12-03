#include "convHelper.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

float ConvHelper::toFloats(void* imgVoid, unsigned int length, void* answerVoid){

    unsigned char * img = (unsigned char *) imgVoid;
    float * answer = ( float *) answerVoid;
    for(int i = 0; i < length; i++){
        answer[i] = ((float)img[i])/255.0;
    }
    return 0.0;
}

float ConvHelper::fromFloats(void * imgVoid, unsigned int length, void* answerVoid){

    float * img = (float *) imgVoid;
    unsigned char * answer = (unsigned char *) answerVoid;
    for(int i = 0; i < length; i++){
        answer[i] = (unsigned char)(img[i]*255.0);
    }
    return 0.0;
}
ConvHelper::ConvHelper(){
    this->sharpen3 = new float[9]
    {   
        -1.0,  -1.0,  -1.0,
        -1.0,  9.0,  -1.0,
        -1.0,  -1.0,  -1.0
    };
    this->sharpen5 = new float[25]
    {   
        -1.0,  -1.0,  -1.0, -1.0, -1.0,
        -1.0,  -1.0,  -1.0, -1.0, -1.0,
        -1.0,  -1.0,  25.0, -1.0, -1.0,
        -1.0,  -1.0,  -1.0, -1.0, -1.0,
        -1.0,  -1.0,  -1.0, -1.0, -1.0,
    };
    this->embossVertical3 = new float[9]
    {   
        0.0,  1.0,  0.0,
        0.0,  0.0,  0.0,
        0.0,  -1.0,  0.0
    };
    this->embossVertical5 = new float[25]
    {   
        0.0,  0.0, 1.0,  0.0, 0.0,
        0.0,  0.0, 1.0,  0.0, 0.0,
        0.0,  0.0, 0.0,  0.0, 0.0,
        0.0,  0.0, -1.0, 0.0, 0.0,
        0.0,  0.0, -1.0, 0.0, 0.0
    };
    this->embossHoriztonal3 = new float[9]
    {   
        0.0,  0.0,  0.0,
        1.0,  0.0,  -1.0,
        0.0,  0.0,  0.0
    };
    this->embossHoriztonal5= new float[25]
    {   
        0.0,  0.0,  0.0, 0.0,   0.0,
        0.0,  0.0,  0.0, 0.0,   0.0, 
        1.0,  1.0,  0.0, -1.0,-1.0,
        0.0,  0.0,  0.0, 0.0,  0.0,
        0.0,  0.0,  0.0, 0.0,  0.0
    };
    this->gausianBlur3 = new float[9] 
    {   1.0/16.0,  2.0/16.0,  1.0/16.0,
        2.0/16.0,  4.0/16.0,  2.0/16.0,
        1.0/16.0,  2.0/16.0,  1.0/16.0
    };
    this->gausianBlur5 = new float[25] 
    {   
        1.0/256.0,  4.0/256.0,  6.0/256.0,  4.0/256.0,  1.0/256.0,
        4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0,  4.0/256.0,
        6.0/256.0, 24.0/256.0, 36.0/256.0, 24.0/256.0,  6.0/256.0,
        4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0,  4.0/256.0,
        1.0/256.0,  4.0/256.0,  6.0/256.0,  4.0/256.0,  1.0/256.0
    };
    this->laplaucian3 = new float[9] 
     {   
        0.0,  1.0,  0.0,
        1.0,  -3.0, 1.0,
        0.0,  1.0,  0.0
    };
    this->laplaucian5 = new float[25] 
    {   
        0.0,  0.0, 1.0,  0.0,  0.0,
        0.0,  1.0, 2.0,  1.0,  0.0,
        1.0,  2.0, -15.0, 2.0, 1.0, 
        0.0,  1.0, 2.0,  1.0,  0.0,
        0.0,  0.0, 1.0,  0.0,  0.0,
    };
    this->horizonzontalEdge3 = new float[9] 
     {   
        1.0,  2.0,  1.0,
        0.0,  0.0,  0.0,
        -1.0,-2.0, -1.0,
    };
    this->horizonzontalEdge5 = new float[25] 
    {   
        1.0,  4.0,  6.0,  4.0,  1.0,
        2.0,  8.0, 12.0,  8.0,  4.0,
        0.0,  0.0,  0.0,  0.0,  0.0, 
       -2.0, -8.0,-12.0, -8.0, -4.0,
       -1.0, -4.0, -6.0, -4.0, -1.0,
    };
    this->verticalEdge3 = new float[9] 
     {   
        1.0,  0.0, -1.0,
        2.0,  0.0, -2.0,
        1.0,  0.0, -1.0,
    };
    this->verticalEdge5 = new float[25] 
    {   
        1.0,  2.0,  0.0, -2.0,  -4.0,
        4.0,  8.0,  0.0, -8.0,  -6.0,
        6.0, 12.0,  0.0, -12.0, -6.0, 
        4.0,  8.0,  0.0, -8.0,  -4.0,
        1.0,  2.0,  0.0, -2.0,  -1.0,
    };    
    this->sharpen2type3 = new float[9] 
     {   
        0.0,  -1.0,  0.0,
       -1.0,   5.0, -1.0,
        0.0,  -1.0,  0.0,
    };
    this->sharpen2type5 = new float[25] 
    {   
        0.0,  0.0, -1.0,  0.0,   0.0,
        0.0,  0.0, -4.0,  0.0,   0.0,
       -1.0, -4.0, 21.0, -4.0,  -1.0, 
        0.5,  0.0, -4.0,  0.0,   0.0,
        0.0,  0.0, -1.0,  0.0,   0.0,
    };
}
ConvHelper::~ConvHelper(){
    // delete[] this->sharpen3;
    // delete[] this->embossHoriztonal3;
    // delete[] this->embossVertical3;
    // delete[] this->gausianBlur3;
    // delete[] this->gausianBlur5;
    // delete[] this->laplaucian3;
}