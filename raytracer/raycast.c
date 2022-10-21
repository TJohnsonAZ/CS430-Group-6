#include "raycast.h"
#include "v3math.h"

void shoot(float* normalVector, float* hitPoint, Object camera, Object objects[]) {
    float* originPoint = camera.position;
    float tValue;

    // Loop through list of objects
    for (int i = 0; i < 128; i++) {
        if (objects[i].objectKindFlag == PLANE) {
            // rayplaneIntersection(objects[i]);
        }
        else if (objects[i].objectKindFlag == SPHERE) {
            tValue = raysphereIntersection(hitPoint, objects[i], normalVector, originPoint);
            hitPoint[0] = originPoint[0] + objects[i].position[0] * tValue;
            hitPoint[1] = originPoint[1] + objects[i].position[1] * tValue;
            hitPoint[2] = originPoint[2] + objects[i].position[2] * tValue;
        }
    }
}

void shade(uint8_t *image, float* hitPoint, Object camera, Object objects[]) {

}

float raysphereIntersection(float* hitPoint, Object sphere, float* normalVector, float* originPoint) {
    float tValue;
    
    float a = (normalVector[0] * normalVector[0]) + (normalVector[1] * normalVector[1]) + (normalVector[2] * normalVector[2]);
    float b = 2 * (normalVector[0] * (originPoint[0] - sphere.position[0])
        + normalVector[1] * (originPoint[1] - sphere.position[1])
        + normalVector[2] * (originPoint[2] - sphere.position[2]));
    float c = ((originPoint[0] - sphere.position[0]) * (originPoint[0] - sphere.position[0]))
        + ((originPoint[1] - sphere.position[1]) * (originPoint[1] - sphere.position[1]))
        + ((originPoint[2] - sphere.position[2]) * (originPoint[2] - sphere.position[2]));

    float discriminant = (b * b) - (4 * a * c);
    tValue = (-b - sqrt(abs(discriminant))) / (2 * a);
    if (tValue < 0) {
        tValue = (-b + sqrt(abs(discriminant))) / (2 * a);
    }

    return tValue;
}

void rayplaneIntersection(Object plane, Object camera) {
    // float* tValue = (-1 * v3_dot_product(plane.pn, (camera.position, )))
}

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

    if (i > 128) {
        fprintf(stderr, "Number of objects exceeds maximum value (128), remaining objects will be ignored.");
    }
    fclose(inputfh);
    free(objName);
    free(prop);

    FILE* outputfh = fopen(argv[4], "w");

    // Find the camera in the list of objects
    Object camera;
    bool cameraFound = false;
    for (int i = 0; !cameraFound; i++) {
        camera = objects[i];

        if (camera.objectKindFlag == CAMERA) {
            cameraFound = true;
        }
    }

    int imageHeight = atoi(argv[1]);
    int imageWidth = atoi(argv[2]);
    uint8_t image[imageWidth * imageHeight * 3];

    int viewscreenWidth = camera.width;
    int viewscreenHeight = camera.height;

    float pixHeight = (float) viewscreenHeight / (float)imageHeight;
    float pixWidth = (float)viewscreenWidth / (float)viewscreenHeight;

    // Iterate through each row in the image
    for (int i = 0; i < imageHeight; i++) {
        // Get the current pixel's y-coord
        float pixY = 0 - viewscreenHeight / 2 + pixHeight * (i + 0.5);

        // Iterate through each column of the image
        for (int j = 0; j < imageWidth; j++) {
            // Get the current pixel's x-coord
            float pixX = 0 - viewscreenWidth / 2 + pixWidth * (j + 0.5);

            // Get the current pixels's z-coord?
            float pixZ = -1;

            // Create vector out of pixel values
            float pixVector[] = {pixX, pixY, pixZ};

            // Normalize pixel vector
            float pixVectorNormal[3];
            v3_normalize(pixVectorNormal, pixVector);

            // Shoot ray out into scene; return position of first hit
            // (requires seperate function)
            float* hitPoint = (float *)malloc(sizeof(float *));
            shoot(pixVectorNormal, hitPoint, camera, objects);

            // Store color values of hit position into image
            // (requires seperate function)
            shade(image, hitPoint, camera, objects);

            free(hitPoint);
        }
    }

    fclose(outputfh);
    
    return 0;

}