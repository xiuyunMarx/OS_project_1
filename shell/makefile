CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
TARGET1 = shell
TARGET2 = serverCore

all: $(TARGET1) $(TARGET2)

$(TARGET1): shell.c
	gcc $(CFLAGS) -o $(TARGET1) shell.c

$(TARGET2): serverCore.c
	gcc $(CFLAGS) -o $(TARGET2) serverCore.c

clean:
	rm -f $(TARGET1) $(TARGET2)

.PHONY: all clean