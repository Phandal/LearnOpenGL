#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

GLuint createShaderProgram(const char* vertexFilePath, const char* fragmentFilePath);

void setFloatUniform(GLuint shaderProgram, const char* uniform, float value);

#endif
