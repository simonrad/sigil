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
	dtx_vertex_attribs(0, 1);

	sliFontSize(12);
	sliFont("ttf/arial.ttf");
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
	sliTextureShader -> bind();
	sliTextureShader -> uniformMatrix4fv("u_Modelview", 1, value_ptr(modelview));
	sliTextureShader -> uniformVec4("u_Color", color);
	sliTextureShader -> uniformVec2("u_Tiling", vec2(1.0));
	sliTextureShader -> uniformVec2("u_Scroll", vec2(0.0));

	// queue the incoming text; this may result in a render if the buffer becomes full (which is why we activate our shader)
	dtx_string(text);
}

void sliTextFlush(mat4 &modelview, vec4 &color)
{
	// prepare our shader object
	sliTextureShader -> bind();
	sliTextureShader -> uniformMatrix4fv("u_Modelview", 1, value_ptr(modelview));
	sliTextureShader -> uniformVec4("u_Color", color);
	sliTextureShader -> uniformVec2("u_Tiling", vec2(1.0));
	sliTextureShader -> uniformVec2("u_Scroll", vec2(0.0));

	// render the buffered text
	dtx_flush();
}

void sliFont(int fontID)
{

}

void sliFont(const char *fontFilename)
{
	sliCurrentFont = dtx_open_font(fontFilename, sliCurrentSize);
	if(sliCurrentFont == NULL)
	{
		fprintf(stderr, "sliFont() could not open font file %s", fontFilename);
		exit(1);
	}
}

void sliFontSize(int fontSize)
{
	if(fontSize != sliCurrentSize)
	{
		sliCurrentSize = fontSize;
		if(sliCurrentFont != NULL)
		{
			dtx_prepare(sliCurrentFont, fontSize);
		}
	}
}
