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
#include <string.h>
#include <stdbool.h>

char readFromP3(FILE* fh, int size) {
    int asciiNum;
    char asciiList[size * 3];
    int index = 0;
    
    while (index < size)
    {
        int scanCount = fscanf(fh, "%u", &asciiNum);

        asciiList[index] = asciiNum;
        index++;
    
    }

    return asciiList;
}

uint8_t readFromP6(FILE* fh, int size) {
    
    uint8_t binaryArray[size];

    fread(&binaryArray, sizeof(uint8_t), size * 3, fh);

    return binaryArray;
}


void writeToP3(FILE* fh) {
    // sprintf
}

void writeToP6(FILE* fh, int size, uint8_t outputArray[]) {
    fwrite(outputArray, sizeof(uint8_t), size, fh )
}

int main (int argc, char **argv) {

    int r = 0, g = 0, b = 0;
    int width = 0, height = 0;
    int maxColorVal = 0;
    int magicNum = 0;

    // check if command line has missing arguments
    if (argc != 4) {
        fprintf(stderr, "Error: Missing/too many arguments on command line");
	    return 7;
    }

    // open the input file
    FILE *inputfh = fopen(argv[2], "r");
    FILE *outputfh = fopen(argv[3], "w");

    // check if input file opened incorrectly
    if (inputfh == NULL) {
        fprintf(stderr, "Error: Input file not found");
	    return 2;
    }

    // check if output file opened incorrectly
    if (outputfh == NULL) {
        fprintf(stderr, "Error: output file not found");
	    return 2;
    }

// MAGIC NUMBER

    // scan to find the magic number
    char magicNumStr[] = "";
    int scanCount = fscanf(inputfh, "P%d", &magicNum);

    // check if no magic number was found
    if( scanCount == 0) {

        // return error message
        fprintf(stderr, "Error: Magic number not found in input file");
        return 3;
    }
    
    // Write the magic number to the output file
    sprintf(magicNumStr, "P%d", magicNum);
    fwrite(magicNumStr, sizeof(magicNumStr), strlen(magicNumStr), outputfh);

    // display final message
    printf("\nConversion Complete\n");

// COMMENTS 

    // scan the document for all comments
    /*while (false) { // TODO: change later

        do {
            
            // write in the comment to the output file

        } while (false); // TODO: change later 

    }
    
// DIMENSIONS
*/
    // scan in the dimensions of the image
    char widthAndHeight[] = "";
    scanCount = fscanf(inputfh, "\n%d %d\n", &width, &height);

    int size = width * height * 3;

    // check if width and height not found
    if (scanCount == 0) {
        fprintf(stderr, "Error: width and height not found");
        return 3;
    }
    
    sprintf(widthAndHeight, "\n%d %d\n", width, height);
    
    // write out the dimensions
    fwrite(widthAndHeight, sizeof(widthAndHeight), strlen(widthAndHeight), outputfh);
    printf("\nWidth and height written");

// MAX COLOR VALUE

    // scan in the max color value
    char maxColorValStr[] = "";
    scanCount = fscanf(inputfh, "%d\n", &maxColorVal);

    // check if max color value not found
    if (scanCount == 0) {
        fprintf(stderr, "Error: max color value not found");
        return 3;
    }
    sprintf(maxColorValStr, "%d\n", maxColorVal);

    // write out the max color value
    fwrite(maxColorValStr, sizeof(maxColorValStr), strlen(maxColorValStr), outputfh);
    printf("\nMax color val written");

/*
// ADD COLOR VALUES
    
    int colorValue;
    int colorTriplet[3];
    int numColorVals = 0;
    int index = 0;

    // Loop through the read file
    while (index < width * height * 3) {
        colorValue = fgetc(inputfh); // TODO: this only gets a single char from the file. Maybe use get_short/get_long from Palmer's sgi code?
	colorTriplet[numColorVals] = colorValue;
	numColorVals++;

	// Check if color triplet has three values and add it to pixmap data structure
	if (numColorVals == 3) {
            // TODO: add color triplet to pixmap D.S.
            printf("%d", colorTriplet[numColorVals]);

	    // Reset number of color values
	    numColorVals = 0;
	}

        index++;
    }
    */
    fclose(inputfh);
    fclose(outputfh);

    return 0;

}
