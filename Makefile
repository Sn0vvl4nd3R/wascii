CXX = clang++
CXXFLAGS = -Wall -std=c++17 $(shell pkg-config --cflags opencv4)
LDFLAGS = $(shell pkg-config --libs opencv4)
TARGET = ascii_cam
SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
