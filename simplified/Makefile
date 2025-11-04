
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = simplified_renderer
SRC = main.cpp

all: $(TARGET)
$(TARGET): $(SRC) space.hpp viewpoint.hpp wall.hpp
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)
