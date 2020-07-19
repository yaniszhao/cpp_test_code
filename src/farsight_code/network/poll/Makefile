CC	= gcc
CFLAGS	= -Wall -O2 -g -std=gnu99 -I.
LDFLAGS	=

SERVER	= server
OBJS	= main.o myserver.o

all: $(SERVER)

$(SERVER) : $(OBJS)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f $(OBJS) client.o $(SERVER) $(CLIENT) 
