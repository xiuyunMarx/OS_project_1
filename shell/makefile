CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
TARGET1 = shellSingle
TARGET2 = shell

all: $(TARGET1) $(TARGET2)

$(TARGET1): shellSingle.c
	gcc $(CFLAGS) -o $(TARGET1) shellSingle.c

$(TARGET2): shell.c
	gcc $(CFLAGS) -o $(TARGET2) shell.c

clean:
	rm -f $(TARGET1) $(TARGET2)

.PHONY: all clean