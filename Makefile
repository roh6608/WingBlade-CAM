
CC = gcc
CFLAGS = -O2 -Wall

all: compile

compile: 
	$(CC) $(CFLAGS) -o ./bin/WingBlade-CAM ./src/main.c

run: clean compile
	./bin/WingBlade-CAM

clean:
	rm ./bin/WingBlade-CAM
