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

	union {
		float color[3];

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
} Object;

// function prototypes
Object shoot(float *normalVector, float* hitPoint, Object camera, Object objects[]);
void shade(uint8_t* image, int imageIndex, Object hitObject, float* hitPoint);
float raysphereIntersection(float* hitPoint, Object sphere, float* normalVector, float* originPoint);
float rayplaneIntersection(Object plane, Object camera, float *normalVector);
bool write_p3(char* fileName, int width, int height, int maxcol, uint8_t* image);


#endif  // CONFIG_OPS_H