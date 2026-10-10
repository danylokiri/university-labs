CC = gcc
CFLAGS = -Wall -g
TARGET = main

all: $(TARGET)

$(TARGET): main.o point.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o point.o

main.o: main.c point.h
	$(CC) $(CFLAGS) -c main.c

point.o: point.c point.h
	$(CC) $(CFLAGS) -c point.c

run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)
	@pause >nul

clean:
	rm -f *.o $(TARGET)
