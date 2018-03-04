#include "vector.h"

/*
 * magnitude
 *
 * INPUTS: v: a vector to compute the magnitude of
 * RETURN VALUE: the magnitude of the vector
 * SIDE EFFECTS: none
 */ 
double magnitude(Vector v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/*
 * normalize
 *
 * INPUTS: v: a vector to normalize
 * RETURN VALUE: a normalized vector
 * SIDE EFFECTS: none
 */ 
Vector normalize(Vector v) {
	double len = magnitude(v);
	v.x /= len;
	v.y /= len;
	v.z /= len;
	return v;
}

/*
 * add_vec
 *
 * INPUTS: v1, v2: the two vectors to compute the sum of
 * RETURN VALUE: A vector representing the result of the vector sum v1 + v2
 * SIDE EFFECTS: none
 */ 
Vector add_vec(Vector v1, Vector v2) {
	return (Vector){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

/*
 * mul_vec
 *
 * INPUTS: s: a scalar to multiply the vector by
 *         v: the vector to multiply the scalar by
 * RETURN VALUE: the vector representing the product of the scalar s and the vector v
 * SIDE EFFECTS: none
 */ 
Vector mul_vec(double s, Vector v) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

/*
 * neg_vec
 *
 * INPUTS: v: the vector to flip
 * RETURN VALUE: the vector with the same magnitude pointing in the opposite direction
 * SIDE EFFECTS: none
 */ 
Vector neg_vec(Vector v) {
	return (Vector){-v.x, -v.y, -v.z};
}

/*
 * cross
 *
 * INPUTS: u, v: the vectors to compute the cross product of
 * RETURN VALUE: the cross product of u and v
 * SIDE EFFECTS: none
 */ 
Vector cross(Vector u, Vector v) {
	Vector r;
	r.x = u.y * v.z - u.z * v.y;
	r.y = u.z * v.x - u.x * v.z;
	r.z = u.x * v.y - u.y * v.x;
	return r;
}

/*
 * dot
 *
 * INPUTS: v1, v2: the vectors to compute the dot product of
 * RETURN VALUE: the dot product of the vectors
 * SIDE EFFECTS: none
 */ 
double dot(Vector v1, Vector v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
