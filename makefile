include .env

CFLAGS = -std=c++17 -I./src -I$(VULKAN_SDK_PATH)/Include -I${GLFW_PATH}/include -I$(TINYOBJ_PATH)
LDFLAGS = -L$(VULKAN_SDK_PATH)/Lib -L${GLFW_PATH}/lib-mingw-w64 -static -lvulkan-1 -lglfw3 -lgdi32

# create list of all spv files and set as dependency
vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = a.exe
$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): ./src/**/*.cpp ./src/**/*.hpp
	g++ $(CFLAGS) -o ./build/$(TARGET) ./src/**/*.cpp $(LDFLAGS)

# make shader targets
%.spv: %
	${GLSLC} $< -o $@

.PHONY: test clean

test: a.exe
	./build/a.exe

clean:
	rm -f ./build/a.exe
	rm -f ./shaders/*.spv