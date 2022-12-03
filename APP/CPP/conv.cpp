#include "conv.h"
#include "convHelper.h"
#include <iostream>
#include <cstdlib>

#include <string.h>
#include <stdio.h>
#include <thread>
// Helper fucntion
int Conv::getPosition(int row, int col, int channel, int num_channels, int num_rows, int num_cols)
{
    if (col < 0 || col >= num_cols)
        return -1;
    if (row < 0 || row >= num_rows)
        return -1;
    return (row * num_channels * num_cols) + col * num_channels + channel;
}
Conv::Conv()
{
    this->kernal_width = 3;
    this->kernal = new float[kernal_width * kernal_width];
    for (int i = 0; i < kernal_width * kernal_width; i++)
    {
        this->kernal[i] = 1.0 / 9.0;
    }
};
Conv::Conv(int width, float *kernal)
{
    this->kernal_width = width;
    this->kernal = kernal;
};
float Conv::applyFilter(int rows, int cols, void *imgVoid, int channels, float padding, void *new_imgVoid, int num_threads)
{

    float *img = (float *)imgVoid;
    float *new_img = (float *)new_imgVoid;
    
    std::thread *ths = new std::thread[num_threads];
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    t1 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < rows;)
    {
        int thread_count = num_threads;
        for(int t = 0; t < num_threads; t++){
            auto f = [this](float* img, float* new_img, int rows, int cols, int r, float padding, int channels) {
                this->setRowNewImage(img, new_img, rows, cols, r, padding, channels);
            };
            ths[t] = std::thread(f, img, new_img, rows, cols, r, padding, channels);
            r++;
            if(r >= rows){
                thread_count = t+1;
                break;
            }
        }
        for(int t = 0; t < thread_count; t++){
            ths[t].join();
        }
    }

    delete[] ths;

    for (int i = 0; i < rows * cols * channels; i++)
    {
        if (new_img[i] < 0)
        {
            new_img[i] = 0;
        }
        if (new_img[i] > 1)
        {
            new_img[i] = 1;
        }
    }
    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> time_span = t2 - t1;

    return std::chrono::duration<float>(time_span).count();
}
float Conv::getPixelValue(int rows, int cols, float *img, int channels, float padding, int r, int c, int channel)
{
    float sum = 0;
    for (int kcol = 0; kcol < this->kernal_width; kcol++)
    {
        for (int krow = 0; krow < this->kernal_width; krow++)
        {
            int row_affect = krow - kernal_width / 2;
            int col_affect = kcol - kernal_width / 2;
            int kpos = getPosition(r + row_affect, c + col_affect, channel, channels, rows, cols);
            float value = padding;
            if (kpos != -1)
            {
                value = img[kpos];
            }
            else
            {
                value = padding;
            }
            sum += this->kernal[krow * this->kernal_width + kcol] * value;
        }
    }
    return sum;
}
void Conv::setKernal(int size, float *&kernal)
{
    float *temp = kernal;
    this->kernal = kernal;
    this->kernal_width = size;
}
float *Conv::getKernal()
{
    return this->kernal;
}
void Conv::printKernal()
{
    for (int y = 0; y < this->kernal_width; y++)
    {
        for (int x = 0; x < this->kernal_width; x++)
        {
            printf("(%d,%f),", y * kernal_width + x, this->kernal[y * kernal_width + x]);
        }
        printf("\n");
    }
}
void Conv::setKernal(int size, std::string name)
{
    if (name == "sharpen")
    {
        float *kernal = this->helper.getSharpenKernal(size);
        this->setKernal(size, kernal);
    }
    if (name == "embossVertical")
    {
        float *kernal = this->helper.getEmbossKernalVertical(size);
        this->setKernal(size, kernal);
    }
    if (name == "embossHorizontal")
    {
        float *kernal = this->helper.getEmbossKernalHorizontal(size);
        this->setKernal(size, kernal);
    }
    if (name == "gaussianBlurKernal")
    {
        float *kernal = this->helper.getGaussianBlurKernal(size);
        this->setKernal(size, kernal);
    }
    if (name == "laplacianKernal")
    {
        float *kernal = this->helper.getLaplacianKernal(size);
        this->setKernal(size, kernal);
    }
    if (name == "edgeHorizontal")
    {
        float *kernal = this->helper.getHorizontalEdgeKernal(size);
        this->setKernal(size, kernal);
    }
    if (name == "edgeVertical")
    {
        float *kernal = this->helper.getVerticalEdgeKernal(size);
        this->setKernal(size, kernal);
    }
    if (name == "sharpen2type")
    {
        float *kernal = this->helper.getSharpen2type(size);
        this->setKernal(size, kernal);
    }
}
Conv::~Conv()
{
    // delete[] this->kernal;
}
void Conv::setRowNewImage(float* img, float* new_img, int rows, int cols, int r, float padding, int channels)
{
    for (int c = 0; c < cols; c++)
    {
        for (int channel = 0; channel < channels; ++channel)
        {
            int pos = this->getPosition(r, c, channel, channels, rows, cols);
            new_img[pos] = getPixelValue(rows, cols, img, channels, padding, r, c, channel);
        }
    }
}