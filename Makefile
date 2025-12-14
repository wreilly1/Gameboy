# Simple Makefile for GameBoy Emulator
# Usage: make -f Makefile.simple

CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iinclude
LIBS = $(shell pkg-config --libs sdl2 SDL2_ttf)
CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_ttf)

# Target executable
TARGET = GameboyEmulator

# Source files
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
SOURCES += main.cpp

# Object files
OBJDIR = obj
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/$(SRCDIR)

# Link the executable
$(TARGET): $(OBJDIR) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

# Compile source files
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Install dependencies (Ubuntu/Debian)
deps:
	sudo apt install libsdl2-dev libsdl2-ttf-dev

# Run the emulator (needs ROM file)
run: $(TARGET)
	./$(TARGET) --rom <your-rom-file.gb>

.PHONY: all clean deps run