# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Executable name
TARGET = main

# Source and object files
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile step (pattern rule)
# Rely on compiler to track headers via includes; no forced %.hpp prerequisite
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
