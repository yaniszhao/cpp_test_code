CC	= gcc
CFLAGS	= -Wall -O2 -g -std=gnu99
LDFLAGS	=

SERVER	= server
OBJS	= server.o myepoll.o

all: $(SERVER)

$(SERVER) : $(OBJS)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f $(OBJS) client.o $(SERVER) $(CLIENT) 
