#pragma once
class ConvHelper {
    private:
        float* sharpen3;
        float* sharpen5;
        float* sharpen2type3;
        float* sharpen2type5;
        float* embossVertical3;
        float* embossVertical5;
        float* embossHoriztonal3;
        float* embossHoriztonal5;
        float* gausianBlur3;
        float* gausianBlur5;
        float* laplaucian3;
        float* laplaucian5;
        float* horizonzontalEdge3;
        float* horizonzontalEdge5;
        float* verticalEdge3;
        float* verticalEdge5;

    public:
        ConvHelper();
        ~ConvHelper();
        float toFloats(void* img, unsigned int length, void* answer);
        float fromFloats(void * img, unsigned int length, void* answer);
        float* getSharpenKernal(int size){
            if(size == 3){
                return this->sharpen3;
            }
            else if (size == 5){
                return this->sharpen5;
            }
            return nullptr;
        };
        float* getEmbossKernalVertical(int size){
            if(size == 3){
                return this->embossVertical3;
            }
            else if (size == 5){
                return this->embossVertical5;
            }
            return nullptr;

        };
        float* getEmbossKernalHorizontal(int size){
            if(size == 3){
                return this->embossHoriztonal3;
            }
            else if(size == 5){
                return this->embossHoriztonal5;
            }
            return nullptr;

        };
        float* getGaussianBlurKernal(int size){
            if(size == 3){
                return this->gausianBlur3;
            }
            else if(size == 5){
                return this->gausianBlur5;
            }
            return nullptr;

        };
        float* getLaplacianKernal(int size){
            if(size == 3){
                return this->laplaucian3;
            }
            else if(size ==5) {
                return this->laplaucian5;
            }
            return nullptr;
        };
        float* getVerticalEdgeKernal(int size){
            if(size == 3){
                return this->verticalEdge3;
            }
            else if(size == 5){
                return this->verticalEdge5;
            }
            return nullptr;
        }
        float* getHorizontalEdgeKernal(int size){
            if(size == 3){
                return this->horizonzontalEdge3;
            }
            else if(size == 5){
                return this->horizonzontalEdge5;
            }
            return nullptr;
        }
        float * getSharpen2type(int size){
            if(size == 3){
                return this->sharpen2type3;
            }
            else if(size == 5){
                return this->sharpen2type5;
            }
            return nullptr;
        }
};