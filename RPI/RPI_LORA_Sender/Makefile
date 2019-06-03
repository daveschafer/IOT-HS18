# dragino lora testing
# Single lora testing app

CC=g++
CFLAGS=-c -Wall
LIBS=-lwiringPi

all: dragino_lora_app

dragino_lora_app: Lora_sender.o
	$(CC) Lora_sender.o  $(LIBS) -o dragino_lora_app

Lora_sender.o: Lora_sender.c
	$(CC) $(CFLAGS) Lora_sender.c

clean:
	rm *.o dragino_lora_app	
