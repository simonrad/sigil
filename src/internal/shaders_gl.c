#include "shaders.h"

#include "../util/transform.h"
#include "../util/shader.h"

#include <stdlib.h>
#include <stdio.h>

static const char *sliBasicVertexShaderCode =
"#version 150 \n"
"uniform mat4 u_Projection;"
"uniform mat4 u_Modelview;"
"in vec2 a_Vertex;"
"void main()"
"{"
"gl_Position = u_Projection * u_Modelview * vec4(a_Vertex, 0.0, 1.0);"
"}";

static const char *sliBasicFragmentShaderCode =
"#version 150 \n"
"uniform vec4 u_Color;"
"out vec4 f_Color;"
"void main()"
"{"
"f_Color = u_Color;"
"}";

static const char *sliPointVertexShaderCode =
"#version 150 \n"
"uniform mat4 u_Projection;"
"in vec2 a_Vertex;"
"in vec4 a_Color;"
"out vec4 v_Color;"
"void main()"
"{"
"v_Color = a_Color;"
"gl_Position = u_Projection * vec4(a_Vertex, 0.0, 1.0);"
"}";

static const char *sliPointFragmentShaderCode =
"#version 150 \n"
"in vec4 v_Color;"
"out vec4 f_Color;"
"void main()"
"{"
"f_Color = v_Color;"
"}";

static const char *sliTextureVertexShaderCode =
"#version 150 \n"
"uniform mat4 u_Projection;"
"uniform mat4 u_Modelview;"
"in vec2 a_Vertex;"
"in vec2 a_TexCoord;"
"out vec2 v_TexCoord;"
"void main()"
"{"
"v_TexCoord = a_TexCoord;"
"gl_Position = u_Projection * u_Modelview * vec4(a_Vertex, 0.0, 1.0);"
"}";

static const char *sliTextureFragmentShaderCode =
"#version 150 \n"
"uniform sampler2D u_Texture;"
"uniform vec4 u_Color;"
"uniform vec2 u_Tiling;"
"uniform vec2 u_Scroll;"
"in vec2 v_TexCoord;"
"out vec4 f_Color;"
"void main()"
"{"
"f_Color = texture(u_Texture, (v_TexCoord * u_Tiling) + u_Scroll) * u_Color;"
"}";

static const char *sliTextVertexShaderCode =
"#version 150 \n"
"uniform mat4 u_Projection;"
"uniform mat4 u_Modelview;"
"in vec2 a_Vertex;"
"in vec2 a_TexCoord;"
"out vec2 v_TexCoord;"
"void main()"
"{"
"v_TexCoord = a_TexCoord;"
"gl_Position = u_Projection * u_Modelview * vec4(a_Vertex, 0.0, 1.0);"
"}";

static const char *sliTextFragmentShaderCode =
"#version 150 \n"
"uniform sampler2D u_Texture;"
"uniform vec4 u_Color;"
"in vec2 v_TexCoord;"
"out vec4 f_Color;"
"void main()"
"{"
"float luminance = texture(u_Texture, v_TexCoord).a;"
"f_Color = u_Color * vec4(luminance);"
"}";

Shader *sliBasicShader;
Shader *sliPointShader;
Shader *sliTextureShader;
Shader *sliTextShader;

void sliShadersInit(Mat4 *projection)
{
	sliBasicShader = shaderLoad(sliBasicVertexShaderCode, sliBasicFragmentShaderCode);
	shaderBindAttrib(sliBasicShader, "a_Vertex", 0);
	shaderLink(sliBasicShader);
	shaderBind(sliBasicShader);
	shaderUniformMatrix4fv(sliBasicShader, "u_Projection", 1, (float*)projection);
	shaderFinalize(sliBasicShader);

	sliPointShader = shaderLoad(sliPointVertexShaderCode, sliPointFragmentShaderCode);
	shaderBindAttrib(sliPointShader, "a_Vertex", 0);
	shaderBindAttrib(sliPointShader, "a_Color", 1);
	shaderLink(sliPointShader);
	shaderBind(sliPointShader);
	shaderUniformMatrix4fv(sliPointShader, "u_Projection", 1, (float*)projection);
	shaderFinalize(sliPointShader);

	sliTextureShader = shaderLoad(sliTextureVertexShaderCode, sliTextureFragmentShaderCode);
	shaderBindAttrib(sliTextureShader, "a_Vertex", 0);
	shaderBindAttrib(sliTextureShader, "a_TexCoord", 1);
	shaderLink(sliTextureShader);
	shaderBind(sliTextureShader);
	shaderUniformMatrix4fv(sliTextureShader, "u_Projection", 1, (float*)projection);
	shaderUniform1i(sliTextureShader, "u_Texture", 0);
	shaderFinalize(sliTextureShader);

	sliTextShader = shaderLoad(sliTextVertexShaderCode, sliTextFragmentShaderCode);
	shaderBindAttrib(sliTextShader, "a_Vertex", 0);
	shaderBindAttrib(sliTextShader, "a_TexCoord", 1);
	shaderLink(sliTextShader);
	shaderBind(sliTextShader);
	shaderUniformMatrix4fv(sliTextShader, "u_Projection", 1, (float*)projection);
	shaderUniform1i(sliTextShader, "u_Texture", 0);
	shaderFinalize(sliTextShader);
}

void sliShadersDestroy()
{
	free(sliBasicShader);
	free(sliPointShader);
	free(sliTextureShader);
	free(sliTextShader);
}
