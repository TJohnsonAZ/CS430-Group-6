#include "raycast.h"
#include "v3math.h"

/*
* Shoots ray from origin to current pixel
* Returns name of object that was hit by ray
* And stores the position of the hit point in a float pointer
*/
Object shoot(float* normalVector, float* hitPoint, Object camera, Object objects[]) {
    float* originPoint = camera.position;
    float tValue;
    
    // Calculate the direction vector of ray shot toward object and normalize it
    float* directionVect = (float *)malloc(sizeof(float *));
    float* directionVectNormal = (float*)malloc(sizeof(float*));
    directionVect[0] = originPoint[0] - normalVector[0];
    directionVect[1] = originPoint[1] - normalVector[1];
    directionVect[2] = originPoint[2] - normalVector[2];
    v3_normalize(directionVectNormal, directionVect);

    // Loop through list of objects
    int i;
    for (i = 0; i < 128; i++) {
        // Check if current object is a plane
        if (objects[i].objectKindFlag == PLANE) {
            // Calculate closest t value for a ray intersecting with a plane
            tValue = rayplaneIntersection(objects[i], camera, normalVector);
        }
        // Check if current object is a sphere
        else if (objects[i].objectKindFlag == SPHERE) {
            // Calculate closest t value for a ray intersecting with a sphere
            tValue = raysphereIntersection(objects[i], normalVector, originPoint);
        }

        // Calcuate the hit point based on the equation R(t) = R_0 + R_d*t,
            // where R_0 is the ray origin (the camera's position), R_d is the 
            // ray direction 
        hitPoint[0] = originPoint[0] + directionVectNormal[0] * tValue;
        hitPoint[1] = originPoint[1] + directionVectNormal[1] * tValue;
        hitPoint[2] = originPoint[2] + directionVectNormal[2] * tValue;

        break;
    }
    
    free(directionVect);
    free(directionVectNormal);
    return objects[i];
}

/*
* Stores the color values of the hit object in the image
*/
void shade(uint8_t* image, int imageIndex, Object hitObject, float* hitPoint) {
    // The current pixel is black by default; it a hit point was
    // found, then an object of a certain color was found and the
    // current pixel's color needs to change
    if (hitPoint[0] > 0 && hitPoint[1] > 0 && hitPoint[2] > 0) {
        image[imageIndex] = hitObject.color[0] * 255;
        image[imageIndex + 1] = hitObject.color[1] * 255;
        image[imageIndex + 2] = hitObject.color[2] * 255;
    }
}

/*
* Calculates the t value for a ray that hits a sphere
*/
float raysphereIntersection(Object sphere, float* normalVector, float* originPoint) {
    // The coefficients of the algebraic equation At^2 + Bt + C = 0 allow for the closest t value to be found
    float a = (normalVector[0] * normalVector[0]) + (normalVector[1] * normalVector[1]) + (normalVector[2] * normalVector[2]);
    float b = 2 * (normalVector[0] * (originPoint[0] - sphere.position[0])
        + normalVector[1] * (originPoint[1] - sphere.position[1])
        + normalVector[2] * (originPoint[2] - sphere.position[2]));
    float c = ((originPoint[0] - sphere.position[0]) * (originPoint[0] - sphere.position[0]))
        + ((originPoint[1] - sphere.position[1]) * (originPoint[1] - sphere.position[1]))
        + ((originPoint[2] - sphere.position[2]) * (originPoint[2] - sphere.position[2]));

    // The t value can be found by using the quadratic formula on At^2 + Bt + C = 0
    // Calculate the discriminant and plug it into the quadratic formula to get the correct t value
    float discriminant = (b * b) - (4 * a * c);
    float tValue = (-b - sqrt(abs(discriminant))) / (2 * a);
    
    // If the t value is less than 0, then the hit point is behind the camera
    // Calculate the other t value if this is the case
    if (tValue < 0) {
        tValue = (-b + sqrt(abs(discriminant))) / (2 * a);
    }

    return tValue;
}

/*
* Calculates the t value for a ray that hits a plane
*/
float rayplaneIntersection(Object plane, Object camera, float *normalVector) {
    float numerator = ((plane.pn[0] * camera.position[0]) + (plane.pn[1] * camera.position[1]) + (plane.pn[2] * camera.position[2]));
    numerator += plane.d;
    float denominator = (plane.pn[0] * normalVector[0]) + (plane.pn[1] * normalVector[1]) + (plane.pn[2] * normalVector[2]);
    float tValue = -(numerator / denominator);
                                            
    return tValue;                     
}

/*
* Writes the header and color values from the given image into 
* a ppm file with a P3 format
*/
bool write_p3(char* fileName, int width, int height, int maxcol, uint8_t* image) {
    // Open file and check that it was open correctly
    FILE* fh = fopen(fileName, "w");
    if (fh == NULL) {
        fprintf(stderr, "Error: Output file not found");
        return false;
    }
    
    // Print the header of a P3 ppm file
    fprintf(fh, "P3 %d %d %d\n", width, height, maxcol);

    // Write the color values found in the image
    for (int index = 0; index < width * height * 3; index += 3) {
        int val1 = image[index];
        int val2 = image[index + 1];
        int val3 = image[index + 2];
        fprintf(fh, "%d %d %d \n", val1, val2, val3);
    }

    fclose(fh);
    return true;
}

int main(int argc, char** argv) {
    struct Object objects[128];
    
    // Check if command line has missing arguments
    if (argc != 5) {
        fprintf(stderr, "Error: Missing/too many arguments on command line");
        return 1;
    }

    // Open the input file & check if it opened incorrectly
    FILE* inputfh = fopen(argv[3], "r");
    if (inputfh == NULL) {
        fprintf(stderr, "Error: Input file not found");
        return 1;
    }

    struct Object currentObject;
    char* objName = (char *)malloc(sizeof(char *));
    char* prop = (char *)malloc(sizeof(char *));

    int objectArrayIndex = 0;
    // Loop through the input file
    while (!feof(inputfh) && objectArrayIndex < 128) {
        // Get the name of the object at beginning of current line
        fscanf(inputfh, "%s,", objName);
        // Check if the object is a camera
        if (strcmp(objName, "camera,") == 0) {
            // Set default values for camera
            currentObject.objectKindFlag = CAMERA;
            currentObject.position[0] = 0;
            currentObject.position[1] = 0;
            currentObject.position[2] = 0;
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
        // Check if the object is a sphere
        else if (strcmp(objName, "sphere,") == 0) {
            currentObject.objectKindFlag = SPHERE;

            // Set default values for sphere
            currentObject.color[0] = 0;
            currentObject.color[1] = 0;
            currentObject.color[2] = 0;
            currentObject.position[0] = 0;
            currentObject.position[1] = 0;
            currentObject.position[2] = 0;
            currentObject.radius = 0;

            // Get first sphere property (color, position, or radius); throw error if none are found
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
                fprintf(stderr, "Error: sphere has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
            }
            
            // Get second sphere property (color, position, or radius); throw error if none are found
            fscanf(inputfh, "%s", prop);
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
                fprintf(stderr, "Error: sphere has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
            }

            // Get third sphere property (color, position, or radius); throw error if none are found
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
                fprintf(stderr, "Error: sphere has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
            }
        }
        // Check if the object is a plane
        else if (strcmp(objName, "plane,") == 0) {
            currentObject.objectKindFlag = PLANE;

            // Set default values for plane
            currentObject.color[0] = 0;
            currentObject.color[1] = 0;
            currentObject.color[2] = 0;
            currentObject.position[0] = 0;
            currentObject.position[1] = 0;
            currentObject.position[2] = 0;
            currentObject.pn[0] = 0;
            currentObject.pn[1] = 0;
            currentObject.pn[2] = 0;

            // Get first plane property (color, position, or normal); throw error if none are found
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
                fprintf(stderr, "Error: plane has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
            }

            // Get second plane property (color, position, or normal); throw error if none are found
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
                fprintf(stderr, "Error: plane has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
            }

            // Get third plane property (color, position, or normal); throw error if none are found
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
                fprintf(stderr, "Error: plane has missing or invalid property \"%s\"\n", prop);
                fseek(inputfh, -strlen(prop), SEEK_CUR);
            }

            float *position = currentObject.position;
            currentObject.d = sqrt((position[0] * position[0]) + (position[1] * position[1])
                                                                              + (position[2] * position[2]));
        }
        // Assume it is an unknown object and throw error
        else {
            fprintf(stderr, "Error: unknown object found: %s\n", objName);
            return 1;
        }

        // Add current object to list of objects
        objects[objectArrayIndex] = currentObject;
        objectArrayIndex++;
    }

    if (objectArrayIndex > 128) {
        fprintf(stderr, "Number of objects exceeds maximum value (128), remaining objects will be ignored.");
    }
    fclose(inputfh);
    free(objName);
    free(prop);

    // Find the camera in the list of objects
    Object camera;
    bool cameraFound = false;
    for (int i = 0; !cameraFound; i++) {
        camera = objects[i];

        if (camera.objectKindFlag == CAMERA) {
            cameraFound = true;
        }
    }

    // Get data about image size
    int imageHeight = atoi(argv[1]);
    int imageWidth = atoi(argv[2]);
    uint8_t image[imageWidth * imageHeight * 3];

    // Get data about viewscreen size from camera
    int viewscreenWidth = camera.width;
    int viewscreenHeight = camera.height;

    // Calculate pixel width and height based on viewscreen & image size
    float pixHeight = (float)viewscreenHeight / (float)imageHeight;
    float pixWidth = (float)viewscreenWidth / (float)viewscreenHeight;

    int imageIndex = 0;
    // Iterate through each row in the image
    for (int i = 0; i < imageHeight; i++) {
        // Get the current pixel's y-coord
        float pixY = (-1 * viewscreenHeight) / 2 + pixHeight * (i + 0.5);

        // Iterate through each column of the image
        for (int j = 0; j < imageWidth; j++) {
            // Get the current pixel's x-coord
            float pixX = (-1 * viewscreenWidth) / 2 + pixWidth * (j + 0.5);

            // Get the current pixels's z-coord
            // (always 1 unit away from camera in the -z direction)
            float pixZ = -1;

            // Create vector out of pixel values
            float pixVector[] = {pixX, pixY, pixZ};

            // Normalize pixel vector
            float pixVectorNormal[3];
            v3_normalize(pixVectorNormal, pixVector);

            // Shoot ray out into scene; return name of object hit & position of hit
            float* hitPoint = (float *)malloc(sizeof(float *));
            Object hitObject = shoot(pixVectorNormal, hitPoint, camera, objects);

            // Store color values of hit position into image
            shade(image, imageIndex, hitObject, hitPoint);

            free(hitPoint);
            imageIndex += 3;
        }
    }

    // Write color values in image to ppm file with p3 format
    write_p3(argv[4], imageWidth, imageHeight, 255, image);
    
    return 0;

}