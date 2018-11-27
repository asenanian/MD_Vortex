#Makefile

CC= g++ -std=c++11 -stdlib=libc++
OBJECTS = objects
SOURCES = sources
BIN = bin
INCLUDES = -I headers/
CFLAGS= -g

all: $(BIN)/thermalGradient

$(BIN)/thermalGradient: $(OBJECTS)/thermalGradient.o $(OBJECTS)/vortex_lattice.o $(OBJECTS)/vortex.o $(OBJECTS)/boundary_condition.o $(OBJECTS)/ran1.o
	${CC} ${CFLAGS} $^ -o $@

$(OBJECTS)/thermalGradient.o: $(SOURCES)/thermalGradient.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@

$(OBJECTS)/vortex_lattice.o: $(SOURCES)/vortex_lattice.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@

$(OBJECTS)/vortex.o: $(SOURCES)/vortex.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@

$(OBJECTS)/boundary_condition.o: $(SOURCES)/boundary_condition.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@

$(OBJECTS)/ran1.o: $(SOURCES)/ran1.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@


clean:
	rm $(OBJECTS)/*.o
