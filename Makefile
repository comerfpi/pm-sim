# Define the compiler
CXX = g++

# Define the output executable name
TARGET1 = pmsim
TARGET2 = histogrammer

# Define the source file
SRC1 = pmsim.C
SRC2 = histogrammer.C

# Define the compilation flags
CXXFLAGS = `root-config --cflags --libs` -lGarfield

# The rule to build all targets
all: $(TARGET1) $(TARGET2)

# The rule to build the targets
$(TARGET1): $(SRC1)
	$(CXX) -o $(TARGET1) $(SRC1) $(CXXFLAGS)

# The rule to build the targets
$(TARGET2): $(SRC2)
	$(CXX) -o $(TARGET2) $(SRC2) $(CXXFLAGS)

# Clean rule to remove the compiled files
clean:
	rm -f $(TARGET1) $(TARGET2)
