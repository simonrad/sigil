#include "transform.h"

#include <math.h>

static Mat4 matrixMultiply(Mat4 *a, Mat4 *b);

Mat4 identity()
{
    Mat4 result = {.cols[0] = {.x = 1.0, .y = 0.0, .z = 0.0, .w = 0.0},
				   .cols[1] = {.x = 0.0, .y = 1.0, .z = 0.0, .w = 0.0},
				   .cols[2] = {.x = 0.0, .y = 0.0, .z = 1.0, .w = 0.0},
				   .cols[3] = {.x = 0.0, .y = 0.0, .z = 0.0, .w = 1.0}};
	return result;
}

Mat4 ortho(float left, float right, float bottom, float top)
{
	const float NEAR = 0.0;
	const float FAR = 1.0;

	Mat4 result = identity();
	result.cols[0].x = 2.0 / (right - left);
	result.cols[1].y = 2.0 / (top - bottom);
	result.cols[2].z = -2.0 / (FAR - NEAR);
	result.cols[3].x = -(right + left) / (right - left);
	result.cols[3].y = -(top + bottom) / (top - bottom);
	result.cols[3].z = -(FAR + NEAR) / (FAR - NEAR);

	return result;
}

Mat4 translate(Mat4 *mat, float x, float y)
{
	Mat4 t = identity();
	t.cols[3].x = x;
	t.cols[3].y = y;

	return matrixMultiply(&t, mat);
}

Mat4 rotate(Mat4 *mat, float degrees)
{
	Mat4 r = identity();
	float radians = (M_PI * degrees) / 180.0;
	float s = sinf(radians);
	float c = cosf(radians);
	r.cols[0].x = c;
	r.cols[0].y = s;
	r.cols[1].x = -s;
	r.cols[1].y = c;

	return matrixMultiply(&r, mat);
}

Mat4 scale(Mat4 *mat, float x, float y)
{
	Mat4 s = identity();
	s.cols[0].x = x;
	s.cols[1].y = y;
	s.cols[2].z = 1.0;

	return matrixMultiply(&s, mat);
}

static Mat4 matrixMultiply(Mat4 *a, Mat4 *b)
{
	Mat4 c;
    c.cols[0].x = a -> cols[0].x * b -> cols[0].x + a -> cols[0].y * b -> cols[1].x + a -> cols[0].z * b -> cols[2].x + a -> cols[0].w * b -> cols[3].x;
    c.cols[0].y = a -> cols[0].x * b -> cols[0].y + a -> cols[0].y * b -> cols[1].y + a -> cols[0].z * b -> cols[2].y + a -> cols[0].w * b -> cols[3].y;
    c.cols[0].z = a -> cols[0].x * b -> cols[0].z + a -> cols[0].y * b -> cols[1].z + a -> cols[0].z * b -> cols[2].z + a -> cols[0].w * b -> cols[3].z;
    c.cols[0].w = a -> cols[0].x * b -> cols[0].w + a -> cols[0].y * b -> cols[1].w + a -> cols[0].z * b -> cols[2].w + a -> cols[0].w * b -> cols[3].w;
    c.cols[1].x = a -> cols[1].x * b -> cols[0].x + a -> cols[1].y * b -> cols[1].x + a -> cols[1].z * b -> cols[2].x + a -> cols[1].w * b -> cols[3].x;
    c.cols[1].y = a -> cols[1].x * b -> cols[0].y + a -> cols[1].y * b -> cols[1].y + a -> cols[1].z * b -> cols[2].y + a -> cols[1].w * b -> cols[3].y;
    c.cols[1].z = a -> cols[1].x * b -> cols[0].z + a -> cols[1].y * b -> cols[1].z + a -> cols[1].z * b -> cols[2].z + a -> cols[1].w * b -> cols[3].z;
    c.cols[1].w = a -> cols[1].x * b -> cols[0].w + a -> cols[1].y * b -> cols[1].w + a -> cols[1].z * b -> cols[2].w + a -> cols[1].w * b -> cols[3].w;
    c.cols[2].x = a -> cols[2].x * b -> cols[0].x + a -> cols[2].y * b -> cols[1].x + a -> cols[2].z * b -> cols[2].x + a -> cols[2].w * b -> cols[3].x;
    c.cols[2].y = a -> cols[2].x * b -> cols[0].y + a -> cols[2].y * b -> cols[1].y + a -> cols[2].z * b -> cols[2].y + a -> cols[2].w * b -> cols[3].y;
    c.cols[2].z = a -> cols[2].x * b -> cols[0].z + a -> cols[2].y * b -> cols[1].z + a -> cols[2].z * b -> cols[2].z + a -> cols[2].w * b -> cols[3].z;
    c.cols[2].w = a -> cols[2].x * b -> cols[0].w + a -> cols[2].y * b -> cols[1].w + a -> cols[2].z * b -> cols[2].w + a -> cols[2].w * b -> cols[3].w;
    c.cols[3].x = a -> cols[3].x * b -> cols[0].x + a -> cols[3].y * b -> cols[1].x + a -> cols[3].z * b -> cols[2].x + a -> cols[3].w * b -> cols[3].x;
    c.cols[3].y = a -> cols[3].x * b -> cols[0].y + a -> cols[3].y * b -> cols[1].y + a -> cols[3].z * b -> cols[2].y + a -> cols[3].w * b -> cols[3].y;
    c.cols[3].z = a -> cols[3].x * b -> cols[0].z + a -> cols[3].y * b -> cols[1].z + a -> cols[3].z * b -> cols[2].z + a -> cols[3].w * b -> cols[3].z;
    c.cols[3].w = a -> cols[3].x * b -> cols[0].w + a -> cols[3].y * b -> cols[1].w + a -> cols[3].z * b -> cols[2].w + a -> cols[3].w * b -> cols[3].w;
    return c;
}
