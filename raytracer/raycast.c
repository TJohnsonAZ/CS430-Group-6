#include "raycast.h"
#include "v3math.h"

/*
* Reads object properties from input file
* Loops for all object properties and
* Stores values in passed object parameter
*/
void readProperties(FILE *inputfh, Object *curr_object) {
    char* prop = (char *)malloc(sizeof(char *));

    // loop to read all object properties
    bool finished = false;
    while (!finished && !feof(inputfh)) {
	    // read camera properties
        if (curr_object->objectKindFlag == CAMERA) {
            fscanf(inputfh, "%s ", prop);
            if (strcmp(prop, "width:") == 0) {
                fscanf(inputfh, "%f,", &curr_object->width);
            }
            else if (strcmp(prop, "height:") == 0) {
                fscanf(inputfh, "%f,", &curr_object->height);
            }
            else {
                fseek(inputfh, -strlen(prop) - 1, SEEK_CUR);
		    finished = true;
            }
        }
	    // read sphere properties
        else if (curr_object->objectKindFlag == SPHERE) {
            fscanf(inputfh, "%s", prop);
            if (strcmp(prop, "color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &curr_object->color[0], &curr_object->color[1], &curr_object->color[2]);
            }
	        else if (strcmp(prop, "diffuse_color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &curr_object->diffuse_color[0], &curr_object->diffuse_color[1], &curr_object->diffuse_color[2]);
            }
	        else if (strcmp(prop, "specular_color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &curr_object->specular_color[0], &curr_object->specular_color[1], &curr_object->specular_color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &curr_object->position[0], &curr_object->position[1], &curr_object->position[2]);
            }
            else if (strcmp(prop, "radius:") == 0) {
                fscanf(inputfh, " %f", &curr_object->radius);
            }
            else {
                fseek(inputfh, -strlen(prop) - 1, SEEK_CUR);
		        finished = true;
            }            
        }
        // read plane properties
        else if (curr_object->objectKindFlag == PLANE) {
            fscanf(inputfh, "%s", prop);
            if (strcmp(prop, "color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],\n", &curr_object->color[0], &curr_object->color[1], &curr_object->color[2]);
            }
	        else if (strcmp(prop, "diffuse_color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &curr_object->diffuse_color[0], &curr_object->diffuse_color[1], &curr_object->diffuse_color[2]);
            }
	        else if (strcmp(prop, "specular_color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &curr_object->specular_color[0], &curr_object->specular_color[1], &curr_object->specular_color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],\n", &curr_object->position[0], &curr_object->position[1], &curr_object->position[2]);
            }
            else if (strcmp(prop, "normal:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],\n", &curr_object->pn[0], &curr_object->pn[1], &curr_object->pn[2]);
            }
            else {
                fseek(inputfh, -strlen(prop) - 1, SEEK_CUR);
		    finished = true;
            }
        }
    }
}

/*
* Shoots ray from origin to current pixel
* Returns name of object that was hit by ray
* And stores the position of the hit point in a float pointer
*/
float* shoot(Object objects[], float* Rd, Object camera, float* hitObjectColor) {
    // float* hitObjectColor = (float*)malloc(sizeof(float*));
    hitObjectColor[0] = 0.0;
    hitObjectColor[1] = 0.0;
    hitObjectColor[2] = 0.0;

    // Get Ro, Rd
    float* Ro = camera.position;
  //  float Rd[] = {//(float*)malloc(sizeof(float*));
  //  pixVectorNormal[0] - Ro[0],
  //  pixVectorNormal[1] - Ro[1],
  //  pixVectorNormal[2] - Ro[2]
  //  };
    // Loop through objects
    float tValue = -1;
    for (int i = 0; objects[i].objectKindFlag != 0; i++) {
        // Check if valid t values found for sphere
        if (objects[i].objectKindFlag == SPHERE) {
            // Ray-sphere intersection test
            tValue = raysphereIntersection(objects[i], Ro, Rd);
            printf("%f\n", tValue);

            if (tValue > 0) {
                printf("THE RAY HIT!!!");
                hitObjectColor[0] = objects[i].color[0];
                hitObjectColor[1] = objects[i].color[1];
                hitObjectColor[2] = objects[i].color[2];
            }
        }
        // Check if valid t values found for plane
        else if (objects[i].objectKindFlag == PLANE) {
            // Ray-plane intersection test
            tValue = rayplaneIntersection(objects[i], Ro, Rd);

            if (tValue > 0.0) {
                printf("RAY HIT THE PLANE!!!");
                hitObjectColor[0] = objects[i].color[0];
                hitObjectColor[1] = objects[i].color[1];
                hitObjectColor[2] = objects[i].color[2];
            }
        }
        // Skip over camera object
        else if (objects[i].objectKindFlag == CAMERA) {
            continue;
        }
        else {
            break;
        }
    }

//    free(Rd);

    // Return black pixel values
    return hitObjectColor;
}

/*
* Stores the color values of the hit object in the image
*/
/*void shade(uint8_t* image, int x, int y, int width, float* hitObjectColor) {
    
}
void shade(uint8_t* image, int imageIndex, float* hitObjectColor) {
    image[imageIndex] = hitObjectColor[0] * 255;
    image[imageIndex + 1] = hitObjectColor[1] * 255;
    image[imageIndex + 2] = hitObjectColor[2] * 255;
}*/

/*
* Calculates the t value for a ray that hits a sphere
*/
float raysphereIntersection(Object sphere, float* Ro, float* Rd) {
    float a = (Rd[0] * Rd[0]) + (Rd[1] * Rd[1]) + (Rd[2] * Rd[2]);
    float b = 2 * (Rd[0] * (Ro[0] - sphere.position[0])
        + Rd[1] * (Ro[1] - sphere.position[1])
        + Rd[2] * (Ro[2] - sphere.position[2]));
    float c = ((Ro[0] - sphere.position[0]) * (Ro[0] - sphere.position[0]))
        + ((Ro[1] - sphere.position[1]) * (Ro[1] - sphere.position[1]))
        + ((Ro[2] - sphere.position[2]) * (Ro[2] - sphere.position[2]))
        - (sphere.radius * sphere.radius);

    //printf("Radius: %f\n", sphere.radius);
    //printf("Sphere position: %f %f %f\n", sphere.position[0], sphere.position[1], sphere.position[2]);
    //printf("Direction: %f %f %f\n", Rd[0], Rd[1], Rd[2]);
    //printf("Origin: %f %f %f\n", Ro[0], Ro[1], Ro[2]);
    /*if (b * b >= 80) {
        printf("b * b value: %.2f\t", b * b);
        printf("4 * c value: %.2f\n", 4 * c);
    }*/
    float discriminant = (b * b) - (4 * a * c);

    if (discriminant < 0.0) {
        return -1;
    }
    //printf("Discriminant: %f\n\n", discriminant);
    
        float tValue = (-b - sqrt(discriminant)) / (2 * a);
        if (tValue < 0.0) {
            tValue = (-b + sqrt(discriminant)) / (2 * a);
        }

        return tValue;
}

/*
* Calculates the t value for a ray that hits a plane
*/
float rayplaneIntersection(Object plane, float* Ro, float* Rd) {
    float numerator = ((plane.pn[0] * Ro[0]) + (plane.pn[1] * Ro[1]) + (plane.pn[2] * Ro[2]));
    numerator += plane.d;
    float denominator = (plane.pn[0] * Rd[0]) + (plane.pn[1] * Rd[1]) + (plane.pn[2] * Rd[2]);
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
// float raysphereIntersection(Object sphere, float* Ro, float* Rd);
int main(int argc, char** argv) {
    /*struct Object sphere;
    float Ro[] = {0, 0, 0};
    sphere.position[0] = 0;
    sphere.position[1] = 0;
    sphere.position[2] = -2;
    sphere.radius = 1;
    float Rd[] = {0, 0, -1};
    float tValue = raysphereIntersection(sphere, Ro, Rd);
    printf("%f\n", tValue);

    return 0;*/
    
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

	    readProperties(inputfh, &currentObject);
        }
        // Check if the object is a sphere
        else if (strcmp(objName, "sphere,") == 0) {
            currentObject.objectKindFlag = SPHERE;

            // Set default values for sphere
            currentObject.color[0] = 0;
            currentObject.color[1] = 0;
            currentObject.color[2] = 0;
            currentObject.diffuse_color[0] = 0;
            currentObject.diffuse_color[1] = 0;
            currentObject.diffuse_color[2] = 0;
            currentObject.specular_color[0] = 0;
            currentObject.specular_color[1] = 0;
            currentObject.specular_color[2] = 0;
            currentObject.position[0] = 0;
            currentObject.position[1] = 0;
            currentObject.position[2] = 0;
            currentObject.radius = 0;

            readProperties(inputfh, &currentObject);
        }
        // Check if the object is a plane
        else if (strcmp(objName, "plane,") == 0) {
            currentObject.objectKindFlag = PLANE;

            // Set default values for plane
            currentObject.color[0] = 0;
            currentObject.color[1] = 0;
            currentObject.color[2] = 0;
            currentObject.diffuse_color[0] = 0;
            currentObject.diffuse_color[1] = 0;
            currentObject.diffuse_color[2] = 0;
            currentObject.specular_color[0] = 0;
            currentObject.specular_color[1] = 0;
            currentObject.specular_color[2] = 0;
            currentObject.position[0] = 0;
            currentObject.position[1] = 0;
            currentObject.position[2] = 0;
            currentObject.pn[0] = 0;
            currentObject.pn[1] = 0;
            currentObject.pn[2] = 0;

	    readProperties(inputfh, &currentObject);

            currentObject.d = -(v3_dot_product(currentObject.position, currentObject.pn));
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
    float pixWidth = (float)viewscreenWidth / (float)imageWidth;

    int imageIndex = 0;
    // Iterate through each row in the image
    for (int y = 0; y < imageHeight; y++) {
        // Get the current pixel's y-coord
        float pixY = (-1 * viewscreenHeight) / 2 + pixHeight * (y + 0.5);

        // Iterate through each column of the image
        for (int x = 0; x < imageWidth; x++) {
            // Get the current pixel's x-coord
            float pixX = (-1 * viewscreenWidth) / 2 + pixWidth * (x + 0.5);

            // Get the current pixels's z-coord
            // (always 1 unit away from camera in the -z direction)
            float pixZ = -1;

            // Create vector out of pixel values
            float pixVector[] = {pixX, pixY, pixZ};

            // Normalize pixel vector
            float pixVectorNormal[3];
            v3_normalize(pixVectorNormal, pixVector);

            // Shoot ray out into scene; return color of object hit, or RGB values
            // for black if no object hit
            float hitObjectColor[3];
            shoot(objects, pixVectorNormal, camera, hitObjectColor);

            image[((imageHeight - y - 1) * imageWidth + x) * 3] = hitObjectColor[0] * 255;
            image[(((imageHeight - y - 1) * imageWidth + x) * 3) + 1] = hitObjectColor[1] * 255;
            image[(((imageHeight - y - 1) * imageWidth + x) * 3) + 2] = hitObjectColor[2] * 255;

            // Store color values of hit position into image
            // shade(image, imageIndex, hitObjectColor);

            // free(hitObjectColor);
            imageIndex += 3;
        }
    }

    // Write color values in image to ppm file with p3 format
    write_p3(argv[4], imageWidth, imageHeight, 255, image);
    
    return 0;

}
