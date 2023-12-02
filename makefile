# Define compiler and flags
CXX = g++
CXXFLAGS = -Wall -g -std=c++11

# Define the target executable
TARGET = main

# Define object files
OBJ = main.o moves.o representation.o perft.o

# Define header files
HEADERS = moves.h representation.h perft.h

# First rule is the one executed when no parameters are fed to the Makefile
all: $(TARGET)

# Rule for linking the program
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for building each object file
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

# Rule for cleaning up generated files
clean:
	rm -f $(TARGET) $(OBJ)

# Rule for running the program
run: $(TARGET)
	./$(TARGET)
