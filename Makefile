# Found from https://stackoverflow.com/questions/2483182/recursive-wildcards-in-gnu-make
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

PROG= a.out
CC= gcc
GCCFLAGS =      -g
CFLAGS =        -g
WARNFLAGS = -Wall -ansi -std=gnu11
WARN_ADDITIONAL = -Wshadow

HFILES= $(call rwildcard,game,*h)
CFILES= $(call rwildcard,game,*c)
CPPFILES=

SOURCEFILES= $(CPPFILES) $(HFILES)	
OFILES=$(CPPFILES:.C=.o) $(CFILES:.c=.o)

all: $(PROG) 

SUFFIXES: .C .o 

.C.o:
	$(CC) -o $*.o -c $(GCCFLAGS) $(WARNFLAGS) $*.C

.cpp.o:
	$(CC) -o $*.o -c $(GCCFLAGS) $(WARNFLAGS) $*.cpp

.c.o:
	$(CC) -o $*.o -c $(GCCFLAGS) $(WARNFLAGS) $*.c -pthread -lGL -lGLU -lglut -lGLEW -lm

$(PROG): $(OFILES)  
	$(CC) -o $(PROG) $(OFILES) -pthread -lGL -lGLU -lglut -lGLEW -lm

clean:
	rm -f $(OFILES) $(PROG) 

build: game/glFunctions/headers/texturesList.h

game/glFunctions/headers/texturesList.h: getAssets.out
	./tools/getAssets.out

getAssets.out: tools/findAssets.o
	$(CC) -o tools/getAssets.out tools/findAssets.o -lm

%.o : %.c
	$(CC) -o $*.o -c $(GCCFLAGS) $(WARNFLAGS) $*.c -lm