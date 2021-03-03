# the compiler:
CC = gcc
# compiler flags:
CFLAGS = -g -Wall -lm
SRCS = main.c
TARGET = calc
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)
clean:
	$(RM) $(TARGET)
