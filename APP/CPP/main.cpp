#include "convHelper.h"
#include "conv.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <sys/stat.h>
using namespace std;

void printImg(float * img, int img_width, int num_channels);
double applyToImage(Conv conv, ConvHelper helper, unsigned char* img, unsigned int length, int rows, int cols, int channels, string outfilename, int num_thread);
double filterImage(
    string filter,
    int kernalSize,
    Conv conv, 
    ConvHelper helper,
    string folder, 
    cv::Mat img,
    unsigned char * arr,
    int length, int num_thread
);
int main(int argc, char ** argv) {
    if (argc != 2 && argc != 3) {
        cout << "Expecting a image file to be passed to program" << endl;
        return -1;
    }
    
    cv::Mat img = cv::imread(argv[1]);
    if (img.empty()) {
        cout << "Not a valid image file" << endl;
        return -1;
    }

    int num_thread = 1;
    if (argc == 3){
        num_thread = atoi(argv[2]);
    }

    //set up the output folder
    // const char* folder;
    string folder_s = (string)argv[1];
    string folder(folder_s.substr(0,folder_s.length() -4));

    ConvHelper helper = ConvHelper();
    Conv conv = Conv();
    float * kernal = helper.getEmbossKernalVertical(3);
    unsigned char * arr = img.isContinuous()? img.data: img.clone().data;
    unsigned int length = img.total()*img.channels();
    double time;
    double total_time = 0;

    string filter = "embossVertical";
    int kernalSize = 3;
    time = filterImage("sharpen", 3, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("embossVertical", 3, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("embossHorizontal", 3, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("gaussianBlurKernal", 3, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("laplacianKernal", 3, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("edgeHorizontal", 3, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("edgeVertical", 3, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("sharpen2type", 3, conv, helper, folder, img, arr, length, num_thread);

    time = filterImage("sharpen", 5, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("embossVertical", 5, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("embossHorizontal", 5, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("gaussianBlurKernal", 5, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("laplacianKernal", 5, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("edgeHorizontal", 5, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("edgeVertical", 5, conv, helper, folder, img, arr, length, num_thread);
    time = filterImage("sharpen2type", 5, conv, helper, folder, img, arr, length, num_thread);

    return 0;

};
double filterImage(
    string filter,
    int kernalSize,
    Conv conv, 
    ConvHelper helper,
    string folder, 
    cv::Mat img,
    unsigned char * arr,
    int length, 
    int num_thread
){
    conv.setKernal(kernalSize, filter);
    double time = applyToImage(conv, helper,  arr, length, (int)img.rows, (int)img.cols, (int)img.channels(), 
        (string)folder + "-cpp/" + filter + to_string(kernalSize) + ".png", num_thread);
    printf("%s,%d,%d,%d,%f\n",filter.c_str(), length/3, kernalSize, num_thread, time);
    return time;
}

double applyToImage(Conv conv, ConvHelper helper, unsigned char* img, unsigned int length, int rows, int cols, int channels, string outfilename, int num_thread){

    float * img_float = new float[length];
    float * conv_img_float = new float[length];
    unsigned char* new_img_char = new unsigned char[length];

    float time = helper.toFloats(img, length, img_float);

    time += conv.applyFilter(rows, cols, img_float, channels, 1.0, conv_img_float, num_thread);
    time += helper.fromFloats(conv_img_float, length, new_img_char);

    cv::Mat new_img = cv::Mat(rows, cols, CV_8UC3, new_img_char);

    cv::imwrite(outfilename, new_img);
    //delete[] img_float;
    //delete[] conv_img_float;
    return time;
}
void printImg(float * img, int img_width, int num_channels){
    for(int x = 0; x < img_width; x++){
        for(int y = 0; y < img_width; y++){
            printf("(");
            for(int c = 0; c < num_channels; c++)
                printf("%.2f,",img[(y * num_channels * img_width) + (x * num_channels) + c]);
            printf(") ");
        }
        cout << endl;
    }
    cout << endl;
};