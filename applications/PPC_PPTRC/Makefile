# Makefile for creating the 'pptrc' executable

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++98 -Wall -Wextra -pedantic -Wno-unused-parameter -D_LINUX

# Directories
BUILD_DIR := build
BIN_DIR := bin

# Source files
SRC := $(wildcard *.cpp)

# Object files
OBJ := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC))

# Executable name
EXE_NAME := pptrc
EXE := $(BIN_DIR)/$(EXE_NAME)

# Targets
all: $(EXE)

$(EXE): $(OBJ) ../../framework/lib/libFATE.a
	@mkdir -p $(BIN_DIR)
	@cp ../../framework/font.ttf .
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJ) -L../../framework/lib -lFATE -lSDL2 -lSDL2_ttf

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	@cp IP.txt $(BIN_DIR)
	./$(EXE)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean run
