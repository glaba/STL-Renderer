/*								
 *
 *
 * Author:	    Kapil Kanwar
 * Version:	    1
 * Creation Date:   27 February 2018
 * Filename:	    renderer.c
 * History:
 *	CZ	1	12 February 2018
 *		First written.
 */
#include "renderer.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ABS(X) (((X) > 0) ? (X) : (-(X)))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#define BUFFER_SIZE 2500000
#define STL_BLOCK_SIZE 50

#define AMBIENT_PORTION 0

/*
 * Triangle
 *
 * Struct representing a triangle that will be rendered
 * Members:
 *  -vertices: the indices of the 3 vertices of the triangle in the global array vertex_list
 *  -color: the color of the triangle
 */
typedef struct {
	int32_t vertices[3];
	int32_t color;
} Triangle;

/*
 * RawTriangle
 *
 * Struct representing a triangle that will not be rendered (must be passed through add_triangle)
 * Members:
 *  -vertices: the actual positions of the 3 vertices of the triangles as vectors
 *  -color: the color of the triangle
 */
typedef struct {
	Vector vertices[3];
	int32_t color;
} RawTriangle;

// A list of the vertices in the current 3D scene
int32_t vertex_list_size = 0;
Vector* vertex_list;

// A list of the triangles in the current 3D scene
int32_t triangles_size = 0;
Triangle* triangles;

/*
 * get_normal
 *
 * INPUTS: t: the triangle to compute the normal of
 * RETURN VALUE: a vector pointing in the direction perpendicular to the triangle face
 * SIDE EFFECTS: none
 */ 
Vector get_normal(Triangle t) {
	Vector u = add_vec(vertex_list[t.vertices[1]], neg_vec(vertex_list[t.vertices[0]]));
	Vector v = add_vec(vertex_list[t.vertices[2]], neg_vec(vertex_list[t.vertices[0]]));
	Vector normal;
	normal.x = u.y * v.z - u.z * v.y;
	normal.y = u.z * v.x - u.x * v.z;
	normal.z = u.x * v.y - u.y * v.x;
	return normalize(normal);
}

/*
 * get_color
 *
 * INPUTS: t: the triangle to get the color of, given its orientation and the lighting conditions
 *         light_direction: the normalized vector pointing in the direction of the ambient light source
 * RETURN VALUE: the color of the triangle under the given lighting conditions
 * SIDE EFFECTS: none
 */ 
int32_t get_color(Triangle t, Vector light_direction) {
	double cos = ABS(dot(get_normal(t), normalize(light_direction)));
	int32_t r = (t.color >> 16) & 0x000000FF;
	int32_t g = (t.color >> 8)  & 0x000000FF;
	int32_t b = (t.color >> 0)  & 0x000000FF; 

	r = (int32_t)(r * AMBIENT_PORTION + ((r / 255.0) * r * cos) * (1 - AMBIENT_PORTION));
	g = (int32_t)(g * AMBIENT_PORTION + ((g / 255.0) * g * cos) * (1 - AMBIENT_PORTION));
	b = (int32_t)(b * AMBIENT_PORTION + ((b / 255.0) * b * cos) * (1 - AMBIENT_PORTION));

	return ((r << 16) | (g << 8) | (b << 0));
}

/*
 * project_point
 *
 * INPUTS: camera_location: the focal point of the camera
 *         camera_direction: the normalized direction of the camera
 *         camera_right: the normalized vector (in 3D) pointing in the (-1, 0) direction in the camera plane
 *         camera_up: the normalizd vector (in 3D) pointing in the (1, 0) direction in the camera plane
 *         origin: the point corresponding to the origin on the camera plane
 *         point: the point to project onto the camera plane
 * RETURNS: A RawTriangle in camera plane coordinates (x, y) if the projection is possible, otherwise it returns a RawTriangle with a negative color
 */
Vector project_point(Vector camera_location, Vector camera_direction, Vector camera_right, Vector camera_up, Vector origin, Vector point) {
	Vector proj;
	proj.z = 0;

	double t = dot(camera_direction, add_vec(origin, neg_vec(camera_location))) / 
	           dot(camera_direction, add_vec(point, neg_vec(camera_location)));
	// Return an impossible projection if t < 0 (non-zero z, where z should be 0)
	if (t < 0) {
		return (Vector){0, 0, -1};
	}

	Vector intersection = (Vector){camera_location.x + (point.x - camera_location.x) * t,
	                               camera_location.y + (point.y - camera_location.y) * t,
	                               camera_location.z + (point.z - camera_location.z) * t};
	intersection = add_vec(intersection, neg_vec(origin));
	proj.x = dot(camera_right, intersection);
	proj.y = dot(camera_up, intersection);
	return proj;
}

/*
 * dynamic_resize
 *
 * INPUTS: array: the array to resize
 *         array_len: a pointer to a number containing the current length of the array
 *         element_size: the size of each element in the array (returned by sizeof)
 * RETURNS: a new array with the old elements but a larger size
 * SIDE EFFECTS: alters the value pointed to by array_len with the new array length
 *               frees the array passed as a parameter
 */
void* dynamic_resize(void* array, int32_t* array_len, uint32_t element_size) {
	char* old_list = (char*)array;
	int32_t old_array_len = *array_len;
	// Double the size of the list
	*array_len = (*array_len == 0) ? (1) : ((*array_len) * 2);
	// Allocate new memory for the new list
	char* new_list = malloc((*array_len) * element_size);
	// Copy the old information from the old array into the new array
	int32_t i;
	for (i = 0; i < old_array_len * element_size; i++) {
		new_list[i] = old_list[i];
	}
	// Free the old list
	free(old_list);
	// Return new list
	return new_list;
}

/*
 * add_triangle
 *
 * INPUTS: t: the triangle object containing the raw coordinates to insert into the scene
 *         num_triangles, num_vertices: pointers to these two counters that will be updated as needed
 */
void add_triangle(RawTriangle t, int32_t* num_triangles, int32_t* num_vertices) {
	int32_t vertexIndices[3];
	int32_t i;
	for (i = 0; i < 3; i++) {
		if (*num_vertices == vertex_list_size) {
			vertex_list = dynamic_resize(vertex_list, &vertex_list_size, (uint32_t)sizeof(Vector));
		}

		// Insert new vertex into vertex list
		vertex_list[*num_vertices] = t.vertices[i];
		vertexIndices[i] = *num_vertices;
		(*num_vertices)++;
	}

	// Create new triangle object
	Triangle new_t;
	new_t.color = t.color;
	for (i = 0; i < 3; i++)
		new_t.vertices[i] = vertexIndices[i];

	if (*num_triangles == triangles_size) {
		triangles = dynamic_resize(triangles, &triangles_size, (uint32_t)sizeof(Triangle));
	}

	// Insert new triangle into triangle array
	triangles[*num_triangles] = new_t;
	(*num_triangles)++;
}

/*
 * parse_and_insert_STL
 *
 * INPUTS: file: the STL file path
 *         max_radius: the maximum distance from the center that each of the vertices in the object should have
 *         num_triangles, num_vertices: pointers to these counters that will be updated as needed when the object is inserted
 *         color: the color of the object
 * SIDE EFFECTS: adds the triangles from the STL file into the scene, and assumes that this object is the only one in the scene
 *               may crash if the file provided is invalid
 */
void parse_and_insert_STL(char* file, double max_radius, int32_t* num_triangles, int32_t* num_vertices, int32_t color) {
	FILE* fp;

	fp = fopen(file, "r");
	
	// Skip STL header
	fseek(fp, 80, SEEK_SET);

	// Get number of triangles
	uint32_t actual_num_triangles;
	fread(&actual_num_triangles, 4, 1, fp);

	int32_t i;
	char buffer[BUFFER_SIZE];
	while (1) {
		int32_t num_elements_read = fread(buffer, 1, BUFFER_SIZE, fp);
		for (i = 0; i < num_elements_read / STL_BLOCK_SIZE; i++) {
			char* cur_buffer = buffer + i * STL_BLOCK_SIZE;
			RawTriangle t;
			t.color = color;	

			// Skip normal
			cur_buffer += 12;

			// Read in triangles
			int32_t j;
			for (j = 0; j < 3; j++) {
				float x, y, z;
				memcpy(&x, cur_buffer + 0, 4);
				memcpy(&y, cur_buffer + 4, 4);
				memcpy(&z, cur_buffer + 8, 4);
				t.vertices[j].x = (double)x;
				t.vertices[j].y = (double)y;
				t.vertices[j].z = (double)z;
				cur_buffer += 12;
			}

			// Skip attributes
			cur_buffer += 2;

			// Add triangle
			add_triangle(t, num_triangles, num_vertices);
		}

		if (num_elements_read != BUFFER_SIZE)
			break;
	}

	fclose(fp);

	// Normalize triangles to be centered at the origin
	Vector center = (Vector){0, 0, 0};
	for (i = 0; i < *num_vertices; i++) {
		center = add_vec(center, vertex_list[i]);
	}
	center = mul_vec(1.0 / (*num_vertices), center);

	for (i = 0; i < *num_vertices; i++) {
		vertex_list[i] = add_vec(vertex_list[i], neg_vec(center));
	}

	// Limit the maximum spread
	double actual_max_radius = 0;
	for (i = 0; i < *num_vertices; i++) {
		double dist = magnitude(vertex_list[i]);
		actual_max_radius = MAX(actual_max_radius, dist);
	}

	for (i = 0; i < *num_vertices; i++) {
		vertex_list[i] = mul_vec(max_radius / actual_max_radius, vertex_list[i]);
	}
}	

/*
 * draw_picture
 *
 * Implements a basic 3D renderer that draws the scene described in the function initialize_scene
 * INPUTS: file: the path to the STL file to render
 *         scale: the maximum radius of any of the object's vertices
 *         camera_location: the location where the camera should be placed
 *         rotation: the amount that the camera should be rotated clockwise from its default orientation
 *         color: the color of the object to draw
 * RETURNS: 1 if any dot is drawn out of bounds
 */
int32_t draw_picture(char* file, double scale, Vector camera_location, double rotation, int32_t color) {
	// Initialize variables
	int32_t output = 1;
	int32_t num_triangles = 0;
	int32_t num_vertices = 0;
	triangles = NULL;
	vertex_list = NULL;

	// Clear image
	int32_t x;
	for (x = 0; x < WIDTH; x++) {
		int32_t y;
		for (y = 0; y < HEIGHT; y++) {
			draw_dot(x, y);
		}
	}

	// Insert object into scene
	int delta_vertices = num_vertices;
	parse_and_insert_STL(file, scale, &num_triangles, &num_vertices, color);
	delta_vertices = num_vertices - delta_vertices;

	// Set camera direction to point towards the origin (where the object is)
	Vector camera_direction = normalize(neg_vec(camera_location));

	// Set camera right to be the vector in the xy plane that is perpendicular to camera_direction
	double right_angle = -atan2(camera_direction.x, camera_direction.y) - rotation;
	Vector camera_right = normalize((Vector){cos(right_angle), sin(right_angle), 0});

	Vector camera_up = normalize(cross(camera_right, camera_direction));

	const double CAMERA_SCALE = 1 / 500.0;
	const double CAMERA_WIDTH = WIDTH * CAMERA_SCALE;
	const double CAMERA_HEIGHT = HEIGHT * CAMERA_SCALE;
	const Vector LIGHT_DIRECTION = camera_direction;

	double z_buffer[WIDTH][HEIGHT];
	for (x = 0; x < WIDTH; x++) {
		int32_t y;
		for (y = 0; y < HEIGHT; y++) {
			z_buffer[x][y] = 100000000.0;
		}
	}

	// Create z buffer
	// Loop through all objects
	// 		Calculate color of object given its normal
	//		Calculate the positions of the vertices in the picture
	//		Loop through the pixels in the triangle and check each one with the z buffer
	// 		If the z buffer is good, put the pixel into the image
	int32_t i;
	for (i = 0; i < num_triangles; i++) {
		int32_t color = get_color(triangles[i], LIGHT_DIRECTION);

		Vector projectedVertices[3];
		int32_t j;
		for (j = 0; j < 3; j++) {
			projectedVertices[j] = project_point(camera_location, camera_direction, camera_right, camera_up, 
			                                      add_vec(camera_location, camera_direction), vertex_list[triangles[i].vertices[j]]);
		}

		Vector proj_start = projectedVertices[0];
		Vector proj_trace = add_vec(projectedVertices[2], neg_vec(projectedVertices[1]));
		Vector actual_start = vertex_list[triangles[i].vertices[0]];
		Vector actual_trace = add_vec(vertex_list[triangles[i].vertices[2]], neg_vec(vertex_list[triangles[i].vertices[1]]));

		// Draw a triangle by going across one edge and drawing lines to the remaining point
		double progress;
		double increment1 = CAMERA_SCALE / magnitude(proj_trace) / 2;
		for (progress = 0; progress <= 1; progress += increment1) {
			Vector proj_end = add_vec(projectedVertices[1], mul_vec(progress, proj_trace));
			Vector actual_end = add_vec(vertex_list[triangles[i].vertices[1]], mul_vec(progress, actual_trace));

			Vector proj_delta = add_vec(proj_end, neg_vec(proj_start));
			Vector actual_delta = add_vec(actual_end, neg_vec(actual_start));

			// Draw a line from start to end
			double t;
			double increment2 = CAMERA_SCALE / magnitude(proj_delta) / 2;
			for (t = 0; t <= 1; t += increment2) {
				Vector proj_point = add_vec(proj_start, mul_vec(t, proj_delta));
				Vector actual_point = add_vec(actual_start, mul_vec(t, actual_delta));

				int32_t camera_x = (proj_point.x + CAMERA_WIDTH / 2) / CAMERA_SCALE;
				int32_t camera_y = (-proj_point.y + CAMERA_HEIGHT / 2) / CAMERA_SCALE;

				if (camera_x >= 0 && camera_x < WIDTH && camera_y >= 0 && camera_y < HEIGHT) {
					// Check distance and z-buffer
					double dist = magnitude(add_vec(actual_point, neg_vec(camera_location)));
					if (dist < z_buffer[camera_x][camera_y]) {
						set_color(color);
						output &= draw_dot(camera_x, camera_y);
						z_buffer[camera_x][camera_y] = dist;
					}
				}
			}
		}
	}

	return output;
}
