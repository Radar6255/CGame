# Here is the simple default Makefile that I use whenever makemake is run 
# without any Makefile in the current directory.

PROG= a.out
CC= gcc
GCCFLAGS =      -g
CFLAGS =        -g
WARNFLAGS = -Wall -ansi -std=gnu11
WARN_ADDITIONAL = -Wshadow

HFILES= glFunctions/display.h glFunctions/keyboard.h glFunctions/init.h engine/eventHandler.h
CFILES= main.c glFunctions/display.c glFunctions/keyboard.c glFunctions/init.c
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
	$(CC) -o $*.o -c $(GCCFLAGS) $(WARNFLAGS) $*.c -lGL -lGLU -lglut -lGLEW -lm

$(PROG): $(OFILES)  
	$(CC) -o $(PROG) $(OFILES) -lGL -lGLU -lglut -lGLEW -lm

clean:
	rm -f $(OFILES) $(PROG) 

