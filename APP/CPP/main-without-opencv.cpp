#include "conv.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

void printImg(float * img, int img_width, int img_length, int num_channels);
void getPosTest();
void testConv(int img_width, int img_length, int num_channels, Conv conv, float* img, float* new_img);
int main(int argc, char ** argv) {
    //print the inputs

    cout << argc << endl;
    cout << argv << endl;
    cout << endl;

    int img_width = 5;
    int img_length = 5;
    int num_channels = 3;
    float kernal[] = { 1.0/16.0,  2.0/16.0,  1.0/16.0,
                       2.0/16.0,  4.0/16.0,  2.0/16.0,
                       1.0/16.0,  2.0/16.0,  1.0/16.0};
    // float kernal[] = {0 ,  0,  0,
    //                    0,  0,  0,
    //                    0,  0,  0};
    Conv conv = Conv();
    conv.setKernal(kernal);
    float *img = new float[img_width*img_length*num_channels];
    float *new_img = new float[img_width*img_length*num_channels];

    testConv(img_width, img_length, num_channels, conv, img, new_img);

    delete []img;
    delete []new_img;


};

void printImg(float * img, int img_width, int img_length, int num_channels){
    for(int x = 0; x < img_width; x++){
        for(int y = 0; y < img_length; y++){
            printf("(");
            for(int c = 0; c < num_channels; c++)
                printf("%.2f,",img[(y * num_channels * img_width) + (x * num_channels) + c]);
            printf(") ");
        }
        cout << endl;
    }
    cout << endl;
};
void getPosTest(){
    Conv conv = Conv();

    int num_channels = 3;
    int img_width = 3;
    int img_length = 4;
    for(int y = 0; y < img_width; y++){
        for(int x = 0; x < img_width; x++){
            for(int c = 0; c < num_channels; c++){
                printf("(x, y, c)\n");
                printf("(%d, %d, %d) -> %d\n", x,y,c, conv.getPosition(x, y, c, num_channels, img_width, img_length));
            }
        }
    }
}
void testConv(int img_width, int img_length, int num_channels, Conv conv, float* img, float* new_img){
    srand((unsigned int)time(NULL));
    for(int i = 0; i < img_width*img_length*num_channels; i ++){
        img[i] = (float(rand())/float((RAND_MAX)) * 1.0);
    }
    float time = conv.applyFilter(img_width, img_length, img, num_channels, 1.0, new_img);
    printImg(img, img_width, img_length, num_channels);
    printImg(new_img, img_width, img_length, num_channels);
}