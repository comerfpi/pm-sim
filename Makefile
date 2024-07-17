# Define the compiler
CXX1 = g++

# Define the output executable name
TARGET1 = pmsim
TARGET2 = histogrammer
TARGET3 = driftvel

# Define the source file
SRC1 = pmsim.C
SRC2 = histogrammer.C
SRC3 = driftvel.cpp

# Define the compilation flags
CXXFLAGS = `root-config --cflags --libs` -lGarfield -I/usr/local/include/NEST -I/usr/local/include/Detectors -lNEST

# The rule to build all targets
all: $(TARGET1) $(TARGET2) $(TARGET3) 

# The rule to build the targets
$(TARGET1): $(SRC1)
	$(CXX) -o $(TARGET1) $(SRC1) $(CXXFLAGS)

$(TARGET2): $(SRC2)
	$(CXX) -o $(TARGET2) $(SRC2) $(CXXFLAGS)

$(TARGET3): $(SRC3)
	$(CXX) -o $(TARGET3) $(SRC3) $(CXXFLAGS)

# Clean rule to remove the compiled files
clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)
