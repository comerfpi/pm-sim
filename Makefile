# Define the compiler
CXX = g++

# Define the output executable name
TARGET = pmsim

# Define the source file
SRC = pmsim.C

# Define the compilation flags
CXXFLAGS = `root-config --cflags --libs` -lGarfield

# The rule to build the target
$(TARGET): $(SRC)
	$(CXX) -o $(TARGET) $(SRC) $(CXXFLAGS)

# Clean rule to remove the compiled files
clean:
	rm -f $(TARGET)
