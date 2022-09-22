all: ppmrw

ppmrw:
	gcc -Wall project_1.c -o ppmrw

clean:
	rm -rf *.o *~ ppmrw output.ppm