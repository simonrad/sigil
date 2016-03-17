#include "assets/shader.h"

#include <glm/glm.hpp>
#include <gl/glew.h>

#include <fstream>
#include <cstdlib>
#include <iostream>
using namespace std;
using namespace glm;

Shader::Shader(const char *vertexCode, const char *fragmentCode)
{
    GLint vertexCompiled;
    GLint fragCompiled;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);

    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
    if(!vertexCompiled)
    {
		cout << "Shader::Shader() could not compile vertex shader " << endl;
		printShaderLogInfo(vertexShader);
		exit(1);
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragCompiled);
    if(!fragCompiled)
    {
		cout << "Shader::Shader() could not compile fragment shader " << endl;
		printShaderLogInfo(fragmentShader);
		exit(1);
    }

    if(vertexCompiled && fragCompiled)
    {
        program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
    }
}

Shader::~Shader()
{
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(program);
}

void Shader::link()
{
	GLint linked = false;

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if(!linked)
	{
		cerr << "Shader::Shader() compiled but could not be linked" << endl;
		printShaderLogInfo(vertexShader);
		printShaderLogInfo(fragmentShader);
		exit(1);
	}
}

void Shader::bind()
{
	glUseProgram(program);
}

void Shader::unbind()
{
    glUseProgram(0);
}

void Shader::bindAttrib(const char *var, unsigned int index)
{
    glBindAttribLocation(program, index, var);
}

void Shader::uniform1f(const char *var, float val)
{
    glUniform1f(getUniLoc(program, var), val);
}

void Shader::uniform1i(const char *var, int val)
{
    glUniform1i(getUniLoc(program, var), val);
}

void Shader::uniform1fv(const char *var, int count, float *vals)
{
    glUniform1fv(getUniLoc(program, var), count, vals);
}

void Shader::uniform2f(const char *var, float v1, float v2)
{
    glUniform2f(getUniLoc(program, var), v1, v2);
}

void Shader::uniform2fv(const char *var, int count, float *vals)
{
    glUniform2fv(getUniLoc(program, var), count, vals);
}

void Shader::uniformVec2(const char *var, vec2 v)
{
    uniform2f(var, v.x, v.y);
}

void Shader::uniform3iv(const char *var, int count, int *vals)
{
    glUniform3iv(getUniLoc(program, var), count, vals);
}

void Shader::uniform3fv(const char *var, int count, float *vals)
{
    glUniform3fv(getUniLoc(program, var), count, vals);
}

void Shader::uniform3f(const char *var, const float v1, const float v2, const float v3)
{
    glUniform3f(getUniLoc(program, var), v1, v2, v3);
}

void Shader::uniformVec3(const char *var, vec3 v)
{
    uniform3f(var, v.x, v.y, v.z);
}

void Shader::uniformMatrix3fv(const char *var, GLsizei count, GLfloat *vals, bool transpose)
{
    glUniformMatrix3fv(getUniLoc(program, var), count, transpose, vals);
}

void Shader::uniform4iv(const char *var, int count, int *vals)
{
    glUniform4iv(getUniLoc(program, var), count, vals);
}

void Shader::uniform4fv(const char *var, int count, float *vals)
{
    glUniform4fv(getUniLoc(program, var), count, vals);
}

void Shader::uniform4f(const char *var, float v1, float v2, float v3, float v4)
{
    glUniform4f(getUniLoc(program, var), v1, v2, v3, v4);
}

void Shader::uniformVec4(const char *var, vec4 v)
{
    uniform4f(var, v.x, v.y, v.z, v.w);
}

void Shader::uniformMatrix4fv(const char *var, GLsizei count, GLfloat *vals, bool transpose)
{
    glUniformMatrix4fv(getUniLoc(program, var), count, transpose, vals);
}

GLint Shader::getUniLoc(GLuint program, const char *name)
{
    GLint loc;
    loc = glGetUniformLocation(program, name);

	if(loc == -1)
	{
		cerr << "Shader::getUniLoc(): uniform '" << name << "' has not been defined" << endl;
		exit(1);
	}

    return loc;
}

void Shader::printShaderLogInfo(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 1)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		cout << infoLog << endl;
		free(infoLog);
	}

}
