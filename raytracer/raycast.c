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
                fscanf(inputfh, " %f,", &curr_object->radius);
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
            else if (strcmp(prop, "normal:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &curr_object->pn[0], &curr_object->pn[1], &curr_object->pn[2]);
            }
            else {
                fseek(inputfh, -strlen(prop) - 1, SEEK_CUR);
                finished = true;
            }
        }
	// read light properties
	else if (curr_object->objectKindFlag == LIGHT) {
            fscanf(inputfh, "%s", prop);
            if (strcmp(prop, "color:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &curr_object->color[0], &curr_object->color[1], &curr_object->color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &curr_object->position[0], &curr_object->position[1], &curr_object->position[2]);
            }
	    else if (strcmp(prop, "radial-a0:") == 0) {
                fscanf(inputfh, " %f,", &curr_object->radial_a0);
	    }
	    else if (strcmp(prop, "radial-a1:") == 0) {
                fscanf(inputfh, " %f,", &curr_object->radial_a1);
	    }
	    else if (strcmp(prop, "radial-a2:") == 0) {
                fscanf(inputfh, " %f,", &curr_object->radial_a2);
	    }
	    else if (strcmp(prop, "angular-a0:") == 0) {
                fscanf(inputfh, " %f,", &curr_object->angular_a0);
	    }
	    else if (strcmp(prop, "theta:") == 0) {
                fscanf(inputfh, " %f,", &curr_object->theta);
	    }
            else if (strcmp(prop, "direction:") == 0) {
                fscanf(inputfh, " [%f, %f, %f],", &curr_object->direction[0], &curr_object->direction[1], &curr_object->direction[2]);
            }
	    else {
                fseek(inputfh, -strlen(prop) - 1, SEEK_CUR);
                finished = true;
	    }
	}
    }

    free(prop);
}

float *illuminate(float *Rd, float *point, Object *lights, Object object, float *color) {
    // If no object was hit, return given color (presumably black)
    if (object.objectKindFlag == DEFAULT) {
        return color;
    }
    
    // Loop through all lights found
    for (int i = 0; lights[i].objectKindFlag != DEFAULT; i++) {
        // Shoot ray from point to current light's position;
        // If t value indicates point is in shadow, continue
        /*float* lightRo;
        lightRo[0] = point[0];
        lightRo[1] = point[1];
        lightRo[2] = point[2];

        float* lightRd;
        float* lightRdNormal;
        v3_from_points(lightRd, lights[i].position, point);
        v3_normalize(lightRdNormal, lightRd);
        
        float tValue = 0.0;*/
        

        // Create light vector: L (vector from light to point)
        float L[3];
        v3_subtract(L, point, lights[i].position);

        // Calculate normal of light vector
	float N[3];
	v3_normalize(N, L);

        // Calculate radial attenuation
        float d = v3_length(L);
        float radialAtt = 1 / (lights[i].radial_a0 + lights[i].radial_a1 * d + lights[i].radial_a2 * d * d);

        // Calculate angular attenuation
        float v0[3], vL[3];
        v0[0] = L[0] * -1;
        v0[1] = L[1] * -1;
        v0[2] = L[2] * -1;
        float angularAtt = pow(v3_dot_product(v0, vL), lights[i].angular_a0);

        // Diffuse calculations
	float Id_dot = v3_dot_product(L, N);
	float Id[3];
	if (Id_dot > 0) {
	    Id[0] = -(Id_dot) * lights[i].color[0] * object.diffuse_color[0];
	    Id[1] = -(Id_dot) * lights[i].color[1] * object.diffuse_color[1];
	    Id[2] = -(Id_dot) * lights[i].color[2] * object.diffuse_color[2];
	}
	else {
	    Id[0] = 0;
	    Id[1] = 0;
	    Id[2] = 0;
	}	

        float diffuse[3];
        diffuse[0] = Id[0] * object.diffuse_color[0];
        diffuse[1] = Id[1] * object.diffuse_color[1];
        diffuse[2] = Id[2] * object.diffuse_color[2];

        // Add diffuse color to provided color values
	float attn = radialAtt + angularAtt;
	color[0] += diffuse[0] * attn;
	color[1] += diffuse[1] * attn;
	color[2] += diffuse[2] * attn;

        // Specular calculations
	float R[3];
	v3_reflect(R, L, N); 

	float V[3];
	V[0] = -(Rd[0]);
	V[1] = -(Rd[1]);
	V[2] = -(Rd[2]);

	float Is_dot = v3_dot_product(V, R);
	float Is[3];
	if (Is_dot > 0 && Id_dot > 0) {
	    float Is_dot_ex = pow(Is_dot, *N); 
            Is[0] = Is_dot_ex * lights[i].color[0] * object.specular_color[0];
            Is[1] = Is_dot_ex * lights[i].color[1] * object.specular_color[1];
            Is[2] = Is_dot_ex * lights[i].color[2] * object.specular_color[2];
	}
	else {
            Is[0] = 0;
            Is[1] = 0;
            Is[2] = 0;
	}

	float specular[3];
	specular[0] = Is[0] * object.specular_color[0];
	specular[1] = Is[1] * object.specular_color[1];
	specular[2] = Is[2] * object.specular_color[2];

        // Add specular color to provided color values
        color[0] += specular[0] * attn;	
        color[1] += specular[1] * attn;	
        color[2] += specular[2] * attn;	
    }

    return color;
}

/*
* Shoots ray from origin to current pixel
* Returns name of object that was hit by ray
* And stores the position of the hit point in a float pointer
*/
Object shoot(Object objects[], float* Rd, Object camera, float* hitObjectColor) {
    Object hitObject;
    
    hitObjectColor[0] = 0.0;
    hitObjectColor[1] = 0.0;
    hitObjectColor[2] = 0.0;

    // Get Ro, Rd
    float* Ro = camera.position;
    // Loop through objects
    float tValue = -1;
    for (int i = 0; objects[i].objectKindFlag != 0; i++) {
        // Check if valid t values found for sphere
        if (objects[i].objectKindFlag == SPHERE) {
            // Ray-sphere intersection test
            tValue = raysphereIntersection(objects[i], Ro, Rd);

            if (tValue > 0) {
                hitObjectColor[0] = objects[i].color[0];
                hitObjectColor[1] = objects[i].color[1];
                hitObjectColor[2] = objects[i].color[2];

                hitObject = objects[i];
            }
        }
        // Check if valid t values found for plane
        else if (objects[i].objectKindFlag == PLANE) {
            // Ray-plane intersection test
            tValue = rayplaneIntersection(objects[i], Ro, Rd);

            if (tValue > 0.0) {
                hitObjectColor[0] = objects[i].color[0];
                hitObjectColor[1] = objects[i].color[1];
                hitObjectColor[2] = objects[i].color[2];

                hitObject = objects[i];
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

    // Return hit object
    return hitObject;
}

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

    float discriminant = (b * b) - (4 * a * c);

    if (discriminant < 0.0) {
        return -1;
    }
    
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
    struct Object objects[128];
    struct Object lights[128];
    
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
        // Check if the object is a light
        else if (strcmp(objName, "light,") == 0) {
            currentObject.objectKindFlag = LIGHT;

	    // set default values for light
            currentObject.color[0] = 0;
            currentObject.color[1] = 0;
            currentObject.color[2] = 0;
	    currentObject.position[0] = 0;
	    currentObject.position[1] = 0;
	    currentObject.position[2] = 0;
	    currentObject.radial_a0 = 0;
	    currentObject.radial_a1 = 0;
	    currentObject.radial_a2 = 0;
	    currentObject.theta = 0;
	    currentObject.angular_a0 = 0;
	    currentObject.direction[0] = 0;
	    currentObject.direction[1] = 0;
	    currentObject.direction[2] = 0;

	    readProperties(inputfh, &currentObject);
	}
        // Assume it is an unknown object and throw error
        else {
            fprintf(stderr, "Error: unknown object found: %s\n", objName);
            return 1;
        }

        // Add current object to list of objects
	if (currentObject.objectKindFlag == LIGHT) {
            lights[objectArrayIndex] = currentObject;
	}
	else {
            objects[objectArrayIndex] = currentObject;
	}
        objectArrayIndex++;
    }
    
    if (objectArrayIndex > 128) {
        fprintf(stderr, "Number of objects exceeds maximum value (128), remaining objects will be ignored.");
    }
    fclose(inputfh);
    free(objName);

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

            // Shoot ray out into scene; if object hit, return object and its color
            float hitObjectColor[3];
            Object hitObject;
            hitObject = shoot(objects, pixVectorNormal, camera, hitObjectColor);

            // Get illuminate value from illuminate function
            illuminate(pixVectorNormal, pixVector, lights, hitObject, hitObjectColor);

            // Color current pixel in image  
            image[((imageHeight - y - 1) * imageWidth + x) * 3] = hitObjectColor[0] * 255;
            image[(((imageHeight - y - 1) * imageWidth + x) * 3) + 1] = hitObjectColor[1] * 255;
            image[(((imageHeight - y - 1) * imageWidth + x) * 3) + 2] = hitObjectColor[2] * 255;

            imageIndex += 3;
        }
    }

    // Write color values in image to ppm file with p3 format
    write_p3(argv[4], imageWidth, imageHeight, 255, image);
    
    return 0;
}
