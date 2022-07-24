
.PHONY:clean

CC = gcc
CFLAGS = -Wall -g

OBJ=main.o example_number.o example_string.o

main:$(OBJ)
	$(CC) $(CFLAGS) main.o ./test_1/example_number.o ./test_2/example_string.o -o $@

main.o: main.c ./test_1/example_number.h ./test_2/example_string.h
	$(CC) $(CFLAGS) -c main.c -I ./test_1 -I ./test_2 -o $@

example_number.o: ./test_1/example_number.c 
	$(CC) $(CFLAGS) -c ./test_1/example_number.c -I ./test_1 -I ./ -o ./test_1/example_number.o

example_string.o: ./test_2/example_string.c 
	$(CC) $(CFLAGS) -c ./test_2/example_string.c -I ./test_2 -I ./ -o ./test_2/example_string.o

clean:
	-rm -f *.o ./test_1/*.o ./test_2/*.o main