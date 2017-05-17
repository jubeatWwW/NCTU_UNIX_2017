CXX=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
SOURCES=main.cpp Pool.cpp Task.cpp ProcGrp.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=jsh

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $< -o $@

run:
	./$(EXECUTABLE)

clean: 
	rm -f $(OBJECTS)

