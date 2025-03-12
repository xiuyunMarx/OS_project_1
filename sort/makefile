CC = gcc
CFLAGS = -g -Wall -Wextra -O2
LINKERS = -lm -pthread
TARGET = MergesortSingle MergesortMulti

.PHONY: all clean

all: clean $(TARGET)

MergesortSingle: MergesortSingle.c
	$(CC) $(CFLAGS) MergesortSingle.c -o MergesortSingle $(LINKERS)

MergesortMulti: MergesortMulti.c
	$(CC) $(CFLAGS) MergesortMulti.c -o MergesortMulti $(LINKERS)

clean:
	rm -f $(TARGET)


