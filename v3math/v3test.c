#include "v3math.h"


int main() {
	float v3_a[] = { 1.0, 2.0, 3.0 };
	float v3_b[] = { 1.0, 5.0, 6.0 };
	float v3_dst[] = { 0.0, 0.0, 0.0 };
	float scalar = 3.0;

	// Vector from two points test
	v3_from_points(v3_dst, v3_a, v3_b);

	// Vector addition test
	v3_add(v3_dst, v3_a, v3_b);

	// Vector subtraction test
	v3_subtract(v3_dst, v3_a, v3_b);

	// Dot product test
	float dot_product = v3_dot_product(v3_a, v3_b);

	// Cross product test
	v3_cross_product(v3_dst, v3_a, v3_b);

	// Scalar test
	v3_scale(v3_dst, scalar);

	// Angle between two vectors test
	float angle = v3_angle(v3_a, v3_b);

	// Angle between two vectors w/o/ cos-1 test
	angle = v3_angle_quick(v3_a, v3_b);
	
	// Vector reflection test
	v3_reflect(v3_dst, v3_a, v3_b);

	// Vector length test
	float vectorLength = v3_length(v3_a);

	// Vector normalization test
	v3_normalize(v3_dst, v3_a);
	
	return 0;
}