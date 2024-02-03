include .env

ASSEMBLY := engine
EXTENTION := .exe
BUILD_DIR := build
OBJ_DIR := obj

TARGET := $(ASSEMBLY)$(EXTENTION)

CFLAGS := -Wall -g -std=c++17 -O2 -I./src -I$(VULKAN_SDK_PATH)/Include -I$(GLFW_PATH)/include -I$(TINYOBJ_PATH)
LDFLAGS := -L$(VULKAN_SDK_PATH)/Lib -L$(GLFW_PATH)/lib-mingw-w64 -lvulkan-1 -lglfw3 -lgdi32 -static

SRC_FILES := $(shell find src -type f -name "*.cpp")
SRC_DIRECTORIES := $(shell find src -type d -name "*")
OBJ_DIRECTORIES := $(addprefix $(OBJ_DIR)/, $(SRC_DIRECTORIES))
OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.cpp=.o))

all: scaffold link assets spv

###############################################################################
# Scaffolding folder structure
###############################################################################
.PHONY: scaffold
scaffold:
	@echo Scaffolding folder structure...
	mkdir -p $(BUILD_DIR) $(BUILD_DIR)/assets/shaders $(BUILD_DIR)/assets/models
	mkdir -p $(OBJ_DIR) $(OBJ_DIRECTORIES)
	@echo Done.

###############################################################################
# Linking
###############################################################################
.PHONY: link
link: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ_FILES)
	g++ -o $@ $^ $(LDFLAGS) 

###############################################################################
# Compiling
###############################################################################
$(OBJ_DIR)/%.o: %.cpp
	gcc -c -MD $(CFLAGS) $< -o $@

include $(wildcard $(addsuffix /*.d,$(OBJ_DIRECTORIES)))

###############################################################################
# Copy assets
###############################################################################
MODELS_SOURCES := $(shell find assets/models -type f -name "*.obj")

.PHONY: assets
assets: $(patsubst %.obj,$(BUILD_DIR)/%.obj,$(MODELS_SOURCES))

$(BUILD_DIR)/%.obj: %.obj
	cp -f $< $@

###############################################################################
# Shaders compiler
###############################################################################
VERT_SOURCES := $(shell find assets/shaders -type f -name "*.vert")
FRAG_SOURCES := $(shell find assets/shaders -type f -name "*.frag")

.PHONY: spv
spv: $(patsubst %,$(BUILD_DIR)/%.spv,$(VERT_SOURCES) $(FRAG_SOURCES))

$(BUILD_DIR)/%.spv: %
	${GLSLC} $< -o $@

###############################################################################
# Run application
###############################################################################
.PHONY: run
run:
	(cd build; ./$(TARGET))

###############################################################################
# Clean assembly
###############################################################################
.PHONY: clean
clean:
	rm -f -r $(OBJ_DIR)/*
	rmdir $(OBJ_DIR)
	rm -f -r $(BUILD_DIR)/*
	rmdir $(BUILD_DIR)
	rm -f ./shaders/*.spv

###############################################################################
# For test commands
###############################################################################
.PHONY: test1
test1: 
	@echo $(patsubst %,$(BUILD_DIR)/%.spv,$(VERT_SOURCES) $(FRAG_SOURCES))