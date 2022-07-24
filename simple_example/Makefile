
.PHONY:clean

CC = gcc
CFLAGS = -Wall -g

OBJ=main.o

main:$(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ 


clean:
	-rm -f *.o main