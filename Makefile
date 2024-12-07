CXX = g++
CXXFLAGS = $(shell fltk-config --use-images --cxxflags)
LDFLAGS = $(shell fltk-config --use-images --ldflags)

SOURCES = main.cpp game24.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = game24

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) 