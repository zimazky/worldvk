include .env

CFLAGS = -std=c++17 -I./src -I$(VULKAN_SDK_PATH)/Include -I${GLFW_PATH}/include
LDFLAGS = -L$(VULKAN_SDK_PATH)/Lib -L${GLFW_PATH}/lib-mingw-w64 -static -lvulkan-1 -lglfw3 -lgdi32

a.exe: ./src/*.cpp ./src/*.hpp
	g++ $(CFLAGS) -o a.exe ./src/*.cpp $(LDFLAGS)

.PHONY: test clean

test: a.exe
	./a.exe

clean:
	rm -f a.exe