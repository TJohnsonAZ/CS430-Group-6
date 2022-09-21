// four functionalities
// 1. Read P3 files into RGBA ordered memory
// 2. Read P6 files into RGBA ordered memory
// 3. Write P3 files from RGBA ordered memory
// 4. Write P6 files from RGBA ordered memory

// 
#include <stdio.h> //fopen, fscanf, ect
#include <stdlib.h> // malloc, ect
#include <stdint.h> // standard integer types
#include <ctype.h> // isspace, ect

int main (int argc, char **argv) {

    int r, g, b;
    int width, height;
    int maxColorVal;
    int magicNum;

    char fileStore[1000];
    char commentLines[1000];

    // check if command line has missing arguments
    if (argc != 4) {
        printf("Missing arguments on command line");
	return 0;
    }

    // open the input file
    FILE *inputfh = fopen(argv[2]);
    FILE *outputfh = fopen(argv[3]);

    // TODO: check if files opened incorrectly

// MAGIC NUMBER

    // scan to find the magic number
    int scanCount = fscanf(inputfh, "P%d\n", &magicNum);

    // check if no magic number was found
    if( scanCount == 0) {

        // return error message
        printf("\nNo magic number found");
        return 0;

    }
    
    // check if the file is converting to its own format
    if (magicNum == argv[2]) {

        // write the data straight to the output file
        fread(fileStore, inputfh);
        fwrite(fileStore, outputfh);

        // display final message
        fprintf("\nConversion Complete\n")
        return 0;

    }
    // assume magic number in file doesn't match given argument
    else {
        printf("Magic number in file does not match given argument");
	return 0;
    }

// COMMENTS 

    // scan the document for all comments
    while (/*is not a number*/) {

        do {
            
            // write in the comment to the output file

        } while (/* is not a newline char*/);

    }
    
// DIMENSIONS

    // scan in the dimensions of the image
    fscanf("\n%d %d\n", &width, &height);

    // write out the dimensions
    fwrite(outputfh, "%d %d\n", width, height);

// MAX COLOR VALUE

    // scan in the max color value
    fscanf("\n%d\n", &maxColorVal);

    // write out the max color value
    fwrite(outputfh, "\n%d\n", maxColorVal);

    

}
