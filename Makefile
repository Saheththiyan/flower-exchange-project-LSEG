# Makefile for Stock Exchange Application
# A C++ implementation of a stock exchange order matching engine

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
INCLUDE_DIR = headers
SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/exchange_app

# Source files
SOURCES = $(SRC_DIR)/csvOrderReader.cpp \
          $(SRC_DIR)/executionReporterWriter.cpp \
          $(SRC_DIR)/instrumentOrderBook.cpp \
          $(SRC_DIR)/orderBook.cpp \
          main.cpp

# Object files (in build directory)
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES:.cpp=.o)))

# Compilation flags with header directory
COMPILE_FLAGS = $(CXXFLAGS) -I$(INCLUDE_DIR)

# Phony targets
.PHONY: all build run clean help rebuild

# Default target
all: build

# Help target
help:
	@echo "Stock Exchange Application - Build Targets"
	@echo "=========================================="
	@echo "  make build  - Compile the application"
	@echo "  make run    - Build and run the application"
	@echo "  make clean  - Remove compiled files and executable"
	@echo "  make help   - Display this help message"

# Create build directory if it doesn't exist (order-only prerequisite)
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Build target
build: $(TARGET)

# Link executable
$(TARGET): $(OBJECTS)
	$(CXX) $(COMPILE_FLAGS) $(OBJECTS) -o $(TARGET)
	@echo "✓ Build successful! Executable: $(TARGET)"

# Compile source files from src directory (create directory if needed)
$(BUILD_DIR)/csvOrderReader.o: $(SRC_DIR)/csvOrderReader.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(COMPILE_FLAGS) -c $< -o $@

$(BUILD_DIR)/executionReporterWriter.o: $(SRC_DIR)/executionReporterWriter.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(COMPILE_FLAGS) -c $< -o $@

$(BUILD_DIR)/instrumentOrderBook.o: $(SRC_DIR)/instrumentOrderBook.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(COMPILE_FLAGS) -c $< -o $@

$(BUILD_DIR)/orderBook.o: $(SRC_DIR)/orderBook.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(COMPILE_FLAGS) -c $< -o $@

# Compile main.cpp (create directory if needed)
$(BUILD_DIR)/main.o: main.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(COMPILE_FLAGS) -c $< -o $@

# Run the application
run: build
	@echo "Running $(TARGET)..."
	./$(TARGET)

# Clean build artifacts
clean:
	@rm -rf $(BUILD_DIR)
	@echo "✓ Clean complete. Removed build directory."

# Rebuild (clean + build)
rebuild: clean build
	@echo "✓ Rebuild complete."
