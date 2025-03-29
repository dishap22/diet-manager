CXX = g++
CXXFLAGS = -std=c++17
SRC = main.cpp
TARGET = DietManager

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)