CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
TARGET = binary_tree
SOURCES = main.cpp
HEADERS = src/AVLTree.h src/RBTree.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

test: $(TARGET)
	printf '3\n0\n' | ./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all test clean
