CC = gcc
CFLAGS = -Wall -g
TARGET_STATIC = main_static
TARGET_DYNAMIC = main_dynamic
STATIC_LIB = libarraylist.a
DYNAMIC_LIB = libarraylist.so

all: $(TARGET_STATIC) $(TARGET_DYNAMIC)

$(STATIC_LIB): arraylist.o
	ar rcs $(STATIC_LIB) arraylist.o

$(DYNAMIC_LIB): arraylist.o
	$(CC) -shared -o $(DYNAMIC_LIB) arraylist.o

arraylist.o: arraylist.c arraylist.h
	$(CC) $(CFLAGS) -fPIC -c arraylist.c

$(TARGET_STATIC): main.o $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $(TARGET_STATIC) main.o $(STATIC_LIB)

$(TARGET_DYNAMIC): main.o $(DYNAMIC_LIB)
	$(CC) $(CFLAGS) -o $(TARGET_DYNAMIC) main.o -L. -larraylist

main.o: main.c arraylist.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o $(TARGET_STATIC) $(TARGET_DYNAMIC) $(STATIC_LIB) $(DYNAMIC_LIB)
