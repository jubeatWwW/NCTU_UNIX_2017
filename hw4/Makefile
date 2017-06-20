CXX=g++
CFLAGS	= -c -Wall -g -I/usr/include/ncurses -std=c++11
LDFLAGS	= -lncurses -pthread
SOURCES=main.cpp othello.cpp server.cpp client.cpp Game.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=othello

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $< -o $@

run:
	./$(EXECUTABLE)

clean: 
	rm -f *.o *~ $(EXECUTABLE)

