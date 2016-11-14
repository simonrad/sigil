#include "text.h"
#include "shaders.h"

#include "../util/shader.h"

#include "../libdrawtext/drawtext.h"

#include <stdio.h>
#include <string.h>

#define DEFAULT_FONT_SLOTS 8

static struct dtx_font *sliCurrentFont;
static int sliCurrentSize;

static struct dtx_font **sliFontList;
static int sliNumFontSlots;
static int sliNumFonts;

static int sliAddFont(struct dtx_font *font);

void sliTextInit()
{
	sliCurrentFont = NULL;
	sliCurrentSize = 0;

	// create our expanding array of fonts we've loaded
	sliFontList = (struct dtx_font**)malloc(sizeof(struct dtx_font*) * DEFAULT_FONT_SLOTS);
	sliNumFontSlots = DEFAULT_FONT_SLOTS;
	sliNumFonts = 0;

	// initialize OpenGL memory in libdrawtext
	dtx_gl_init();
}

void sliTextDestroy()
{
	int i;

	// delete all of the fonts we've laoded
	for(i = 0; i < sliNumFonts; i ++)
	{
		dtx_close_font(sliFontList[i]);
	}

	// clear our list
	free(sliFontList);

	// free OpenGL memory
	dtx_gl_kill();
}

double sliTextWidth(const char *text)
{
	double result = 0.0;
	if(text[0] != 0)
		result = dtx_string_width(text);
	return result;
}

double sliTextHeight(const char *text)
{
	double result = 0.0;
	if(text[0] != 0)
		result = dtx_string_height(text);
	return result;
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

int sliLoadFont(const char *fontFilename)
{
	int result = -1;
	struct dtx_font *font = dtx_open_font(fontFilename, 0);

	if(font)
	{
		result = sliAddFont(font);
	}
	else
	{
		fprintf(stderr, "sliLoadFont() could not open font file %s", fontFilename);
		exit(1);
	}

	return result;
}

void sliFont(int font, int fontSize)
{
	if(font >= 0 && font < sliNumFonts)
	{
		sliCurrentFont = sliFontList[font];
		sliCurrentSize = fontSize;

		dtx_prepare(sliCurrentFont, sliCurrentSize);
		dtx_use_font(sliCurrentFont, sliCurrentSize);
		dtx_use_interpolation(1);
	}
	else
	{
		fprintf(stderr, "sliFont(): font index %d is out of range", font);
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

static int sliAddFont(struct dtx_font *font)
{
	struct dtx_font **newFontList;

	// allocate more space for our font list if needed
	if(sliNumFonts >= sliNumFontSlots)
	{
		newFontList = (struct dtx_font**)malloc(sizeof(struct dtx_font*) * sliNumFontSlots * 2);
		memcpy(newFontList, sliFontList, sizeof(struct dtx_font*) * sliNumFontSlots);
		sliNumFontSlots *= 2;
		sliFontList = newFontList;
	}

	// now insert our new font and return its index in our list
	sliFontList[sliNumFonts] = font;
	return sliNumFonts++;
}
