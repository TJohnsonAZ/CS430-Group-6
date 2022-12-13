all: raytrace input pnmquant ppmtogif gifsicle

raytrace: raytrace.c v3math.c
	gcc -Wall -std=c99 raytrace.c v3math.c -o raytrace -lm

input:
	read -p "Enter image dimensions & file name (width height file): " width height fh; \
	./raytrace $$width $$height $$fh frame.ppm;

pnmquant: 
	for frame in frame* ; do \
		pnmquant 256 $${frame} >/tmp/colormap.ppm >$${frame}_256.ppm ; \
	done

ppmtogif:
	for frame in frame*.ppm_256.ppm ; do \
		ppmtogif $${frame} > $${frame}.gif ; \
	done

gifsicle:
	gifsicle --colors 256 --loopcount=forever *.gif > test.gif

clean:
	rm -rf raytrace *.gif *_256.ppm *~
