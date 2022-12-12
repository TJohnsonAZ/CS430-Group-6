#include "raytrace.h"
#include "v3math.h"

/*
* Reads object properties from input file
* Loops for all object properties and
* Stores values in passed object parameter
*/
void read_properties(FILE *input_fh, Object *curr_object) {
    char *prop = (char *)malloc(sizeof(char *));

    // Loop to read all object properties
    bool finished = false;
    while (!finished && !feof(input_fh)) {
        // Read camera properties
        if (curr_object->object_kind_flag == CAMERA) {
            // Get the name of the current property
            fscanf(input_fh, "%s ", prop);
            
            // Check what kind of property was found in the input file and store it in its respective value
            if (strcmp(prop, "width:") == 0) {
                fscanf(input_fh, "%f,", &curr_object->width);
            }
            else if (strcmp(prop, "height:") == 0) {
                fscanf(input_fh, "%f,", &curr_object->height);
            }
            else {
                fseek(input_fh, -strlen(prop) - 1, SEEK_CUR);
                finished = true;
            }
        }
        // Read sphere properties
        else if (curr_object->object_kind_flag == SPHERE) {
            // Get the name of the current property
            fscanf(input_fh, "%s", prop);
            
            // Check what kind of property was found in the input file and store it in its respective value
            if (strcmp(prop, "diffuse_color:") == 0) {
                fscanf(input_fh, " [%f, %f, %f],", &curr_object->diffuse_color[0], &curr_object->diffuse_color[1], &curr_object->diffuse_color[2]);
            }
            else if (strcmp(prop, "specular_color:") == 0) {
                fscanf(input_fh, " [%f, %f, %f],", &curr_object->specular_color[0], &curr_object->specular_color[1], &curr_object->specular_color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(input_fh, " [%f, %f, %f],", &curr_object->position[0], &curr_object->position[1], &curr_object->position[2]);
            }
            else if (strcmp(prop, "reflectivity:") == 0) {
                fscanf(input_fh, " %f,", &curr_object->reflectivity);
            }
            else if (strcmp(prop, "radius:") == 0) {
                fscanf(input_fh, " %f,", &curr_object->radius);
            }
            else {
                fseek(input_fh, -strlen(prop) - 1, SEEK_CUR);
                finished = true;
            }            
        }
        // Read plane properties
        else if (curr_object->object_kind_flag == PLANE) {
            // Get the name of the current property
            fscanf(input_fh, "%s", prop);
            
            // Check what kind of property was found in the input file and store it in its respective value
            if (strcmp(prop, "diffuse_color:") == 0) {
                fscanf(input_fh, " [%f, %f, %f],", &curr_object->diffuse_color[0], &curr_object->diffuse_color[1], &curr_object->diffuse_color[2]);
            }
            else if (strcmp(prop, "specular_color:") == 0) {
                fscanf(input_fh, " [%f, %f, %f],", &curr_object->specular_color[0], &curr_object->specular_color[1], &curr_object->specular_color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(input_fh, " [%f, %f, %f],", &curr_object->position[0], &curr_object->position[1], &curr_object->position[2]);
            }
            else if (strcmp(prop, "normal:") == 0) {
                fscanf(input_fh, " [%f, %f, %f],", &curr_object->plane_normal[0], &curr_object->plane_normal[1], &curr_object->plane_normal[2]);
            }
            else if (strcmp(prop, "reflectivity:") == 0) {
                fscanf(input_fh, " %f,", &curr_object->reflectivity);
            }
            else {
                fseek(input_fh, -strlen(prop) - 1, SEEK_CUR);
                finished = true;
            }
        }
	// Read light properties
	else if (curr_object->object_kind_flag == LIGHT) {
            // Get the name of the current property
            fscanf(input_fh, "%s", prop);
            
            // Check what kind of property was found in the input file and store it in its respective value
            if (strcmp(prop, "color:") == 0) {
                fscanf(input_fh, " [%f, %f, %f],", &curr_object->color[0], &curr_object->color[1], &curr_object->color[2]);
            }
            else if (strcmp(prop, "position:") == 0) {
                fscanf(input_fh, " [%f, %f, %f],", &curr_object->position[0], &curr_object->position[1], &curr_object->position[2]);
            }
            else if (strcmp(prop, "radial-a0:") == 0) {
                fscanf(input_fh, " %f,", &curr_object->radial_a0);
            }
            else if (strcmp(prop, "radial-a1:") == 0) {
                fscanf(input_fh, " %f,", &curr_object->radial_a1);
            }
            else if (strcmp(prop, "radial-a2:") == 0) {
                fscanf(input_fh, " %f,", &curr_object->radial_a2);
            }
            else if (strcmp(prop, "angular-a0:") == 0) {
                fscanf(input_fh, " %f,", &curr_object->angular_a0);
            }
            else if (strcmp(prop, "theta:") == 0) {
                fscanf(input_fh, " %f,", &curr_object->theta);
            }
            else if (strcmp(prop, "direction:") == 0) {
                fscanf(input_fh, " [%f, %f, %f],", &curr_object->direction[0], &curr_object->direction[1], &curr_object->direction[2]);
            }
            else {
                fseek(input_fh, -strlen(prop) - 1, SEEK_CUR);
                finished = true;
            }
        }
    }

    free(prop);
}

/*
* Copies all the attributes from one object into another object
*/
void copy_object(Object* dst_object, Object* src_object) {
    // Copy the object kind flag
    dst_object->object_kind_flag = src_object->object_kind_flag;

    // Copy the object's position
    dst_object->position[0] = src_object->position[0];
    dst_object->position[1] = src_object->position[1];
    dst_object->position[2] = src_object->position[2];

    // Copy any of the object's color values, depending on the type of object it is
    
    // Light color
    if (dst_object->object_kind_flag == LIGHT) {
        dst_object->color[0] = src_object->color[0];
        dst_object->color[1] = src_object->color[1];
        dst_object->color[2] = src_object->color[2];
    }
    
    // Object color
    else if (dst_object->object_kind_flag == SPHERE || dst_object->object_kind_flag == PLANE) {
        dst_object->diffuse_color[0] = src_object->diffuse_color[0];
        dst_object->diffuse_color[1] = src_object->diffuse_color[1];
        dst_object->diffuse_color[2] = src_object->diffuse_color[2];
        dst_object->specular_color[0] = src_object->specular_color[0];
        dst_object->specular_color[1] = src_object->specular_color[1];
        dst_object->specular_color[2] = src_object->specular_color[2];
    }

    // Copy the object's reflectivity
    dst_object->reflectivity = src_object->reflectivity;

    // Copy the camera's width and height
    if (dst_object->object_kind_flag == CAMERA) {
        dst_object->width = src_object->width;
        dst_object->height = src_object->height;
    }

    // Copy the plane's distance from origin and plane unit normal
    else if (dst_object->object_kind_flag == PLANE) {
        dst_object->distance = src_object->distance;
        dst_object->plane_normal[0] = src_object->plane_normal[0];
        dst_object->plane_normal[1] = src_object->plane_normal[1];
        dst_object->plane_normal[2] = src_object->plane_normal[2];
    }

    // Copy the sphere's radius
    else if (dst_object->object_kind_flag == SPHERE) {
        dst_object->radius = src_object->radius;
    }

    // Copy any light values
    else if (dst_object->object_kind_flag == LIGHT) {
        dst_object->radial_a0 = src_object->radial_a0;
        dst_object->radial_a1 = src_object->radial_a1;
        dst_object->radial_a2 = src_object->radial_a2;

        dst_object->theta = src_object->theta;
        dst_object->angular_a0 = src_object->angular_a0;
        dst_object->direction[0] = src_object->direction[0];
        dst_object->direction[1] = src_object->direction[1];
        dst_object->direction[2] = src_object->direction[2];
    }
}

/*
* Compares the two given objects to see if they are the same by
* Checking the object's kind and object's positon;
* Returns true of the objects are the same
*/
bool compare_objects(Object obj1, Object obj2) {
    return obj1.object_kind_flag == obj2.object_kind_flag
        && obj1.position[0] == obj2.position[0] 
        && obj1.position[1] == obj2.position[1] 
        && obj1.position[2] == obj2.position[2];
}

/* 
 * Increments an object's position value one at a time until all
 * Coordinates have moved the desired amount according to parameters;
 * Updates incrementally to support animation
 */
void increment_position(float *position, int x, int y) {
    // loop until movement complete
    while (x != 0 && y != 0) {
	// increment and update x
	if (x > 0) {
            position[0] += 1;
            x -= 1;
	}
	else if (x < 0) {
            position[0] -= 1;
            x += 1;
	}

	// increment and update y
	if (y > 0) {
            position[1] += 1;
            y -= 1;
	}
	else if (y < 0) {
            position[1] -= y;
            y += 1;
	}
    }
}

/*
* Shoots ray from origin to current pixel
* Returns name of object that was hit by ray
* And stores the position of the hit point in a float pointer
*/
float shoot(Object objects[], float* Ro, float* Rd, Object curr_object, Object* hit_object) {
    // Loop through objects
    float return_TValue = -1;
    float curr_TValue;
    
    // Loop through each object in the scene
    for (int i = 0; objects[i].object_kind_flag != 0; i++) {
        
        // Skip over camera object or the current object
        if (objects[i].object_kind_flag == CAMERA || compare_objects(objects[i], curr_object)) {
            continue;
        }
        
        // Check if valid t values found for sphere
        else if (objects[i].object_kind_flag == SPHERE) {
            // Ray-sphere intersection test
            curr_TValue = ray_sphere_intersection(objects[i], Ro, Rd);

            if (curr_TValue > 0) {
                if (return_TValue == -1 || curr_TValue < return_TValue) {
                    return_TValue = curr_TValue;
                    copy_object(hit_object, &objects[i]);
                }
            }
        }
        
        // Check if valid t values found for plane
        else if (objects[i].object_kind_flag == PLANE) {
            // Ray-plane intersection test
            curr_TValue = ray_plane_intersection(objects[i], Ro, Rd);

            if (curr_TValue > 0) {
                if (return_TValue == -1 || curr_TValue < return_TValue) {
                    return_TValue = curr_TValue;
                    copy_object(hit_object, &objects[i]);
                }
            }
        }
        
        // Assume gone through all objects
        else {
            break;
        }
    }

    return return_TValue;
}

/*
* Calculates the t value for a ray that hits a sphere
*/
float ray_sphere_intersection(Object sphere, float* Ro, float* Rd) {
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
float ray_plane_intersection(Object plane, float* Ro, float* Rd) {
    float numerator = ((plane.plane_normal[0] * Ro[0]) + (plane.plane_normal[1] * Ro[1]) + (plane.plane_normal[2] * Ro[2]));
    numerator += plane.distance;
    float denominator = (plane.plane_normal[0] * Rd[0]) + (plane.plane_normal[1] * Rd[1]) + (plane.plane_normal[2] * Rd[2]);
    float tValue = -(numerator / denominator);

    return tValue;
}

/*
* Calculates the color at the current position based on a hit object's diffuse and specular color,
* As well as its reflectivity if it has that property
*/
void illuminate(Object objects[], Object lights[], float *point, Object object, float *Rd, float *color, int recursion_level) {
  float current_color[3] = {0, 0, 0};
      color[0] = 0;
      color[1] = 0;
      color[2] = 0;
    
    // Check if at the first level of recursion; make color the background color (black) if so
    if (recursion_level > MAX_RECURSION_LEVEL) {
        return;
    }
    
    // Loop through all lights found
    for (int i = 0; lights[i].object_kind_flag != DEFAULT; i++) {
        // Shoot ray from point to current light's position;
        // If t value indicates point is in shadow, continue
        float light_Ro[3];
        light_Ro[0] = point[0];
        light_Ro[1] = point[1];
        light_Ro[2] = point[2];

        // Calculate Rd (distance from point to light)
        float light_Rd[3];
        float light_Rd_normal[3];
        v3_from_points(light_Rd, lights[i].position, point);
        v3_normalize(light_Rd_normal, light_Rd);

        // Shoot ray from point to light
        float tValue = -1;
        Object hit_object;
        tValue = shoot(objects, light_Ro, light_Rd_normal, object, &hit_object);

        if(tValue > 0 && tValue < v3_length(light_Rd)) {
            continue;
        }

        // Calculate radial attenuation
        float distance = v3_length(light_Rd);
        float radial_attenuation = 1 / (lights[i].radial_a0 + lights[i].radial_a1 * distance + lights[i].radial_a2 * distance * distance);

        // Calculate angular attenuation
        float v0[3];
        v0[0] = light_Rd_normal[0] * -1;
        v0[1] = light_Rd_normal[1] * -1;
        v0[2] = light_Rd_normal[2] * -1;

        // Check if the current light is a spot light; set angular attenuation to 1 if not
        float angular_attenuation;
        if (lights[i].theta == 0) {
            angular_attenuation = 1.0;
        }
        // Assume current light is a spot light
        else {            
            float angular_attenuation_dot = v3_dot_product(v0, lights[i].direction);
            float theta_radians = (lights[i].theta * PI) / 180;

            if (angular_attenuation_dot < cos(theta_radians)) {
                angular_attenuation = 0.0;
            }
            else {
                angular_attenuation = pow(angular_attenuation_dot, lights[i].angular_a0);
            }
        }

        // Calculate diffuse light
        float diffuse_light[3];
        float normal[3];
        if (object.object_kind_flag == SPHERE) {
            normal[0] = point[0] - object.position[0];
            normal[1] = point[1] - object.position[1];
            normal[2] = point[2] - object.position[2];
        }
        else if (object.object_kind_flag == PLANE) {
            normal[0] = object.plane_normal[0];
            normal[1] = object.plane_normal[1];
            normal[2] = object.plane_normal[2];
        } 
        else {
	        printf("Bad objecy type.\n");
	        exit(1);
	    }

        v3_normalize(normal, normal);
        float diffuse_light_dot = v3_dot_product(normal, light_Rd_normal);

        // Dot product of light vector and object's normal must be greater than 0 in order to contribute diffuse light
        // Otherwise it is 0
        if (diffuse_light_dot > 0) {
            diffuse_light[0] = diffuse_light_dot * lights[i].color[0] * object.diffuse_color[0];
            diffuse_light[1] = diffuse_light_dot * lights[i].color[1] * object.diffuse_color[1];
            diffuse_light[2] = diffuse_light_dot * lights[i].color[2] * object.diffuse_color[2];
        }
        else {
            diffuse_light[0] = 0.0;
            diffuse_light[1] = 0.0;
            diffuse_light[2] = 0.0;
        }

        // Calculate specular light
        float specular_light[3];
        float v[3] = {Rd[0], Rd[1], Rd[2]};
        float R[3];
        float v0_normal[3];
        v3_normalize(v0_normal, v0);
        v3_normalize(v, v);
        v3_reflect(R, light_Rd_normal, normal);
        v3_normalize(R, R);
        float specular_light_dot = v3_dot_product(v, R);
	//	if (object.object_kind_flag == SPHERE) printf("dot: %f %f\n", specular_light_dot, diffuse_light_dot);
        if (diffuse_light_dot > 0 && specular_light_dot > 0) {
            specular_light[0] = pow(specular_light_dot, 20) * lights[i].color[0] * object.specular_color[0];
            specular_light[1] = pow(specular_light_dot, 20) * lights[i].color[1] * object.specular_color[1];
            specular_light[2] = pow(specular_light_dot, 20) * lights[i].color[2] * object.specular_color[2];
	    
            if (specular_light[0] > .1) printf("spec: %f\n", specular_light[0]);
        }
        else {
            specular_light[0] = 0.0;
            specular_light[1] = 0.0;
            specular_light[2] = 0.0;
        }

        angular_attenuation = 1;
        current_color[0] += radial_attenuation * angular_attenuation * (diffuse_light[0] + specular_light[0]);
        current_color[1] += radial_attenuation * angular_attenuation * (diffuse_light[1] + specular_light[1]);
        current_color[2] += radial_attenuation * angular_attenuation * (diffuse_light[2] + specular_light[2]);
    }

    if (object.reflectivity > 0) {
        // Get reflected Rd
        float reflectedRd[3];

        // Calculate object's normal
        float normal[3];
        if (object.object_kind_flag == SPHERE) {
            normal[0] = point[0] - object.position[0];
            normal[1] = point[1] - object.position[1];
            normal[2] = point[2] - object.position[2];
        }
        else if (object.object_kind_flag == PLANE) {
            normal[0] = object.plane_normal[0];
            normal[1] = object.plane_normal[1];
            normal[2] = object.plane_normal[2];
        } 
        else {
	        printf("Bad object.\n");
	        exit(1);
	    }

        v3_normalize(normal, normal);
	    v3_normalize(Rd, Rd); // hack
        v3_reflect(reflectedRd, Rd, normal);
        v3_normalize(reflectedRd, reflectedRd);

        // Shoot "bounce" ray from point in direction of reflected Rd
        Object reflected_object;
        float tValue = shoot(objects, point, reflectedRd, object, &reflected_object);

        // If no object hit, exit function and keep pixel color as the background color
        // Assume ray hit an object
	    float reflectedColor[3] = {0, 0, 0};
        if (tValue > 0) {
            float reflectedPoint[3];
            reflectedPoint[0] = point[0] + (tValue * reflectedRd[0]);
            reflectedPoint[1] = point[1] + (tValue * reflectedRd[1]);
            reflectedPoint[2] = point[2] + (tValue * reflectedRd[2]);

            illuminate(objects, lights, reflectedPoint, reflected_object, reflectedRd, reflectedColor, recursion_level + 1);            
        }
	    
        current_color[0] = (1 - object.reflectivity) * current_color[0] + (object.reflectivity * reflectedColor[0]);
	    current_color[1] = (1 - object.reflectivity) * current_color[1] + (object.reflectivity * reflectedColor[1]);
	    current_color[2] = (1 - object.reflectivity) * current_color[2] + (object.reflectivity * reflectedColor[2]);
    }

    color[0] = current_color[0];
    color[1] = current_color[1];
    color[2] = current_color[2];
}

void adjust_color(float *color) {
    for (int i = 0; i < 3; i++) {
        if (color[i] > 1) {
            color[i] = 1;
        }
        else if (color[i] < 0) {
            color[i] = 0;
        }
    }
}

/*
* Writes the header and color values from the given image into 
* a ppm file with a P3 format
*/
bool write_p3(char *file_name, int width, int height, int max_color_value, uint8_t *image) {
    // Open file and check that it was open correctly
    FILE *fh = fopen(file_name, "w");
    if (fh == NULL) {
        fprintf(stderr, "Error: Output file not found");
        return false;
    }
    
    // Print the header of a P3 ppm file
    fprintf(fh, "P3 %d %d %d\n", width, height, max_color_value);

    // Write the color values found in the image
    for (int index = 0; index < width * height * 3; index += 3) {
        int red_channel = image[index];
        int green_channel = image[index + 1];
        int blue_channel = image[index + 2];
        fprintf(fh, "%d %d %d \n", red_channel, green_channel, blue_channel);
    }

    fclose(fh);
    return true;
}
// float ray_sphere_intersection(Object sphere, float* Ro, float* Rd);
int main(int argc, char **argv) {
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

    struct Object curr_object;
    char* obj_name = (char *)malloc(sizeof(char *));

    int object_array_index = 0;
    int light_array_index = 0;
    // Loop through the input file
    while (!feof(inputfh) && object_array_index < 128 && light_array_index < 128) {
        // Get the name of the object at beginning of current line
        fscanf(inputfh, "%s,", obj_name);
        // Check if the object is a camera
        if (strcmp(obj_name, "camera,") == 0) {
            // Set default values for camera
            curr_object.object_kind_flag = CAMERA;
            curr_object.position[0] = 0;
            curr_object.position[1] = 0;
            curr_object.position[2] = 0;
            curr_object.width = 0;
            curr_object.height = 0;

	    read_properties(inputfh, &curr_object);
        }
        // Check if the object is a sphere
        else if (strcmp(obj_name, "sphere,") == 0) {
            curr_object.object_kind_flag = SPHERE;

            // Set default values for sphere
            curr_object.color[0] = 0;
            curr_object.color[1] = 0;
            curr_object.color[2] = 0;
            curr_object.diffuse_color[0] = 0;
            curr_object.diffuse_color[1] = 0;
            curr_object.diffuse_color[2] = 0;
            curr_object.specular_color[0] = 0;
            curr_object.specular_color[1] = 0;
            curr_object.specular_color[2] = 0;
            curr_object.position[0] = 0;
            curr_object.position[1] = 0;
            curr_object.position[2] = 0;
            curr_object.radius = 0;

            read_properties(inputfh, &curr_object);
        }
        // Check if the object is a plane
        else if (strcmp(obj_name, "plane,") == 0) {
            curr_object.object_kind_flag = PLANE;

            // Set default values for plane
            curr_object.color[0] = 0;
            curr_object.color[1] = 0;
            curr_object.color[2] = 0;
            curr_object.diffuse_color[0] = 0;
            curr_object.diffuse_color[1] = 0;
            curr_object.diffuse_color[2] = 0;
            curr_object.specular_color[0] = 0;
            curr_object.specular_color[1] = 0;
            curr_object.specular_color[2] = 0;
            curr_object.position[0] = 0;
            curr_object.position[1] = 0;
            curr_object.position[2] = 0;
            curr_object.plane_normal[0] = 0;
            curr_object.plane_normal[1] = 0;
            curr_object.plane_normal[2] = 0;

	    read_properties(inputfh, &curr_object);

            curr_object.distance = -(v3_dot_product(curr_object.position, curr_object.plane_normal));
        }
        // Check if the object is a light
        else if (strcmp(obj_name, "light,") == 0) {
            curr_object.object_kind_flag = LIGHT;

	        // set default values for light
            curr_object.color[0] = 0;
            curr_object.color[1] = 0;
            curr_object.color[2] = 0;
            curr_object.position[0] = 0;
            curr_object.position[1] = 0;
            curr_object.position[2] = 0;
            curr_object.radial_a0 = 0;
            curr_object.radial_a1 = 0;
            curr_object.radial_a2 = 0;
            curr_object.theta = 0;
            curr_object.angular_a0 = 0;
            curr_object.direction[0] = 0;
            curr_object.direction[1] = 0;
            curr_object.direction[2] = 0;

        read_properties(inputfh, &curr_object);
        }
        // Assume it is an unknown object and throw error
        else {
            fprintf(stderr, "Error: unknown object found: %s\n", obj_name);
            return 1;
        }

        // Add current object to list of objects
        if (curr_object.object_kind_flag == LIGHT) {
            lights[light_array_index] = curr_object;
            light_array_index++;
        }
        else {
            objects[object_array_index] = curr_object;
            object_array_index++;
        }
    }
    
    if (object_array_index > 128) {
        fprintf(stderr, "Number of objects exceeds maximum value (128), remaining objects will be ignored.");
    }
    fclose(inputfh);
    free(obj_name);

    // Find the camera in the list of objects
    Object camera;
    bool camera_found = false;
    for (int i = 0; !camera_found; i++) {
        camera = objects[i];

        if (camera.object_kind_flag == CAMERA) {
            camera_found = true;
        }
    }

    // Get data about image size
    int image_height = atoi(argv[1]);
    int image_width = atoi(argv[2]);
    uint8_t image[image_width * image_height * 3];

    // Get data about viewscreen size from camera
    int viewscreen_width = camera.width;
    int viewscreen_height = camera.height;

    // Calculate pixel width and height based on viewscreen & image size
    float pix_height = (float)viewscreen_height / (float)image_height;
    float pix_width = (float)viewscreen_width / (float)image_width;

    int imageIndex = 0;
    // Iterate through each row in the image
    for (int y = 0; y < image_height; y++) {
        // Get the current pixel's y-coord
        float pixY = (-1 * viewscreen_height) / 2 + pix_height * (y + 0.5);

        // Iterate through each column of the image
        for (int x = 0; x < image_width; x++) {
            // Get the current pixel's x-coord
            float pixX = (-1 * viewscreen_width) / 2 + pix_width * (x + 0.5);

            // Get the current pixels's z-coord
            // (always 1 unit away from camera in the -z direction)
            float pixZ = -1;

            // Create vector out of pixel values
            float pix_vector[3] = {pixX, pixY, pixZ};

            // Normalize pixel vector
            float pix_vector_normal[3];
            v3_normalize(pix_vector_normal, pix_vector);

            // Shoot ray out into scene; if object hit, return object and its color
            Object hit_object;
            float tValue = -1;
            tValue = shoot(objects, camera.position, pix_vector_normal, camera, &hit_object);

            // Get illuminate value from illuminate function
            float hit_object_color[3];
            if(tValue > -1) {
                float point[3];
                int recursion_level = 1;
                point[0] = camera.position[0] + (pix_vector_normal[0] * tValue);
                point[1] = camera.position[1] + (pix_vector_normal[1] * tValue);
                point[2] = camera.position[2] + (pix_vector_normal[2] * tValue);
                illuminate(objects, lights, point, hit_object, pix_vector_normal, hit_object_color, recursion_level);
                adjust_color(hit_object_color);
            }
            else {
            	hit_object_color[0] = 0.0;
                hit_object_color[1] = 0.0;
                hit_object_color[2] = 0.0;
            }

            // Color current pixel in image  
            image[((image_height - y - 1) * image_width + x) * 3] = hit_object_color[0] * 255;
            image[(((image_height - y - 1) * image_width + x) * 3) + 1] = hit_object_color[1] * 255;
            image[(((image_height - y - 1) * image_width + x) * 3) + 2] = hit_object_color[2] * 255;

            imageIndex += 3;
        }
    }

    // Write color values in image to ppm file with p3 format
    write_p3(argv[4], image_width, image_height, 255, image);
    
    return 0;
}
