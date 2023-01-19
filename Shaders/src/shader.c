#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>

void _checkCompilationStatus(GLenum shaderType, GLuint shader){
  int compilationSuccess;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccess);
  if(!compilationSuccess) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    fprintf(stderr, "Failed to compile %s shader\n", shaderType == GL_VERTEX_SHADER? "vertex" : "fragment");
    fprintf(stderr, "%s\n", infoLog);
  }
}

void _checkLinkStatus(GLuint shaderProgram) {
  int compilationStatus;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &compilationStatus);
  if(!compilationStatus) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    fprintf(stderr, "Failed to link program\n");
    fprintf(stderr, "%s\n", infoLog);
  }
}

char* _readFileToString(const char* filePath) {
  FILE* fd = fopen(filePath, "r");
  if (!fd) {
    fprintf(stderr, "Failed to open shader source file: %s\n", filePath);
    return NULL;
  }
  char* buffer;
  fseek(fd, 0, SEEK_END);
  long length = ftell(fd);
  fseek(fd, 0, SEEK_SET);
  buffer = malloc(length);
  fread(buffer, 1, length, fd);
  fclose(fd);

  buffer[length-1] = '\0';
  return buffer;
}

GLuint _compileShader(GLenum shaderType, const char* shaderSource) {
  GLuint shader;
  shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);
  _checkCompilationStatus(shaderType, shader);

  return shader;
}

void setFloatUniform(GLuint shaderProgram, const char* uniform, float value) {
  GLint uniformLocation = glGetUniformLocation(shaderProgram, uniform);
  if (uniformLocation < 0) {
    fprintf(stderr, "Could not find uniform in shader: %s\n", uniform);
    return;
  }
  glUniform1f(uniformLocation, value);
}

GLuint createShaderProgram(const char* vertexFilePath, const char* fragmentFilePath) {
  char* vsSource = _readFileToString(vertexFilePath);
  char* fsSource = _readFileToString(fragmentFilePath);
  GLuint vs = _compileShader(GL_VERTEX_SHADER, vsSource);
  GLuint fs = _compileShader(GL_FRAGMENT_SHADER, fsSource);

  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vs);
  glAttachShader(shaderProgram, fs);
  glLinkProgram(shaderProgram);

  glDeleteShader(vs);
  glDeleteShader(fs);
  free(vsSource);
  free(fsSource);
  
  return shaderProgram;
}
