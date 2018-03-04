CC := gcc
CFLAGS :=-Wall -g
LDFLAGS := -lpng -g -lm
HEADERS := renderer.h
EXE := renderer
SOURCES := renderer.o main.o vector.o

.ALL: ${EXE}

%.o: %.c ${HEADERS} Makefile
	${CC} -c ${CFLAGS} -o $@ $<

${EXE}: ${SOURCES}
	$(CC) ${SOURCES} -o ${EXE} ${LDFLAGS}

clean::
	rm -f ${SOURCES} renderer image.png

