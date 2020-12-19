all: 
	clear
	gcc -o server server.c
	gcc -o client client.c

s: 
	clear
	./server 8080
c:
	clear
	./client 127.0.0.1 8080