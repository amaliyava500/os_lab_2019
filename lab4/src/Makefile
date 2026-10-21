CC=gcc
CFLAGS=-I.

all: parallel_min_max zombie process_memory parallel_sum1

# Задание 1
parallel_min_max: utils.o find_min_max.o parallel_min_max.c
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o: utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o: find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

# Задание 2
zombie: zombie.c
	$(CC) -o zombie zombie.c $(CFLAGS)

# Задание 3
process_memory: process_memory.c
	$(CC) -o process_memory process_memory.c $(CFLAGS)

# Задание 5 и 6
libsum.a: sum.o
	ar rcs libsum.a sum.o

sum.o: sum.h
	$(CC) -o sum.o -c sum.c $(CFLAGS)

parallel_sum1: utils.o libsum.a parallel_sum1.c
	$(CC) -o parallel_sum1 parallel_sum1.c utils.o -L. -lsum -lpthread $(CFLAGS)

clean:
	rm -f *.o *.a parallel_min_max zombie process_memory parallel_sum1
