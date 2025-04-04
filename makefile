CXX = g++
CXXFLAGS = -std=c++17 -O2 -I./include

SRC = main.cpp counter_seq_cst.cpp 
OBJ = $(SRC:.cpp=.o)

TARGET = counter_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) -pthread

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
