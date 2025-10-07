CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ihdr

SRC_DIR = src
BIN_DIR = bin
TARGET = $(BIN_DIR)/regex_engine

SRCS = $(wildcard $(SRC_DIR)/*.cpp)

all: $(TARGET)

$(TARGET):
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@


clean:
	rm -rf $(BIN_DIR)

run: all
	./$(TARGET)
