#import v3math.h
#import <math.h>


void v3_from_points(float *dst, float *a, float *b) {

    

}

void v3_add(float *dst, float *a, float *b) {



}

void v3_subtract(float *dst, float *a, float *b) {



}

float v3_dot_product(float *a, float *b) {

`   return 0;

}

void v3_cross_product(float *dst, float *a, float *b) {



}

void v3_scale(float *dst, float s) {

    dst[0] = dst[0] * s;
    dst[1] = dst[1] * s;
    dst[2] = dst[2] * s;

}

float v3_angle(float *a, float *b) {

    float lengthA = v3_length(*a);
    float lengthB = v3_length(*b);

    float numerator = (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
    float denominator = lengthA * lengthB;
    
    float angle = numerator / denominator;

    float cosAngle = acos(angle);

    return cosAngle

}

float v3_angle_quick(float *a, float *b) {

    float lengthA = v3_length(*a);
    float lengthB = v3_length(*b);

    float numerator = (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
    float denominator = lengthA * lengthB;

    float angle = numerator / denominator;

    return angle;

}

void v3_reflect(float *dst, float *v, float *n) {



}

float v3_length(float *a) {

    float length = sqrt(a[0]^2 + a[1]^2 + a[2]^2);

    return length;
}

void v3_normalize(float *dst, float *a) {



}