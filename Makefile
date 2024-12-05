CC = gcc
CFLAGS = -Wall -pedantic -g -std=c99
TARGET = corgis.o
SRC = corgis.c
HEADERS = corgis.h

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)