#include "transform.h"

#ifdef _MSC_VER
	#define _USE_MATH_DEFINES
#endif
#include <math.h>

static Mat4 matrixMultiply(Mat4 *a, Mat4 *b);

Mat4 identity(void)
{
	Mat4 result;

	result.cols[0].x = 1.0f; result.cols[0].y = 0.0f; result.cols[0].z = 0.0f; result.cols[0].w = 0.0f;
	result.cols[1].x = 0.0f; result.cols[1].y = 1.0f; result.cols[1].z = 0.0f; result.cols[1].w = 0.0f;
	result.cols[2].x = 0.0f; result.cols[2].y = 0.0f; result.cols[2].z = 1.0f; result.cols[2].w = 0.0f;
	result.cols[3].x = 0.0f; result.cols[3].y = 0.0f; result.cols[3].z = 0.0f; result.cols[3].w = 1.0f;

	return result;
}

Mat4 ortho(double left, double right, double bottom, double top)
{
	const double NEAR = 0.0;
	const double FAR = 1.0;

	Mat4 result = identity();
	result.cols[0].x = (float)(2.0 / (right - left));
	result.cols[1].y = (float)(2.0 / (top - bottom));
	result.cols[2].z = (float)(-2.0 / (FAR - NEAR));
	result.cols[3].x = (float)(-(right + left) / (right - left));
	result.cols[3].y = (float)(-(top + bottom) / (top - bottom));
	result.cols[3].z = (float)(-(FAR + NEAR) / (FAR - NEAR));

	return result;
}

Mat4 translate(Mat4 *mat, double x, double y)
{
	Mat4 t = identity();
	t.cols[3].x = (float)x;
	t.cols[3].y = (float)y;

	return matrixMultiply(&t, mat);
}

Mat4 rotate(Mat4 *mat, double degrees)
{
	Mat4 r = identity();
	float radians = (float)(M_PI * degrees) / 180.0f;
	float s = sinf(radians);
	float c = cosf(radians);
	r.cols[0].x = c;
	r.cols[0].y = s;
	r.cols[1].x = -s;
	r.cols[1].y = c;

	return matrixMultiply(&r, mat);
}

Mat4 scale(Mat4 *mat, double x, double y)
{
	Mat4 s = identity();
	s.cols[0].x = (float)x;
	s.cols[1].y = (float)y;
	s.cols[2].z = 1.0f;

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
