CC=g++

CFLAGS=-Wall -std=c99 

LDFLAGS=-I ./include/ 

TARGET=-o blocker 

SDIR=src

ODIR=obj
	  
blocker : src/main.o src/csdlgraphics.o src/cgrid.o src/cgamepiece.o src/cintroscreen.o src/cgamescreen.o
	${CC} ${CFLAGS} ${LDFLAGS} ${TARGET} src/main.o src/csdlgraphics.o src/cgrid.o src/cgamepiece.o \
		src/cintroscreen.o src/cgamescreen.o -lmingw32 -lSDLmain -lSDL -lSDL_ttf -lSDL_image -mconsole 
clean:
	-del $(SDIR)\*.o
	-del blocker
