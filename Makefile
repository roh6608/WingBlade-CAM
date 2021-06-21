
CC = gcc
CFLAGS = -O2 -Wall

all: compile

compile: 
	$(CC) $(CFLAGS) -o ./bin/WingBlade-CAM ./src/main.c -lm

run: clean compile
	./bin/WingBlade-CAM

clean:
	rm ./bin/WingBlade-CAM
