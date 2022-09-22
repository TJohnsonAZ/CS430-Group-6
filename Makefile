all: ppmrw

ppmrw:
	gcc -Wall ppmrw.c -o ppmrw.o

clean:
	rm -rf *.o *~
