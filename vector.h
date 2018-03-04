#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

/*
 * Vector
 *
 * Struct for a 3D vector
 * Members:
 *  -x, y, z: the Cartesian coordinates of the vector
 */
typedef struct {
	double x;
	double y;
	double z;
} Vector;

/*
 * magnitude
 *
 * INPUTS: v: a vector to compute the magnitude of
 * RETURN VALUE: the magnitude of the vector
 * SIDE EFFECTS: none
 */ 
extern double magnitude(Vector v);

/*
 * normalize
 *
 * INPUTS: v: a vector to normalize
 * RETURN VALUE: a normalized vector
 * SIDE EFFECTS: none
 */ 
extern Vector normalize(Vector v);

/*
 * add_vec
 *
 * INPUTS: v1, v2: the two vectors to compute the sum of
 * RETURN VALUE: A vector representing the result of the vector sum v1 + v2
 * SIDE EFFECTS: none
 */ 
extern Vector add_vec(Vector v1, Vector v2);

/*
 * mul_vec
 *
 * INPUTS: s: a scalar to multiply the vector by
 *         v: the vector to multiply the scalar by
 * RETURN VALUE: the vector representing the product of the scalar s and the vector v
 * SIDE EFFECTS: none
 */ 
extern Vector mul_vec(double s, Vector v);

/*
 * neg_vec
 *
 * INPUTS: v: the vector to flip
 * RETURN VALUE: the vector with the same magnitude pointing in the opposite direction
 * SIDE EFFECTS: none
 */ 
extern Vector neg_vec(Vector v);
/*
 * cross
 *
 * INPUTS: u, v: the vectors to compute the cross product of
 * RETURN VALUE: the cross product of u and v
 * SIDE EFFECTS: none
 */ 
extern Vector cross(Vector u, Vector v);

/*
 * dot
 *
 * INPUTS: v1, v2: the vectors to compute the dot product of
 * RETURN VALUE: the dot product of the vectors
 * SIDE EFFECTS: none
 */ 
extern double dot(Vector v1, Vector v2);

#endif