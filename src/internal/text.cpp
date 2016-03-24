#include "internal/text.h"
#include "internal/shaders.h"

#include "assets/shader.h"

#include "libdrawtext/drawtext.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <cstdio>

static struct dtx_font *sliCurrentFont = NULL;
static int sliCurrentSize = 0;

void sliTextInit()
{
	sliFont("ttf/arial.ttf", 12);
}

void sliTextDestroy()
{

}

void sliTextAlign(int fontAlign)
{

}

double sliTextWidth(const char *text)
{
	return dtx_string_width(text);
}

double sliTextHeight(const char *text)
{
	return dtx_string_height(text);
}

void sliText(mat4 &modelview, vec4 &color, const char *text)
{
	// prepare our shader object in case the dtx text buffer is ready for rendering
	sliTextShader -> bind();
	sliTextShader -> uniformMatrix4fv("u_Modelview", 1, value_ptr(modelview));
	sliTextShader -> uniformVec4("u_Color", color);

	// queue the incoming text; this may result in a render if the buffer becomes full (which is why we activate our shader)
	dtx_string(text);
}

void sliTextFlush(mat4 &modelview, vec4 &color)
{
	// prepare our shader object
	sliTextShader -> bind();
	sliTextShader -> uniformMatrix4fv("u_Modelview", 1, value_ptr(modelview));
	sliTextShader -> uniformVec4("u_Color", color);

	// render the buffered text
	dtx_flush();
}

void sliFont(int fontID)
{

}

void sliFont(const char *fontFilename, int fontSize)
{
	sliCurrentFont = dtx_open_font(fontFilename, fontSize);

	if(sliCurrentFont != NULL)
	{
		sliCurrentSize = fontSize;
		dtx_use_font(sliCurrentFont, sliCurrentSize);
		dtx_use_interpolation(true);
	}
	else
	{
		fprintf(stderr, "sliFont() could not open font file %s", fontFilename);
		exit(1);
	}
}
/*
void sliFontSize(int fontSize)
{
	if(fontSize != sliCurrentSize)
	{
		sliCurrentSize = fontSize;
		if(sliCurrentFont != NULL)
		{
			dtx_prepare(sliCurrentFont, fontSize);
			dtx_use_font(sliCurrentFont, sliCurrentSize);
			dtx_use_interpolation(true);
		}
	}
}
*/
