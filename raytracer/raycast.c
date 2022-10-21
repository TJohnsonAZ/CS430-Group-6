#include "raycast.h"

int main(int argc, char** argv) {
    struct Object objects[128];
    
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

    int i = 0;
    while (!feof(inputfh) && i < 128) {
        fscanf(inputfh, "%s,", objName);
        if (strcmp(objName, "camera,") == 0) {
            // Camera's position is always [0, 0, 0]
            currentObject.objectKindFlag = CAMERA;
            currentObject.position[0] = 0;
            currentObject.position[1] = 0;
            currentObject.position[2] = 0;

            // set default values
            currentObject.width = 0;
            currentObject.height = 0;

            // Get first property value (width or height)
            fscanf(inputfh, "%s ", prop);
            if (strcmp(prop, "width:") == 0) {
                fscanf(inputfh, "%f,", &currentObject.width);
            }
            else if (strcmp(prop, "height:") == 0) {
                fscanf(inputfh, "%f,", &currentObject.height);
            }
            else {
                fprintf(stderr, "Error: camera has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
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
                fprintf(stderr, "Error: camera has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
            }
        }
        
        else if (strcmp(objName, "sphere,") == 0) {
            currentObject.objectKindFlag = SPHERE;

            // set default values
            currentObject.color[0] = 0;
            currentObject.color[1] = 0;
            currentObject.color[2] = 0;
            currentObject.position[0] = 0;
            currentObject.position[1] = 0;
            currentObject.position[2] = 0;
            currentObject.radius = 0;

            // Get first sphere property
            fscanf(inputfh, "%s", prop);
            if (strcmp(prop, "color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &currentObject.color[0], &currentObject.color[1], &currentObject.color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &currentObject.position[0], &currentObject.position[1], &currentObject.position[2]);
            }
            else if (strcmp(prop, "radius:") == 0) {
                fscanf(inputfh, " %f", &currentObject.radius);
            }
            else {
                fprintf(stderr, "1st Error: sphere has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
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
                fprintf(stderr, "2nd Error: sphere has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
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
                fprintf(stderr, "3rd Error: sphere has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
            }
        }
        
        else if (strcmp(objName, "plane,") == 0) {
            currentObject.objectKindFlag = PLANE;

            // set default values
            currentObject.color[0] = 0;
            currentObject.color[1] = 0;
            currentObject.color[2] = 0;
            currentObject.position[0] = 0;
            currentObject.position[1] = 0;
            currentObject.position[2] = 0;
            currentObject.pn[0] = 0;
            currentObject.pn[1] = 0;
            currentObject.pn[2] = 0;

            // get first plane property
            fscanf(inputfh, "%s", prop);
            if (strcmp(prop, "color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],\n", &currentObject.color[0], &currentObject.color[1], &currentObject.color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],\n", &currentObject.position[0], &currentObject.position[1], &currentObject.position[2]);
            }
            else if (strcmp(prop, "normal:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],\n", &currentObject.pn[0], &currentObject.pn[1], &currentObject.pn[2]);
            }
            else {
                fprintf(stderr, "1st Error: plane has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
            }

            // get second plane property
            fscanf(inputfh, "%s", prop);
            if (strcmp(prop, "color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],\n", &currentObject.color[0], &currentObject.color[1], &currentObject.color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],\n", &currentObject.position[0], &currentObject.position[1], &currentObject.position[2]);
            }
            else if (strcmp(prop, "normal:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],\n", &currentObject.pn[0], &currentObject.pn[1], &currentObject.pn[2]);
            }
            else {
                fprintf(stderr, "2nd Error: plane has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
            }

            // get third plane property
            fscanf(inputfh, "%s", prop);
            if (strcmp(prop, "color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f]\n", &currentObject.color[0], &currentObject.color[1], &currentObject.color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(inputfh, " [%f, %f, %f]\n", &currentObject.position[0], &currentObject.position[1], &currentObject.position[2]);
            }
            else if (strcmp(prop, "normal:") == 0) {
                fscanf(inputfh, " [%f, %f, %f]\n", &currentObject.pn[0], &currentObject.pn[1], &currentObject.pn[2]);
            }
            else {
                fprintf(stderr, "3rd Error: plane has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
            }
        }

        else {
            printf("You're at the plane\n");
            return 1;
        }

        objects[i] = currentObject;
        i++;
    }

    fclose(inputfh);

    if (i > 128) {
        fprintf(stderr, "Number of objects exceeds maximum value (128), remaining objects will be ignored.");
    }

    // Find the camera in the list of objects
    bool cameraFound = false;
    Object camera;
    for (int i = 0; !cameraFound; i++) {
        camera = objects[i];

        if (camera.objectKindFlag == CAMERA) {
            cameraFound = true;
        }
    }

    int imageHeight = atoi(argv[1]);
    int imageWidth = atoi(argv[2]);
    image = malloc(imageWidth * imageHeight * 3);

    int viewscreenWidth = camera.width;
    int viewscreenHeight = camera.height;

    float pixHeight = float(viewscreenHeight) / float(imageHeight);
    float pixWidth = float(viewscreenWidth) / float(viewscreenHeight);

    // Iterate through each row in the image
    for (int i = 0; i < imageHeight; i++) {
        // Get the current pixel's y-coord

        // Iterate through each column of the image
        for (int j = 0; j < imageWidth; j++) {
            // Get the current pixel's x-coord

            // Get the current pixels's z-coord?

            // Create vector out of pixel values

            // Normalize pixel vector

            // Shoot ray out into scene; return position of first hit
            // (requires seperate function)

            // Store color values of hit position into image
            // (requires seperate function)
        }
    }
    
    return 0;

}