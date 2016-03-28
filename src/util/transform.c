#include "util/transform.h"

static Mat4 matrixMultiply(Mat4 *a, Mat4 *b);

Mat4 identity()
{
    Mat4 result = {.cols[0] = {.x = 1.0, .y = 0.0, .z = 0.0, .w = 1.0},
				   .cols[1] = {.x = 0.0, .y = 1.0, .z = 0.0, .w = 0.0},
				   .cols[2] = {.x = 0.0, .y = 0.0, .z = 1.0, .w = 0.0},
				   .cols[3] = {.x = 0.0, .y = 0.0, .z = 0.0, .w = 1.0}};
	return result;
}

Mat4 ortho(float left, float right, float bottom, float top)
{

}

Mat4 translate(Mat4 mat, float x, float y)
{

}

Mat4 rotate(Mat4 mat, float degrees)
{

}

Mat4 scale(Mat4 mat, float x, float y)
{

}

static Mat4 matrixMultiply(Mat4 a, Mat4 b)
{
	Mat4 c;
    c.cols[0][0] = a -> cols[0][0] * b -> cols[0][0] + a -> cols[0][1] * b -> cols[1][0] + a -> cols[0][2] * b -> cols[2][0] + a -> cols[0][3] * b -> cols[3][0];
    c.cols[0][1] = a -> cols[0][0] * b -> cols[0][1] + a -> cols[0][1] * b -> cols[1][1] + a -> cols[0][2] * b -> cols[2][1] + a -> cols[0][3] * b -> cols[3][1];
    c.cols[0][2] = a -> cols[0][0] * b -> cols[0][2] + a -> cols[0][1] * b -> cols[1][2] + a -> cols[0][2] * b -> cols[2][2] + a -> cols[0][3] * b -> cols[3][2];
    c.cols[0][3] = a -> cols[0][0] * b -> cols[0][3] + a -> cols[0][1] * b -> cols[1][3] + a -> cols[0][2] * b -> cols[2][3] + a -> cols[0][3] * b -> cols[3][3];
    c.cols[1][0] = a -> cols[1][0] * b -> cols[0][0] + a -> cols[1][1] * b -> cols[1][0] + a -> cols[1][2] * b -> cols[2][0] + a -> cols[1][3] * b -> cols[3][0];
    c.cols[1][1] = a -> cols[1][0] * b -> cols[0][1] + a -> cols[1][1] * b -> cols[1][1] + a -> cols[1][2] * b -> cols[2][1] + a -> cols[1][3] * b -> cols[3][1];
    c.cols[1][2] = a -> cols[1][0] * b -> cols[0][2] + a -> cols[1][1] * b -> cols[1][2] + a -> cols[1][2] * b -> cols[2][2] + a -> cols[1][3] * b -> cols[3][2];
    c.cols[1][3] = a -> cols[1][0] * b -> cols[0][3] + a -> cols[1][1] * b -> cols[1][3] + a -> cols[1][2] * b -> cols[2][3] + a -> cols[1][3] * b -> cols[3][3];
    c.cols[2][0] = a -> cols[2][0] * b -> cols[0][0] + a -> cols[2][1] * b -> cols[1][0] + a -> cols[2][2] * b -> cols[2][0] + a -> cols[2][3] * b -> cols[3][0];
    c.cols[2][1] = a -> cols[2][0] * b -> cols[0][1] + a -> cols[2][1] * b -> cols[1][1] + a -> cols[2][2] * b -> cols[2][1] + a -> cols[2][3] * b -> cols[3][1];
    c.cols[2][2] = a -> cols[2][0] * b -> cols[0][2] + a -> cols[2][1] * b -> cols[1][2] + a -> cols[2][2] * b -> cols[2][2] + a -> cols[2][3] * b -> cols[3][2];
    c.cols[2][3] = a -> cols[2][0] * b -> cols[0][3] + a -> cols[2][1] * b -> cols[1][3] + a -> cols[2][2] * b -> cols[2][3] + a -> cols[2][3] * b -> cols[3][3];
    c.cols[3][0] = a -> cols[3][0] * b -> cols[0][0] + a -> cols[3][1] * b -> cols[1][0] + a -> cols[3][2] * b -> cols[2][0] + a -> cols[3][3] * b -> cols[3][0];
    c.cols[3][1] = a -> cols[3][0] * b -> cols[0][1] + a -> cols[3][1] * b -> cols[1][1] + a -> cols[3][2] * b -> cols[2][1] + a -> cols[3][3] * b -> cols[3][1];
    c.cols[3][2] = a -> cols[3][0] * b -> cols[0][2] + a -> cols[3][1] * b -> cols[1][2] + a -> cols[3][2] * b -> cols[2][2] + a -> cols[3][3] * b -> cols[3][2];
    c.cols[3][3] = a -> cols[3][0] * b -> cols[0][3] + a -> cols[3][1] * b -> cols[1][3] + a -> cols[3][2] * b -> cols[2][3] + a -> cols[3][3] * b -> cols[3][3];
    return C;
}
