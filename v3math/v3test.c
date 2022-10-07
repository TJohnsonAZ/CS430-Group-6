#include "v3math.h"


int main() {
	float dstVector[] = { 0.0, 0.0, 0.0 };
	float firstVector[] = { 1.0, 2.0, 3.0 };
	float secondVector[] = { 1.0, 5.0, 6.0 };
	float decimalVector[] = { 1.5, 3.4, 8.75 };
	float negativeVector[] = { -2.0, -10.0, -1.5 };
	float posAndNegVector[] = { 4.0, -1.0, 7.0 };
	float ultimateVector[] = { -2.75, -15.5, 1.0 };
    float vector2D1[] = { 5, 10 };
    float vector2D2[] = { 4.7, 7.2 };
    float vector2D3[] = { 6, 3.4 };
    float nVector[] = { 2, 8 };
	float scalar;

	// VECTOR FROM TWO POINTS TEST //////////////////
	printf("\nVECTOR FROM TWO POINTS TESTS\n");
	printf("----------------------------\n");

	// Test 1
	v3_from_points(dstVector, firstVector, secondVector);
	float testVector = {0, -3, -3};
	if (v3_equals(dstVector, testVector, 0)) {
		printf("Test 1 passed\n");
	}
	else {
		printf("Test 1 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 2
	v3_from_points(dstVector, firstVector, secondVector);
	float testVector = {0, 3, 3};
	if (v3_equals(dstVector, testVector, 0)) {
		printf("Test 2 passed\n");
	}
	else {
		printf("Test 2 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 3
	v3_from_points(dstVector, firstVector, negativeVector);
	float testVector = {3, 12, 1.5};
	if (v3_equals(dstVector, testVector, 0)) {
		printf("Test 3 passed\n");
	}
	else {
		printf("Test 3 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// VECTOR ADDITION TEST /////////////////////////
	printf("VECTOR ADDITION TESTS\n");
	printf("---------------------\n");
	
	// Test 1
	float addTest1[] = { 2.0, 7.0, 9.0 };
	v3_add(dstVector, firstVector, secondVector);
	if (v3_equals(dstVector, addTest1, 0)) {
		printf("Test 1 passed\n");
	}
	else {
		printf("Test 1 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 2
	float addTest2[] = {-1.0, -5.0, 4.5};
	v3_add(dstVector, secondVector, negativeVector);
	if (v3_equals(dstVector, addTest2, 0)) {
		printf("Test 2 passed\n");
	}
	else {
		printf("Test 2 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 3
	float addTest3[] = {2.0, -11, 5.5};
	v3_add(dstVector, negativeVector, posAndNegVector);
	if (v3_equals(dstVector, addTest3, 0)) {
		printf("Test 3 passed\n");
	}
	else {
		printf("Test 3 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 4
	float addTest4[] = { 2.0, 7.0, 9.0 };
	v3_add(dstVector, secondVector, firstVector);
	if (v3_equals(dstVector, addTest4, 0)) {
		printf("Test 4 passed\n");
	}
	else {
		printf("Test 4 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// VECTOR SUBTRACTION TEST //////////////////////
	printf("\nVECTOR SUBTRACTION TESTS\n");
	printf("------------------------\n");
	
	// Test 1
	float subTest1[] = {0.0, -3.0, -3.0};
	v3_subtract(dstVector, subTest1, secondVector);
	if (v3_equals(dstVector, subTest1, 0)) {
		printf("Test 1 passed\n");
	}
	else {
		printf("Test 1 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 2
	float subTest2[] = {3.0, 15.0, 7.5};
	v3_subtract(dstVector, secondVector, negativeVector);
	if (v3_equals(dstVector, subTest2, 0)) {
		printf("Test 2 passed\n");
	}
	else {
		printf("Test 2 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 3
	float subTest3[] = { 4.25, 18.90, 7.75 };
	v3_subtract(dstVector, decimalVector, ultimateVector);
	if (v3_equals(dstVector, subTest3, 0)) {
		printf("Test 3 passed\n");
	}
	else {
		printf("Test 3 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	
	// DOT PRODUCT TEST /////////////////////////////
	printf("DOT PRODUCT TESTS\n");
	printf("-----------------\n");
	
	// Test 1
	float dotProductTest = 29.0;
	float dotProduct = v3_dot_product(firstVector, secondVector);
	if (float_equals(dotProductTest, dotProduct)) {
		printf("Test 1 passed");
	}
	else {
		printf("Test 1 failed");
	}

	// Test 2
	dotProductTest = -8.5;
	dotProduct = v3_dot_product(negativeVector, posAndNegVector);
	if (float_equals(dotProduct, dotProductTest, 0)) {
		printf("Test 2 passed\n");
	}
	else {
		printf("Test 2 failed\n");
	}


	// Test 3
	dotProductTest = -48.075;
	dotProduct = v3_dot_product(decimalVector, ultimateVector);
	if (float_equals(dotProduct, dotProductTest, tolerance)) {
		printf("Test 3 passed\n");
	}
	else {
		printf("Test 3 failed\n");
	}


	// Test 4
	dotProductTest = 29.0;
	dotProduct = v3_dot_product(secondVector, firstVector);
	if (float_equals(dotProduct, dotProductTest, 0)) {
		printf("Test 4 passed");
	}
	else {
		printf("Test 4 failed");
	}


	// Test 5
	dotProductTest = 248.8125;
	dotProduct = v3_dot_product(ultimateVector, ultimateVector);
	if (float_equals(dotProduct, dotProductTest, 2.0)) {
		printf("Test 5 passed");
	}
	else {
		printf("Test 5 failed");
	}


	// CROSS PRODUCT TEST ///////////////////////////
	printf("CROSS PRODUCT TESTS\n");
	printf("-------------------\n");
	
	// Test 1
    float crossTest1[] = { -3, -3, 3 };
	v3_cross_product(dstVector, firstVector, secondVector);
	if (v3_equals(dstVector, crossTest1, 0)) {
		printf("Test 1 passed");
	}
	else {
		printf("Test 1 failed");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 2
    float crossTest2[] = { 23.35, 0.25, -4.1 };
    v3_cross_product(dstVector, secondVector, decimalVector);
	if (v3_equals(dstVector, crossTest2, 0)) {
		printf("Test 2 passed\n");
	}
	else {
		printf("Test 2 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 3
    float crossTest3[] = { 107.5, -23.25, -64.75 };
    v3_cross_product(dstVector, posAndNegVector, ultimateVector);
	if (v3_equals(dstVector, crossTest3, 0)) {
		printf("Test 3 passed\n");
	}
	else {
		printf("Test 3 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// SCALAR TEST //////////////////////////////////
	printf("SCALAR TESTS\n");
	printf("------------\n");
	
	// Test 1
	scalar = 3.0;
	testVector[] = [3.0, 6.0, 9.0];
	v3_scale(dstVector, scalar);
	if (v3_equals(dstVector, testVector, 0)) {
		printf("Test 1 passed\n");
	}
	else {
		printf("Test 1 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 2
	scalar = -4.0;
	testVector = [-4.0, -20.0, -24.0];
	v3_scale(dstVector, scalar);
	if (v3_equals(dstVector, testVector, 0)) {
		printf("Test 2 passed\n");
	}
	else {
		printf("Test 2 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 3
	scalar = 2.5;
	testVector = [2.5, 12.5, 15.0];
	v3_scale(dstVector, scalar);
	if (v3_equals(dstVector, testVector, 0)) {
		printf("Test 3 passed\n");
	}
	else {
		printf("Test 3 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// ANGLE BETWEEN TWO VECTORS TEST ////////////////
	printf("ANGLE BETWEEN TWO VECTORS TESTS\n");
	printf("-------------------------------\n");
	
	// Test 1
	float angle = v3_angle(firstVector, secondVector);
	float testAngle = 0.18;
	if (v2_equals(dstVector, testVector)) {
		printf("Test 1 passed");
	}
	else {
		printf("Test 1 failed");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 2
	
	angle = v3_angle(decimalVector, secondVector);
	float testAngle = 0.32;
	if (angle == testAngle)) {
		printf("Test 2 passed\n");
	}
	else {
		printf("Test 2 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 3
	angle = v3_angle(firstVector, decimalVector);
	float testAngle = 0.24;
	if (angle == testAngle)) {
		printf("Test 3 passed\n");
	}
	else {
		printf("Test 3 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// ANGLE BETWEEN TWO VECTORS W/O/ COS-1 TEST////
	printf("ANGLE BETWEEN TWO VECTORS W/O/ COS-1 TESTS\n");
	printf("------------------------------------------\n");
	
	// Test 1
	angle = v3_angle_quick(firstVector, secondVector);
	float testAngle = 0.99;
	if (angle == testAngle)) {
		printf("Test 1 passed\n");
		printf("Test 1 passed");
	}
	else {
		printf("Test 1 failed");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 2
	angle = v3_angle_quick(firstVector, secondVector);
	float testAngle = 0.95;
	if (angle == testAngle)) {
		printf("Test 2 passed\n");
	}
	else {
		printf("Test 2 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 3
	angle = v3_angle_quick(firstVector, secondVector);
	float testAngle = 0.97;
	if (angle == testAngle)) {
		printf("Test 3 passed\n");
	}
	else {
		printf("Test 3 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}
	
	// VECTOR REFLECTION TEST ///////////////////////
	printf("VECTOR REFLECTION TESTS\n");
	printf("-----------------------\n");
	
	// Test 1
    float reflectTest1[] = { -355, -1430 };
	v3_reflect(dstVector, vector2D1, nVector);
	if (v3_equals(dstVector, reflectTest1)) {
		printf("Test 1 passed");
	}
	else {
		printf("Test 1 failed");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 2
    float reflectTest2[] = { -263.3, -1064.8 };
	v3_reflect(dstVector, vector2D2, nVector);
	if (v3_equals(dstVector, reflectTest2, 0)) {
		printf("Test 2 passed\n");
	}
	else {
		printf("Test 2 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 3
    float reflectTest3[] = { -150.8, -623.8 };
	v3_reflect(dstVector, vector2D3, nVector);
	if (v3_equals(dstVector, reflectTest3, 0)) {
		printf("Test 3 passed\n");
	}
	else {
		printf("Test 3 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// VECTOR LENGTH TEST //////////////////////////
	printf("VECTOR LENGTH TESTS\n");
	printf("-------------------\n");
	
	// Test 1
	float vectorLength = v3_length(firstVector);
	float testLength = 3.74;
	if (vectorLength == testLength)) {
		printf("Test 1 passed\n");
		printf("Test 1 passed");
	}
	else {
		printf("Test 1 failed");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 2
	float vectorLength = v3_length(secondVector);
	float testLength = 7.87;
	if (vectorLength == testLength)) {
		printf("Test 2 passed\n");
	}
	else {
		printf("Test 2 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 3
	float vectorLength = v3_length(decimalVector);
	float testLength = 9.51;
	if (vectorLength == testLength)) {
		printf("Test 3 passed\n");
	}
	else {
		printf("Test 3 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// VECTOR NORMALIZATION TEST ///////////////////
	printf("VECTOR NORMALIZATION TESTS\n");
	printf("--------------------------\n");
	
	// Test 1
	v3_normalize(dstVector, firstVector);
	float testVector[] = {0.28, 0.53, 0.8};
	if (v3_equals(dstVector, testVector)) {
		printf("Test 1 passed");
	}
	else {
		printf("Test 1 failed");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}

	// Test 2
	v3_normalize(dstVector, secondVector);
	float testVector[] = {0.13, 0.64, 0.76};
	if (v3_equals(dstVector, testVector, 0)) {
		printf("Test 2 passed\n");
	}
	else {
		printf("Test 2 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
		
	}

	// Test 3
	v3_normalize(dstVector, secondVector);
	float testVector[] = {0.16, 0.36, 0.92};
	if (v3_equals(dstVector, testVector, 0)) {
		printf("Test 3 passed\n");
	}
	else {
		printf("Test 3 failed\n");
		for (int index = 0; index > 3; index ++)
		{

			printf("%.2f ", dstVector[index]);

		}
	}
	
	return 0;
}