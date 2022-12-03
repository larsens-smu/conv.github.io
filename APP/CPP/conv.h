#pragma once
#include <string>
#include "convHelper.h"
class Conv {
    private:
        int kernal_width;
        float * kernal;
        ConvHelper helper;
    public:
        Conv();
        Conv(int, float*);
        float applyFilter(int rows, int cols, void * img, int channels, float padding, void * new_img, int num_threads);
        void setKernal(int size, float *&);
        float * getKernal();
        int getPosition(int x, int y, int c, int num_channels, int img_width, int img_length);
        void printKernal();
        float getPixelValue(int rows, int cols, float * img, int channels, float padding, int r, int c,int channel);
        void setKernal(int size, std::string name);
        void setRowNewImage(float* img, float* new_img, int rows, int cols, int r, float padding, int channels);
        ~Conv();
};