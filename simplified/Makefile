
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = simplified_renderer
TARGET2 = interact
SRC = main.cpp
SRC2 = interact.cpp

all: $(TARGET) $(TARGET2)

$(TARGET): $(SRC) space.hpp viewpoint.hpp wall.hpp
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

$(TARGET2): $(SRC2) space.hpp viewpoint.hpp wall.hpp utils.hpp
	$(CXX) -o $(TARGET2) $(SRC2) -static

clean:
	rm -f $(TARGET) $(TARGET2)
