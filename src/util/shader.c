#include "util/shader.h"

#include "gl/glew.h"

#include <stdlib.h>
#include <stdio.h>

static void shaderPrintLog(GLuint program);
static GLint shaderGetUniLoc(Shader *shader, const char *name);

Shader *shaderLoad(const char *vertexCode, const char *fragmentCode)
{
	GLuint program = 0;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLint vertexCompiled;
    GLint fragCompiled;
	Shader *result = NULL;

	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);

    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
    if(!vertexCompiled)
    {
		fprintf(stderr, "shaderLoad() could not compile vertex shader\n");
		shaderPrintLog(vertexShader);
		exit(1);
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragCompiled);
    if(!fragCompiled)
    {
		fprintf(stderr, "shaderLoad() could not compile fragment shader\n");
		shaderPrintLog(fragmentShader);
		exit(1);
    }

    if(vertexCompiled && fragCompiled)
    {
        program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        result = (Shader*)malloc(sizeof(Shader));
        result -> program = program;
        result -> vertexShader = vertexShader;
        result -> fragmentShader = fragmentShader;
    }

    return result;
}

void shaderLink(Shader *shader)
{
	GLint linked = 0;

	glLinkProgram(shader -> program);
	glGetProgramiv(shader -> program, GL_LINK_STATUS, &linked);

	if(!linked)
	{
		fprintf(stderr, "shaderLink() could not link shader program\n");
		exit(1);
	}
}

void shaderBind(Shader *shader)
{
	glUseProgram(shader -> program);
}

void shaderUnbind()
{
	glUseProgram(0);
}

void shaderBindAttrib(Shader *shader, const char *var, unsigned int index)
{
    glBindAttribLocation(shader -> program, index, var);
}

void shaderFinalize(Shader *shader)
{
	glDetachShader(shader -> program, shader -> vertexShader);
	glDetachShader(shader -> program, shader -> fragmentShader);
    glDeleteShader(shader -> vertexShader);
    glDeleteShader(shader -> fragmentShader);
}

void shaderUniform1f(Shader *shader, const char *var, float val)
{
    glUniform1f(shaderGetUniLoc(shader, var), val);
}

void shaderUniform1i(Shader *shader, const char *var, int val)
{
    glUniform1i(shaderGetUniLoc(shader, var), val);
}

void shaderUniform1fv(Shader *shader, const char *var, int count, float *vals)
{
    glUniform1fv(shaderGetUniLoc(shader, var), count, vals);
}

void shaderUniform2f(Shader *shader, const char *var, float v1, float v2)
{
    glUniform2f(shaderGetUniLoc(shader, var), v1, v2);
}

void shaderUniform2fv(Shader *shader, const char *var, int count, float *vals)
{
    glUniform2fv(shaderGetUniLoc(shader, var), count, vals);
}

void shaderUniform3iv(Shader *shader, const char *var, int count, int *vals)
{
    glUniform3iv(shaderGetUniLoc(shader, var), count, vals);
}

void shaderUniform3fv(Shader *shader, const char *var, int count, float *vals)
{
    glUniform3fv(shaderGetUniLoc(shader, var), count, vals);
}

void shaderUniform3f(Shader *shader, const char *var, const float v1, const float v2, const float v3)
{
    glUniform3f(shaderGetUniLoc(shader, var), v1, v2, v3);
}

void shaderUniformMatrix3fv(Shader *shader, const char *var, GLsizei count, GLfloat *vals)
{
    glUniformMatrix3fv(shaderGetUniLoc(shader, var), count, GL_FALSE, vals);
}

void shaderUniform4iv(Shader *shader, const char *var, int count, int *vals)
{
    glUniform4iv(shaderGetUniLoc(shader, var), count, vals);
}

void shaderUniform4fv(Shader *shader, const char *var, int count, float *vals)
{
    glUniform4fv(shaderGetUniLoc(shader, var), count, vals);
}

void shaderUniform4f(Shader *shader, const char *var, float v1, float v2, float v3, float v4)
{
    glUniform4f(shaderGetUniLoc(shader, var), v1, v2, v3, v4);
}

void shaderUniformMatrix4fv(Shader *shader, const char *var, GLsizei count, GLfloat *vals)
{
    glUniformMatrix4fv(shaderGetUniLoc(shader, var), count, GL_FALSE, vals);
}

GLint shaderGetUniLoc(Shader *shader, const char *name)
{
    GLint result;
    result = glGetUniformLocation(shader -> program, name);

	if(result == -1)
	{
		fprintf(stderr, "shaderGetUniLoc(): uniform '%s' has not been defined\n", name);
		exit(1);
	}

    return result;
}

void shaderPrintLog(GLuint program)
{
    int infologLength = 0;
    int charsWritten = 0;
    char *infoLog;

	glGetShaderiv(program, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 1)
	{
		infoLog = (char*)malloc(infologLength);
		glGetShaderInfoLog(program, infologLength, &charsWritten, infoLog);
		fprintf(stderr, "%s\n", infoLog);
		free(infoLog);
	}
}
