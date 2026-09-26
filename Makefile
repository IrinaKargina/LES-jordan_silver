CC = gcc
CFLAGS = -O3 -Wall -Wextra -std=c99
LDFLAGS = -lm

OBJS = main.o matrix_io.o solve.o
TARGET = a.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.c matrix_io.h solve.h
	$(CC) $(CFLAGS) -c main.c

matrix_io.o: matrix_io.c matrix_io.h
	$(CC) $(CFLAGS) -c matrix_io.c

solve.o: solve.c solve.h
	$(CC) $(CFLAGS) -c solve.c

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean
