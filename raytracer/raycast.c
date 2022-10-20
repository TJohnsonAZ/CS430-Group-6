#include "raycast.h"

int main(int argc, char** argv) {
    struct Object camera;
    struct Object *objects;
    
    // check if command line has missing arguments
    if (argc != 5) {
        fprintf(stderr, "Error: Missing/too many arguments on command line");
        return 1;
    }

    // open the input file
    FILE* inputfh = fopen(argv[3], "r");
    // check if input file opened incorrectly
    if (inputfh == NULL) {
        fprintf(stderr, "Error: Input file not found");
        return 2;
    }

    struct Object currentObject;
    char* objName = (char *)malloc(sizeof(char *));
    char* prop = (char *)malloc(sizeof(char *));

    while (!feof(inputfh)) {
        fscanf(inputfh, "%s,", objName);
        printf("+++%s", objName);
        if (strcmp(objName, "camera,") == 0) {
            printf("This is a camera\n");

            // Camera's position is [0, 0, 0], which we set here
            currentObject.objectKindFlag = CAMERA;
            currentObject.position[0] = 0;
            currentObject.position[1] = 0;
            currentObject.position[2] = 0;

            // Get first property value (width or height)
            fscanf(inputfh, "%s ", prop);
            if (strcmp(prop, "width:") == 0) {
                fscanf(inputfh, "%f,", &currentObject.width);
            }
            else if (strcmp(prop, "height:") == 0) {
                fscanf(inputfh, "%f,", &currentObject.height);
            }
            else {
                fprintf(stderr, "Error: camera has invalid property \"%s\"", prop);
                return 1;
            }

            // Get second property value (width or height)
            fscanf(inputfh, "%s", prop);
            if (strcmp(prop, "width:") == 0) {
                fscanf(inputfh, "%f\n", &currentObject.width);
            }
            else if (strcmp(prop, "height:") == 0) {
                fscanf(inputfh, "%f\n", &currentObject.height);
            }
            else {
                fprintf(stderr, "Error: camera has invalid property \"%s\"", prop);
                return 1;
            }
        }
        else if (strcmp(objName, "sphere,") == 0) {
            printf("This is a sphere\n");

            currentObject.objectKindFlag = SPHERE;

            // Get first sphere property
            fscanf(inputfh, "%s", prop);
            if (strcmp(prop, "color:") == 0) {
                printf("Getting color...\n");
                float colorVals[3];
                fscanf(inputfh, " [%f, %f, %f],", &currentObject.color[0], &currentObject.color[1], &currentObject.color[2]);
                printf("%f %f %f\n", currentObject.color[0], currentObject.color[1], currentObject.color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &currentObject.position[0], &currentObject.position[1], &currentObject.position[2]);
            }
            else if (strcmp(prop, "radius:") == 0) {
                fscanf(inputfh, " %f", &currentObject.radius);
            }
            else {
                fprintf(stderr, "1st Error: sphere has invalid property \"%s\"", prop);
                return 1;
            }
            
            fscanf(inputfh, "%s", prop);
            // Get second sphere property
            if (strcmp(prop, "color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &currentObject.color[0], &currentObject.color[1], &currentObject.color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &currentObject.position[0], &currentObject.position[1], &currentObject.position[2]);
            }
            else if (strcmp(prop, "radius:") == 0) {
                fscanf(inputfh, " %f,", &currentObject.radius);
            }
            else {
                fprintf(stderr, "2nd Error: sphere has invalid property \"%s\"", prop);
                return 1;
            }

            // Get third sphere property
            fscanf(inputfh, "%s", prop);
            if (strcmp(prop, "color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f]\n", &currentObject.color[0], &currentObject.color[1], &currentObject.color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(inputfh, " [%f, %f, %f]\n", &currentObject.position[0], &currentObject.position[1], &currentObject.position[2]);
            }
            else if (strcmp(prop, "radius:") == 0) {
                fscanf(inputfh, " %f\n", &currentObject.radius);
            }
            else {
                fprintf(stderr, "3rd Error: sphere has invalid property \"%s\"", prop);
                return 1;
            }
        }
        /*else if (strcmp(objName, "plane,") == 0) {
            printf("This is a plane");

            currentObject.objectKindFlag = PLANE;

            fscanf(inputfh, "%s", prop);
            if (strcmp(prop, "width:") == 0) {
                fscanf(inputfh, "%f", currentObject.width);
            }
            else if (strcmp(objName, "height:") == 0) {
                fscanf(prop, "%f", currentObject.height);
            }
            else {
                fprintf(stderr, "Error: plane has invalid properties");
                return 1;
            }
        }*/
        else {
            printf("You're at the plane\n");
            return 1;
        }
    }
    
    // Create object of type found in input file
    /*while (!feof(inputfh)) {
        
    } */

    return 0;

}