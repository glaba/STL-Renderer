/*									
 *
 * renderer.h - header file for ECE220 picture drawing program
 *
 * "Copyright (c) 2018 by Charles H. Zega, and Saransh Sinha."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Charles Zega, Saransh Sinha
 * Version:	    2
 * Creation Date:   12 February 2018
 * Filename:	    renderer.h
 * History:
 *	CZ	1	12 February 2018
 *		First written.
 *	SL	2	13 February 2018
 *		Split into separate C files for compilation.
 */

#if !defined(RENDERER_H)
#define RENDERER_H

#include <stdint.h>

#define WIDTH 624
#define HEIGHT 320

/*
 * Draws the 3D rendered STL file
 */
extern int32_t draw_picture();

/* 
 *  set_color
 *	 
 * Created by the ECE 220H course staff
 *	
 *	
 * INPUTS: new_color represents the color for future calls to draw_dot where:
 * bits 31-24 are ignored
 * bits 23-16 are red
 * bits 15-8 are green
 * bits 7-0 are blue
 * OUTPUTS: none
 * RETURN VALUE: none
 * SIDE EFFECTS: sets color to be new_color
 *
 *
 * Changes color of pixels drawn using draw_dot. The format of the input
 * is the most significant byte being ignored, the second most significant
 * byte representing red, the third green, and the least significant byte
 * being blue.
 */
extern void set_color(int32_t new_color);

/* 
 * draw_dot
 *	 
 * Created by the ECE 220H course staff
 *	
 *	
 * INPUTS: x,y -- the coordinates of a new pixel to be drawn
 * OUTPUTS: sets the element in the picture_data array that represents the coordinates
 * (x,y) to be a pixel_t that represents the color given by the variable, color
 * RETURN VALUE: 1 if the pixel is in bounds, 0 if it is out of bounds
 * SIDE EFFECTS: none
 *
 *
 * Draws a pixel at the location (x,y) of the color set by the most recent
 * call to set color. Returns 1 if the pixel is in bounds and 0 if the pixel
 * is out of bounds. Does not draw anything if it is out of bounds.
 */
extern int32_t draw_dot(int32_t x, int32_t y);


#endif // RENDERER_H
