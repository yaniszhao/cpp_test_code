CC = gcc
#SOURCE = $(wildcard *.c *.o)
SOURCE = main.c main.o foo.c foo.o bar.c bar.o
TARGET = simple
ADD_SOURCE = $(addprefix /home/yanis/Desktop/, $(SOURCE))
SOURCE_C = $(filter %.c, $(SOURCE))
#SOURCE_O = $(filter-out %.c, $(SOURCE)) 
SOURCE_O = $(patsubst %.c, %.o, $(SOURCE_C))

test:
	@echo $(SOURCE)
	@echo $(ADD_SOURCE) 
	@echo $(SOURCE_C)
	@echo $(SOURCE_O)

all:$(SOURCE_O)
	$(CC)  -o  $(TARGET) $(SOURCE_O) 
main.o:main.c
	$(CC) -c $^ -o $@
foo.o:foo.c
	$(CC) -c $^ -o $@
bar.o:bar.c
	$(CC) -c $^ -o $@

clean:
	$(RM) *.o
.PHONY:clean
