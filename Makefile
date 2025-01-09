CXX      = $(shell fltk-config --cxx)
DEBUG    = -g
CXXFLAGS = $(shell fltk-config --use-images --cxxflags) -I.
LDFLAGS  = $(shell fltk-config --use-images --ldflags)
LDSTATIC = $(shell fltk-config --use-images --ldstaticflags)
LINK     = $(CXX)

# Build directory
BUILD_DIR = build

TARGET = $(BUILD_DIR)/DimensionalPointsDB
OBJS   = $(BUILD_DIR)/main.o
SRCS   = src/main.cpp

.SUFFIXES: .o .cpp

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

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