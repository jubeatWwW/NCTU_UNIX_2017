CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=netstat.cpp Processes.cpp Socketfd.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=netstat

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run:
	./$(EXECUTABLE)

clean: 
	rm -f $(OBJECTS)
