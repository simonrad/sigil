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

Mat4 identity();
Mat4 ortho(float left, float right, float bottom, float top);

Mat4 translate(Mat4 *mat, float x, float y);
Mat4 rotate(Mat4 *mat, float degrees);
Mat4 scale(Mat4 *mat, float x, float y);
