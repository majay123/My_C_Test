all:
	gcc -o test.o test.c -lm

clean:
	rm *.o