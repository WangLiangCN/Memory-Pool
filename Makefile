CC = gcc
CFLAGS = -Wall -g
TARGET = ./memoryPoolTester
SUBDIR = FABMemoryPool FALMemoryPool FUBMemoryPool FULMemoryPool VABMemoryPool VALMemoryPool VUBMemoryPool VULMemoryPool
SOURCES = $(wildcard *.c) $(shell find $(SUBDIR) -name '*.c')
OBJS = $(patsubst %.c, %.o, $(SOURCES))

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
clean:
	rm $(OBJS) $(TARGET) -rf
