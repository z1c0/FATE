# Compiler and flags
CXX := g++
CXXFLAGS :=-std=c++98 -Wall -Wextra -pedantic -Wno-unused-parameter -D_LINUX

# Directories
SRC_DIR := .
INCLUDED_SUBDIRS := base gui comm os/LINUX
BUILD_DIR := obj
LIB_DIR := lib

# Source files
SRC := $(wildcard $(SRC_DIR)/*.cpp)
SUBDIR_SRCS := $(foreach dir, $(INCLUDED_SUBDIRS), $(wildcard $(dir)/*.cpp))
ALL_SRCS := $(SRC) $(SUBDIR_SRCS)

# Object files
OBJ := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(ALL_SRCS))

# FATE library
LIB_NAME := libFATE.a
LIB := $(LIB_DIR)/$(LIB_NAME)

# Targets
all: $(LIB)

$(LIB): $(OBJ)
	@mkdir -p $(dir $@)
	ar rcs $(LIB) $(OBJ)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

demo: $(LIB)
	$(CXX) $(CXXFLAGS) demo.cpp -o demo -L$(LIB_DIR) -lFATE -lSDL2 -lSDL2_ttf

clean:
	rm -rf $(BUILD_DIR) $(LIB) 

.PHONY: all clean
