#Makefile

CC= g++ -std=c++17 -stdlib=libc++
OBJECTS = examples/objects
SOURCES = lib/sources
MAIN = examples
BIN = bin
INCLUDES = -I lib/headers/
CFLAGS= -g -O2

all: $(BIN)/thermalGradient

$(BIN)/thermalGradient: $(OBJECTS)/thermalGradient.o $(OBJECTS)/vortex.o $(OBJECTS)/boundary_condition.o $(OBJECTS)/ran1.o
	${CC} ${CFLAGS} $^ -o $@

$(OBJECTS)/thermalGradient.o: $(MAIN)/thermalGradient.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@

$(OBJECTS)/vortex.o: $(SOURCES)/vortex.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@

$(OBJECTS)/boundary_condition.o: $(SOURCES)/boundary_condition.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@

$(OBJECTS)/ran1.o: $(SOURCES)/ran1.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@


clean:
	rm $(OBJECTS)/*.o
