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

void read_p3(FILE * fh, uint8_t *image) {
    int index = 0;
    int scanCount = 3;
    while (!feof(fh) && scanCount == 3) {
        int r;
        int g;
        int b;
        scanCount = fscanf(fh, "%u %u %u", &r, &g, &b);
        image[index] = r;
        image[index+1] = g;
        image[index+2] = b;
        index += 3;
    }
}

void read_p6(FILE* fh, int width, int height, uint8_t *image) {    
    for (int i = 0; i < width * height * 3; i++) {
        fread(&image[i], sizeof(uint8_t), 1, fh);
    }
}


void skipWS(FILE* fh) {
    char currentChar;
    fscanf(fh, "%c", &currentChar);
}

void write_p3(FILE* fh, int width, int height, int maxcol, uint8_t *image) {
    fprintf(fh, "P3 %d %d %d\n", width, height, maxcol);
    printf("Writing stuff...");

    for (int index = 0; index < width * height * 3 ; index += 3) {
        int val1 = image[index];
        int val2 = image[index + 1];
        int val3 = image[index + 2];
        fprintf(fh, "%d %d %d \n", val1, val2, val3);
    }
}

void write_p6(FILE* fh, int width, int height, int maxcol, uint8_t* image) {

    fprintf(fh, "P6 %d %d %d\n", width, height, maxcol);
    fwrite(image, sizeof(uint8_t), width * height * 3, fh);

}

int main(int argc, char** argv) {

    int width = 0, height = 0;
    int maxColorVal = 0;

    // check if command line has missing arguments
    if (argc != 4) {
        fprintf(stderr, "Error: Missing/too many arguments on command line");
        return 7;
    }

    // open the input file
    FILE* inputfh = fopen(argv[2], "r");

    // check if input file opened incorrectly
    if (inputfh == NULL) {
        fprintf(stderr, "Error: Input file not found");
        return 2;
    }

    // MAGIC NUMBER
    int c = fgetc(inputfh);
    if (c != 'P') {
        fprintf(stderr, "Error: magic number not found\n");
        return 3;
    }
    char inputFormat = fgetc(inputfh);
    if ((inputFormat != '3') && (inputFormat != '6')) {
        fprintf(stderr, "Error: magic number not found\n");
        return 3;
    }

    // Read width, height, and max color val; skip whitespaces/comments
    skipWS(inputfh);
    fscanf(inputfh, "%d", &width);
    
    skipWS(inputfh);
    fscanf(inputfh, "%d", &height);
    
    skipWS(inputfh);
    fscanf(inputfh, "%d", &maxColorVal);
    skipWS(inputfh);

    uint8_t image[width * height * 3];
    // Read input file depending on format
    if (inputFormat == '3') {
        read_p3(inputfh, image);
    }
    else {
        printf("%d\t%d\n", width, height);
        read_p6(inputfh, width, height, image);
        printf("%d\t%d\n", width, height);
    }

    // Write to output file depending on format
    int outputFormat = atoi(argv[1]);
    FILE* outputfh = fopen(argv[3], "w");
    if (outputfh == NULL) {
        fprintf(stderr, "Error: output file not found");
        return 2;
    }

    if (outputFormat == 3) {
        write_p3(outputfh, width, height, maxColorVal, image);
    }
    else {
        write_p6(outputfh, width, height, maxColorVal, image);
    }

    // close input and output files
    fclose(inputfh);
    fclose(outputfh);

    return 0;

}