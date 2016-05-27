/*
libdrawtext - a simple library for fast text rendering in OpenGL
Copyright (C) 2011-2012  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef NO_OPENGL
#include <stdarg.h>
#include <math.h>
#include <ctype.h>
#include <float.h>

#include <stdlib.h>
#include <stdio.h>

#ifndef _MSC_VER
//#include <alloca.h>
#include <malloc.h>
#else
#include <malloc.h>
#endif

#include "config.h"

#ifdef USE_GLES
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

#include "drawtext.h"
#include "drawtext_impl.h"

struct vertex {
	float x, y;
	float s, t;
};

struct quad {
	struct vertex v[6];
};

static void cleanup(void);
static void add_glyph(struct glyph *g, float x, float y);

#define QBUF_SZ		2048
//512
static struct quad *qbuf;
static int num_quads;
static unsigned int font_tex;
static int buf_mode = DTX_NBF;
static struct dtx_box prepared_box;

#ifndef USE_GLES
	static GLuint vao;
#endif
static GLuint vbo;

int dtx_gl_init(void)
{
	if(qbuf) {
		return 0;	/* already initialized */
	}

	if(!(qbuf = (struct quad*)malloc(QBUF_SZ * sizeof *qbuf))) {
		return -1;
	}

	// currently there are no characters to render
	num_quads = 0;

	// generate our default vertex array object and the one we'll use for rendering
	#ifndef USE_GLES
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	#endif

	// set up our buffer object and allocate enough space for the entire character buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct vertex) * QBUF_SZ * 6, NULL, GL_DYNAMIC_DRAW);

	// vertex position attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), 0);

	// vertex tex coord attributes
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (GLvoid*)(sizeof(float) * 2));

	atexit(cleanup);
	return 0;
}

static void cleanup(void)
{
	free(qbuf);
}

void dtx_update_texture_interpolation()
{
	if(dtx_interpolation)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		// for heaven's sake don't let OpenGL interpolate what should be crisp text
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

static void set_glyphmap_texture(struct dtx_glyphmap *gmap)
{
	if(!gmap->tex)
	{
		// generate and bind our texture
		glGenTextures(1, &gmap->tex);
		glBindTexture(GL_TEXTURE_2D, gmap->tex);

		// specify any mipmapping levels
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

		// don't tile the texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// fill our texture data
		#ifdef USE_GLES
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, gmap->xsz, gmap->ysz, 0, GL_ALPHA, GL_UNSIGNED_BYTE, gmap->pixels);
		#else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, gmap->xsz, gmap->ysz, 0, GL_RED, GL_UNSIGNED_BYTE, gmap->pixels);
		#endif
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	if(font_tex != gmap->tex)
	{
		dtx_flush();
	}

	font_tex = gmap->tex;
}

void dtx_glyph(int code)
{
	struct dtx_glyphmap *gmap;

	if(!dtx_font || !(gmap = dtx_get_font_glyphmap(dtx_font, dtx_font_sz, code))) {
		return;
	}
	set_glyphmap_texture(gmap);

	add_glyph(gmap->glyphs + code - gmap->cstart, 0, 0);
	dtx_flush();
}

static const char *put_char(struct dtx_glyphmap *gmap, const char *str, float *pos_x, float *pos_y, int *should_flush)
{
	float px, py;
	int index;
	int code = *str;//dtx_utf8_char_code(str);
	str++;// = dtx_utf8_next_char((char*)str);

	if(buf_mode == DTX_LBF && code == '\n') {
		*should_flush = 1;
	}

	px = *pos_x;
	py = *pos_y;

	// process special characters
	if(code == '\n')
	{
		*pos_y -= gmap->line_advance;
		*pos_x = 0.0;
	}
	else if(code == '\t') *pos_x = (fmod(*pos_x, 4.0) + 4.0) * gmap->glyphs[0].advance;
	else if(code == '\r') *pos_x = 0.0;
	else
	{
		// advance the cursor normally
		*pos_x += gmap->glyphs[code - gmap->cstart].advance;

		// add the character
		index = code - gmap->cstart;
		set_glyphmap_texture(gmap);
		add_glyph(gmap->glyphs + index, px, py);
	}

	return str;
}

void dtx_string(const char *str)
{
	int should_flush = buf_mode == DTX_NBF;
	struct dtx_glyphmap *gmap;
	float pos_x = 0.0;
	float pos_y = 0.0;

	if(dtx_font)
	{
		// get the glyphmap *first*, then render each character with it. Christ...
		gmap = dtx_get_font_glyphmap(dtx_font, dtx_font_sz, *str);

		while(*str)
		{
			str = put_char(gmap, str, &pos_x, &pos_y, &should_flush);
		}
	}

	if(should_flush)
	{
		dtx_flush();
	}
}

void dtx_printf(const char *fmt, ...)
{
	va_list ap;
	int buf_size;
	char *buf, tmp;

	if(!dtx_font) {
		return;
	}

	va_start(ap, fmt);
	buf_size = vsnprintf(&tmp, 0, fmt, ap);
	va_end(ap);

	if(buf_size == -1) {
		buf_size = 512;
	}

	buf = (char*)alloca(buf_size + 1);
	va_start(ap, fmt);
	vsnprintf(buf, buf_size + 1, fmt, ap);
	va_end(ap);

	dtx_string(buf);
}

static void prepare_char(struct dtx_glyphmap *gmap, const char *str, float *pos_x, float *pos_y)
{
	float px, py;
	int index;
	int code = *str;

	px = *pos_x;
	py = *pos_y;

	// process special characters
	if(code == '\n')
	{
		*pos_y -= gmap->line_advance;
		*pos_x = 0.0;
	}
	else if(code == '\t') *pos_x = (fmod(*pos_x, 4.0) + 4.0) * gmap->glyphs[0].advance;
	else if(code == '\r') *pos_x = 0.0;
	else
	{
		// advance the cursor normally
		*pos_x += gmap->glyphs[code - gmap->cstart].advance;

		// add the character
		index = code - gmap->cstart;
		set_glyphmap_texture(gmap);
		add_glyph(gmap->glyphs + index, px, py);
	}
}

void dtx_prepare_string(const char *str)
{
	struct dtx_glyphmap *gmap;
	struct glyph *g = 0;
	float pos_x = 0.0f;
	float pos_y = 0.0f;

	float x0, y0, x1, y1;

	x0 = y0 = FLT_MAX;
	x1 = y1 = -FLT_MAX;

	// clear whatever we were prepared to render previously
	num_quads = 0;

	// make sure we have a valid font object
	if(dtx_font)
	{
		// get the glyphmap *first*, then render each character with it. Christ...
		gmap = dtx_get_font_glyphmap(dtx_font, dtx_font_sz, *str);

		while(*str)
		{
			// retrieve the glyph that we're going to prepare
			g = gmap->glyphs + (int)(*str) - gmap->cstart;

			// prepare the character by adding it to the vertex list and the text box
			prepare_char(gmap, str, &pos_x, &pos_y);
			str++;

			// adjust the size of the rendering box
			if(pos_x + g->orig_x < x0) x0 = pos_x + g->orig_x;
			if(pos_y - g->orig_y < y0) y0 = pos_y - g->orig_y;
			if(pos_x + g->orig_x + g->width > x1) x1 = pos_x + g->orig_x + g->width;
			if(pos_y - g->orig_y + g->height > y1) y1 = pos_y - g->orig_y + g->height;
		}
	}

	// prepare the rendering box dimensions so they can retrieved later
	prepared_box.x = x0;
	prepared_box.y = y0;
	prepared_box.width = x1 - x0;
	prepared_box.height = y1 - y0;

	// don't render or flush; wait for render call
}

float dtx_get_prepared_width()
{
	return prepared_box.width;
}

float dtx_get_prepared_height()
{
	return prepared_box.height;
}

void dtx_render_prepared()
{
	dtx_render();
}

static void qvertex(struct vertex *v, float x, float y, float s, float t)
{
	v->x = x;
	v->y = y;
	v->s = s;
	v->t = t;
}

static void add_glyph(struct glyph *g, float x, float y)
{
	struct quad *qptr = qbuf + num_quads;

	x -= g->orig_x;
	y -= g->orig_y;

	qvertex(qptr->v + 0, x, y, g->nx, g->ny + g->nheight);
	qvertex(qptr->v + 1, x + g->width, y, g->nx + g->nwidth, g->ny + g->nheight);
	qvertex(qptr->v + 2, x + g->width, y + g->height, g->nx + g->nwidth, g->ny);

	qvertex(qptr->v + 3, x, y, g->nx, g->ny + g->nheight);
	qvertex(qptr->v + 4, x + g->width, y + g->height, g->nx + g->nwidth, g->ny);
	qvertex(qptr->v + 5, x, y + g->height, g->nx, g->ny);

	if(++num_quads >= QBUF_SZ) {
		dtx_flush();
	}
}

void dtx_render()
{
	// don't do anything unless we have more text to render
	if(num_quads)
	{
		// enable texturing and set the texture we want to use that contains our text portions
		glBindTexture(GL_TEXTURE_2D, font_tex);
		dtx_update_texture_interpolation();

		// activate our vertex array object to bring in attribute states
		#ifndef USE_GLES
			glBindVertexArray(vao);
		#endif

		// fill the buffer with our new quad data
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct vertex) * num_quads * 6, &qbuf[0]);
		#ifdef USE_GLES
        		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), 0);
        		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (GLvoid*)(sizeof(float) * 2));
		#endif

		// render the text itself
		glDrawArrays(GL_TRIANGLES, 0, num_quads * 6);
	}
}

void dtx_flush(void)
{
	dtx_render();
	num_quads = 0;
}

#endif	/* !def NO_OPENGL */
