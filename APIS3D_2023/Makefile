FILES := $(shell ls *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(FILES))
CPP=g++
CC=gcc
EXE=motor.exe

all: $(OBJS)
	g++ --std=c++2a -g $(OBJS) -o $(EXE) -lGL -lglfw -lGLEW -lpugixml -lSDL2

%.o: %.cpp
	$(CPP) --std=c++2a -g -c -o $@ $< $(CFLAGS)

%.o: %.c
	$(CC) -g -c -o $@ $< $(CFLAGS)
	
clean:
	rm *.o
	rm $(EXE)
