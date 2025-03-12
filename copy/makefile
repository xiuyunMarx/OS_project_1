CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET1 = MyCopy
TARGET2 = ForkCopy
TARGET3 = PipeCopy
all: clean $(TARGET1) $(TARGET2) $(TARGET3)


$(TARGET1): MyCopy.c
	$(CC) $(CFLAGS) -o $(TARGET1) MyCopy.c

$(TARGET2): ForkCopy.c
	$(CC) $(CFLAGS) -o $(TARGET2) ForkCopy.c

$(TARGET3): PipeCopy.c
	$(CC) $(CFLAGS) -o $(TARGET3) PipeCopy.c

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)

.PHONY: all clean
