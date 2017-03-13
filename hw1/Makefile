CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
SOURCES=netstat.cpp Processes.cpp Socketfd.cpp NetInfo.cpp
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
