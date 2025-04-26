TARGET=ssd1306_API

all: $(TARGET)

cc = gcc

CFLAGS = -g -O2 -Wall

$(TARGET): $(TARGET).c
	$(CC) $(TARGET).c -o $(TARGET)

clean:
	rm -f $(TARGET)
