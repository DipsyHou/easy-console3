# Makefile for 3D Console Renderer

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I src
TARGET = renderer3d
SRC = src/main.cpp

# Windows specific
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    RM = del /Q
    CLEAR = cls
else
    RM = rm -f
    CLEAR = clear
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET)

.PHONY: all run clean
