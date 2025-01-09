CXX      = $(shell fltk-config --cxx)
DEBUG    = -g
CXXFLAGS = $(shell fltk-config --use-images --cxxflags) -I.
LDFLAGS  = $(shell fltk-config --use-images --ldflags)
LDSTATIC = $(shell fltk-config --use-images --ldstaticflags)
LINK     = $(CXX)

BUILD_DIR = build

SRCS = src/main.cpp src/Point.cpp src/PointDatabase.cpp src/MainWindow.cpp

OBJS = $(SRCS:src/%.cpp=$(BUILD_DIR)/%.o)

TARGET = $(BUILD_DIR)/DimensionalPointsDB

.SUFFIXES: .o .cpp

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) -Isrc -c $< -o $@

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(LINK) -o $(TARGET) $(OBJS) $(LDSTATIC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run