CXX = g++
CXXFLAGS = -std=c++11 -Iinclude
SOURCES = $(wildcard src/*.cpp) main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = EncryptionTool

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)