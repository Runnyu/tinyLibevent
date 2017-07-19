main: event.o util.o
	cc -c main.c -o main.o 
	cc main.o event.o util.o -o main	
event:
	cc -c event.c -o event.o 
util:
	cc -c util.c -o util.o 


clean:
	rm main.o event.o util.o
