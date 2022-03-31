# Found from https://stackoverflow.com/questions/2483182/recursive-wildcards-in-gnu-make
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

PROG= a.out
CC= gcc
GCCFLAGS =      -g
CFLAGS =        -g
WARNFLAGS = -Wall -ansi -std=gnu11
WARN_ADDITIONAL = -Wshadow

HFILES= $(call rwildcard,game,*.h)
CFILES= $(call rwildcard,game,*.c)
IMGFILES = $(call rwildcard,assets,*.png)

# SOURCEFILES= $(CPPFILES) $(HFILES)	
OFILES=$(CFILES:.c=.o)

all: $(PROG) 

SUFFIXES: .C .o 

%.o:%.c $(HFILES)
	$(CC) -o $*.o -c $(GCCFLAGS) $(WARNFLAGS) $*.c -pthread -lGL -lGLU -lglut -lGLEW -lm

$(PROG): $(OFILES) lib_parser.a
	$(CC) -o $(PROG) $(OFILES) -pthread -lGL -lGLU -lglut -lGLEW -lm -L. -l_parser

clean:
	rm -f $(OFILES) $(PROG) 

# build: game/glFunctions/headers/texturesList.h

game/glFunctions/headers/texturesList.h: tools/gatherAssets/getAssets.out $(IMGFILES)
	./tools/gatherAssets/getAssets.out

tools/gatherAssets/getAssets.out: tools/gatherAssets/findAssets.o
	$(CC) -o tools/gatherAssets/getAssets.out tools/gatherAssets/findAssets.o -lm

# %.o : %.c
# 	$(CC) -o $*.o -c $(GCCFLAGS) $(WARNFLAGS) $*.c -lm