EMSDK_PATH = /Users/stevenlarsen/emsdk/upstream/emscripten
CODE_PATH = /Users/stevenlarsen/Documents/School/CS_7345/conv_final/APP
OUT_PATH = /Users/stevenlarsen/Documents/School/CS_7345/conv_final/www/app/emcc
glue: 
	cd $(EMSDK_PATH);\
	python3 tools/webidl_binder.py $(CODE_PATH)/CPP/my_classes.idl \
	$(CODE_PATH)/CPP/glue


emcc:
	cd $(EMSDK_PATH);\
	./em++ $(CODE_PATH)/CPP/fake_main.cpp $(CODE_PATH)/CPP/conv.cpp ${CODE_PATH}/CPP/convHelper.cpp \
	$(CODE_PATH)/CPP/my_glue_wrapper.cpp \
	-sINITIAL_MEMORY=2147483648 \
	-std="c++11" \
	-s USE_PTHREADS -s  -sPTHREAD_POOL_SIZE=128 -sPROXY_TO_PTHREAD \
	--post-js /$(CODE_PATH)/CPP/glue.js -o \
	$(OUT_PATH)/output.js -O2 -s \
	EXPORTED_FUNCTIONS="['_malloc', '_free']" \
	