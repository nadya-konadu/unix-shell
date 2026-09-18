CC = gcc
CFLAGS = -Wall -g
TARGET = shell

$(TARGET): shell.c
	$(CC) $(CFLAGS) -o $(TARGET) shell.c

clean:
	rm -f $(TARGET)