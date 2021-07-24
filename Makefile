# Found from https://stackoverflow.com/questions/2483182/recursive-wildcards-in-gnu-make
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

PROG= a.out
CC= gcc
GCCFLAGS =      -g
CFLAGS =        -g
WARNFLAGS = -Wall -ansi -std=gnu11
WARN_ADDITIONAL = -Wshadow

HFILES= $(call rwildcard,.,*h)
CFILES= $(call rwildcard,.,*c)
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

