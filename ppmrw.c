// four functionalities
// 1. Read P3 files into RGBA ordered memory
// 2. Read P6 files into RGBA ordered memory
// 3. Write P3 files from RGBA ordered memory
// 4. Write P6 files from RGBA ordered memory

#include <stdio.h> //fopen, fscanf, ect
#include <stdlib.h> // malloc, ect
#include <stdint.h> // standard integer types
#include <ctype.h> // isspace, ect
#include <string.h>
#include <stdbool.h>

void readFromP3(FILE* fh, int size, uint32_t *asciiArray) {
    int index = 0;
    int scanCount = 1;
    while (!feof(fh) && scanCount == 1) {
        scanCount = fscanf(fh, "%u", &asciiArray[index]); 
        index++;
    }
}

void readFromP6(FILE* fh, int size, uint8_t *binaryArray) {
    fread(&binaryArray, sizeof(uint8_t), size, fh);
}


void skipComment(FILE* fh) {
    char currentChar[1];
    while (currentChar[0] != '\n') {
        fread(currentChar, 1, 1, fh);
    }
}

void writeToP3FromP3(FILE* fh, int width, int size, uint32_t outputArray[]) {

    int index;

    for (index = 0; index < size; index++) {
        
        fprintf(fh, "%d %d %d ", outputArray[index], outputArray[index+1], outputArray[index+2]);\

        index += 3;

        if ((index / 3) == width) {

            fprintf(fh, "\n");

        }
    }
}

void writeToP3FromP6(FILE* fh, int width, int size, uint8_t outputArray[]) {

    int index;
    for (index = 0; index < size; index++) {

        fprintf(fh, "%d %d %d ", outputArray[index], outputArray[index+1], outputArray[index+2]);

        index += 3;

        if ((index / 3) == width) {

            fprintf(fh, "\n");

        }
    }
}

void writeToP6FromP3(FILE* fh, int size, uint32_t outputArray[]) {

    fwrite(outputArray, sizeof(uint8_t), size, fh);
}


void writeToP6FromP6(FILE* fh, int size, uint8_t outputArray[]) {
    
    fwrite(outputArray, sizeof(uint8_t), size, fh);
}

int main (int argc, char **argv) {

    int width = 0, height = 0;
    int maxColorVal = 0;

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

    char currentChar[1];
    char magicNumChar[2];
    bool skippedComment = false;

    while (!skippedComment) {
        fread(currentChar, 1, 1, inputfh);
        if (strcmp(currentChar, "P") == 0) {
            skippedComment = true;
        }
        else {
            if (ispunct(currentChar[0])) {
                skipComment(inputfh);
            }
            else {
                fprintf(stderr, "Error: Magic number not found in input file\n");
                return 3;
            }
        }
    }
    
    fread(magicNumChar, 1, 1, inputfh);
    
    // Write the magic number to the output file
    sprintf(magicNumStr, "P%s", argv[1]);
    fwrite(magicNumStr, sizeof(magicNumStr), strlen(magicNumStr), outputfh);

    // display final message
    printf("\nConversion Complete\n");
    
// DIMENSIONS

    // scan in the dimensions of the image
    char widthAndHeight[] = "";
    int scanCount = fscanf(inputfh, "\n%d %d\n", &width, &height);

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
    printf("\nMax color val written\n");

// READ AND WRITE COLOR VALUES
    
    // check what file is being read from
    uint32_t asciiArray[size];
    uint8_t binaryArray[size];
    
    if (strcmp(magicNumChar, "3") == 0) {
        readFromP3(inputfh, size, asciiArray);

        // check what file to write to
        if (strcmp(argv[1], "6") == 0) {
            writeToP6FromP3(outputfh, size, asciiArray);
        }
        else if (strcmp(argv[1], "3") == 0) {
            writeToP3FromP3(outputfh, width, size, asciiArray);
        }
    }
    else if (strcmp(magicNumChar, "6") == 0) {
        readFromP6(inputfh, size, binaryArray);

        // check what file to write to
        if (strcmp(argv[1], "6") == 0) {
            writeToP6FromP6(outputfh, size, binaryArray);
        }
        else if (strcmp(argv[1], "3") == 0) {
            writeToP3FromP6(outputfh, width, size, binaryArray);
        }
    }

    // close input and output files
    fclose(inputfh);
    fclose(outputfh);

    return 0;

}
