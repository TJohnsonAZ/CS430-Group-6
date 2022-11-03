// preprocessor directive
#ifndef RAYCAST_H
#define RAYCAST_H

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

// structures
typedef struct Object {
	int objectKindFlag;
	float position[3];

	// Camera/shapes
	union {
	    float color[3];
	    float diffuse_color[3];
	    float specular_color[3];
            // Camera values
	    struct {
		float width;
		float height;
	    };
	    // Plane values
	    struct {
	        float pn[3]; // plane unit normal (A, B, C)
	        float d; // distance from plane to (0, 0, 0)
	    };
	    // Sphere value
	    struct {
	        float radius;
	    };
	};
	
	// Light values
	union {
		float radial_a0;
		float radial_a1;
		float radial_a2;
		// Spot light values
		struct {
			float theta;
			float angular_a0;
			float direction[3];
		};
	};
} Object;

// function prototypes
void readProperties(FILE *inputfh, Object *curr_object);
float* illuminate(float* Rd, float* point, Object lights[], Object object, float* color);
float* shoot(Object objects[], float* pixVectorNormal, Object camera, float* hitObjectColor);
// void shade(uint8_t* image, int imageIndex, float* hitObjectColor);
float raysphereIntersection(Object sphere, float* Ro, float* Rd);
float rayplaneIntersection(Object plane, float* Ro, float* Rd);
bool write_p3(char* fileName, int width, int height, int maxcol, uint8_t* image);


#endif  // CONFIG_OPS_H
