
client: server
	gcc -o client myclient.c client_fun.c

server:
	gcc -o server myserver.c serverfun.c -lpthread

clean:
	rm -f client server
	
.PHONY: clean
 
	