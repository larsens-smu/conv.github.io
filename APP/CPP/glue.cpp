
#include <emscripten.h>

extern "C" {

// Not using size_t for array indices as the values used by the javascript code are signed.

EM_JS(void, array_bounds_check_error, (size_t idx, size_t size), {
  throw 'Array index ' + idx + ' out of bounds: [0,' + size + ')';
});

void array_bounds_check(const int array_size, const int array_idx) {
  if (array_idx < 0 || array_idx >= array_size) {
    array_bounds_check_error(array_idx, array_size);
  }
}

// VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// Conv

Conv* EMSCRIPTEN_KEEPALIVE emscripten_bind_Conv_Conv_0() {
  return new Conv();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Conv_setKernal_2(Conv* self, int size, char* name) {
  self->setKernal(size, name);
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_Conv_applyFilter_7(Conv* self, int rows, int cols, void* imgVoid, int channels, float padding, void* new_imgVoid, int num_threads) {
  return self->applyFilter(rows, cols, imgVoid, channels, padding, new_imgVoid, num_threads);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Conv___destroy___0(Conv* self) {
  delete self;
}

// ConvHelper

ConvHelper* EMSCRIPTEN_KEEPALIVE emscripten_bind_ConvHelper_ConvHelper_0() {
  return new ConvHelper();
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_ConvHelper_toFloats_3(ConvHelper* self, void* img, unsigned int length, void* answer) {
  return self->toFloats(img, length, answer);
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_ConvHelper_fromFloats_3(ConvHelper* self, void* img, unsigned int length, void* answer) {
  return self->fromFloats(img, length, answer);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ConvHelper___destroy___0(ConvHelper* self) {
  delete self;
}

}

