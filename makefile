# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

# Executable name
TARGET = main

# Source and object files
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=%.o)

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile step (pattern rule)
# Rely on compiler to track headers via includes; no forced %.hpp prerequisite
%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
