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
                float distance; // distance from plane to (0, 0, 0)
                float plane_normal[3]; // plane unit normal (A, B, C)
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
void read_properties(FILE *input_fh, Object *curr_object);
void copy_object(Object* dstObject, Object* srcObject);
bool compare_objects(Object obj1, Object obj2);
float shoot(Object objects[], float* Ro, float* Rd, Object curr_object, Object* hit_object);
float ray_sphere_intersection(Object sphere, float* Ro, float* Rd);
float ray_plane_intersection(Object plane, float* Ro, float* Rd);
void illuminate(Object objects[], Object lights[], float *point, Object object, float *Rd, float *color, int recursion_level);
void adjust_color(float *color);
bool write_p3(char* file_name, int width, int height, int max_color_value, uint8_t *image);
void draw_frame(int viewscreen_width, int viewscreen_height, float pix_width, float pix_height, uint8_t *image, int image_width, int image_height,
    Object objects[], Object lights[], Object camera, char* fh);

#endif
