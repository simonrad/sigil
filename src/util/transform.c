#include "util/transform.h"

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
