
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = simplified_renderer
TARGET2 = interact
SRC = main.cpp
SRC2 = interact.cpp

all: $(TARGET) $(TARGET2)

$(TARGET): $(SRC)
	$(CXX) -o $(TARGET) $(SRC)

$(TARGET2): $(SRC2)
	$(CXX) -o $(TARGET2) $(SRC2)

clean:
	rm -f $(TARGET) $(TARGET2)
