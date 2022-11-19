// preprocessor directive
#ifndef RAYTRACE_H
#define RAYTRACE_H

// header files
#include <stdio.h> //fopen, fscanf, ect
#include <stdlib.h> // malloc, ect
#include <stdint.h> // standard integer types
#include <ctype.h> // isspace, ect
#include <string.h>
#include <stdbool.h>

// constants
#define DEFAULT 0
#define CAMERA 1
#define SPHERE 2
#define PLANE 3
#define LIGHT 4
#define PI 3.141592653
#define MAX_RECURSION_LEVEL 7

// structures
typedef struct Object {
        int object_kind_flag;
        float position[3];
        float color[3];
        float diffuse_color[3];
        float specular_color[3];
        float reflectivity;
        float radial_a0;
        float radial_a1;
        float radial_a2;
  
        union {
            // Camera values
            struct {
            float width;
            float height;
            };
            // Plane values
            struct {
                float d; // distance from plane to (0, 0, 0)
                float pn[3]; // plane unit normal (A, B, C)
            };
            // Sphere value
            struct {
                float radius;
            };
        };

        // Light values
        union {
            // Spot light values
            struct {
                float theta;
                float angular_a0;
                float direction[3];
            };
        };
} Object;

// function prototypes
void read_properties(FILE *inputfh, Object *curr_object);
void illuminate(Object objects[], Object lights[], float *point, Object object, float *Rd, float *color, int recursion_level);
void adjust_color(float *color);
void copy_object(Object *dstObject, Object *srcObject);
float shoot(Object objects[], float *Ro, float *Rd, Object curr_object, Object *hit_object);
bool compare_object_position(Object obj1, Object obj2);
float ray_sphere_intersection(Object sphere, float *Ro, float *Rd);
float ray_plane_intersection(Object plane, float *Ro, float *Rd);
bool write_p3(char* fileName, int width, int height, int maxcol, uint8_t *image);


#endif
