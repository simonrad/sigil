#pragma once

typedef struct VEC2
{
	float x;
	float y;
} Vec2;

typedef struct VEC3
{
	float x;
	float y;
	float z;
} Vec3;

typedef struct VEC4
{
	float x;
	float y;
	float z;
	float w;
} Vec4;

typedef struct MAT4
{
	Vec4 cols[4];
} Mat4;

Mat4 identity(void);
Mat4 ortho(double left, double right, double bottom, double top);

Mat4 translate(Mat4 *mat, double x, double y);
Mat4 rotate(Mat4 *mat, double degrees);
Mat4 scale(Mat4 *mat, double x, double y);
