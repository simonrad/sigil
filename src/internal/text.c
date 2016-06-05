#include "text.h"
#include "shaders.h"

#include "../util/shader.h"

#include "../libdrawtext/drawtext.h"

#include <stdio.h>

static struct dtx_font *sliCurrentFont = NULL;
static int sliCurrentSize = 0;

void sliTextInit()
{

}

void sliTextDestroy()
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

void sliText(Mat4 *modelview, Vec4 *color, const char *text)
{
	// prepare our shader object in case the dtx text buffer is ready for rendering
	shaderBind(sliTextShader);
	shaderUniformMatrix4fv(sliTextShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliTextShader, "u_Color", (float)color -> x, (float)color -> y, (float)color -> z, (float)color -> w);

	// queue the incoming text; this may result in a render if the buffer becomes full (which is why we activate our shader)
	dtx_string(text);
}

void sliTextFlush(Mat4 *modelview, Vec4 *color)
{
	// prepare our shader object
	shaderBind(sliTextShader);
	shaderUniformMatrix4fv(sliTextShader, "u_Modelview", 1, (float*)modelview);
	shaderUniform4f(sliTextShader, "u_Color", (float)color -> x, (float)color -> y, (float)color -> z, (float)color -> w);

	// render the buffered text
	dtx_flush();
}

void sliFont(const char *fontFilename, int fontSize)
{
	sliCurrentFont = dtx_open_font(fontFilename, fontSize);

	if(sliCurrentFont != NULL)
	{
		sliCurrentSize = fontSize;
		dtx_use_font(sliCurrentFont, sliCurrentSize);
		dtx_use_interpolation(1);
	}
	else
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
			dtx_use_font(sliCurrentFont, sliCurrentSize);
			dtx_use_interpolation(1);
		}
	}
}
