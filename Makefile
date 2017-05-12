CC = clang

CFLAGS  = -g -Wall

LFLAGS = GeographicLib-1.48/legacy/C/geodesic.c

TARGET = gmm

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LFLAGS)
