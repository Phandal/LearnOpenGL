#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/affine.h>
#include <cglm/io.h>
#include <cglm/types.h>

#include "cglm/cam.h"
#include "shader.h"
#include "stb_image.h"

#include <stdio.h>
#include <math.h>

#define WIDTH 800.0
#define HEIGHT 600.0
#define TEXTURE_PATH1 "./assets/container.jpg"
#define TEXTURE_PATH2 "./assets/awesomeface.png"

float mix = 0.5;
int wireFrame = GL_FALSE;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  #pragma unused(window)
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    if (wireFrame == GL_TRUE) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      wireFrame = GL_FALSE;
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      wireFrame = GL_TRUE;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    if (mix < 1) {
      mix += 0.05f;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    if (mix > 0) {
      mix -= 0.05f;
    }
  }
}

int main(void) {

  /* Used later on */

  if(!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGl Tutorial", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to create a window\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwTerminate();
    return 1;
  }

  printf("GLFW Version: %s\n", glfwGetVersionString());
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float vertices[] = {
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f
  };

  vec3 cubePositions[] = {
    { 0.0f,  0.0f,  0.0f },
    { 2.0f,  5.0f, -15.0f },
    {-1.5f, -2.2f, -2.5f },
    {-3.8f, -2.0f, -12.3f },
    { 2.4f, -0.4f, -3.5f },
    {-1.7f,  3.0f, -7.5f },
    { 1.3f, -2.0f, -2.5f },
    { 1.5f,  2.0f, -2.5f },
    { 1.5f,  0.2f, -1.5f },
    {-1.3f,  1.0f, -1.5f },
  };

  vec3 modelRotateVec = { 1.0f, 0.3f, 0.5f };
  vec3 viewTranslateVec = { 0.0f, 0.0f, -3.0f };
  mat4 model;
  mat4 view;
  mat4 perspective;

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Position Attr
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Color Attr
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture Attr
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  int width, height, nrChannels;
  unsigned char* data;

  GLuint texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load(TEXTURE_PATH1, &width, &height, &nrChannels, 0);
  if (!data) {
    fprintf(stderr, "Failed to load texture file: %s\n", TEXTURE_PATH1);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);

  GLuint texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(1);
  data = stbi_load(TEXTURE_PATH2, &width, &height, &nrChannels, 0);
  if (!data) {
    fprintf(stderr, "Failed to load texture file: %s\n", TEXTURE_PATH2);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);

  GLuint shaderProgram;
  shaderProgram = createShaderProgram("./src/shaders/vertex.vs", "./src/shaders/fragment.fs");
  glUseProgram(shaderProgram); /* DO NOT FORGET TO ACTIVATE THE SHADER BEFORE SETTING UNIFORMS */
  glUniform1i(glGetUniformLocation(shaderProgram, "aTexture"), 0);
  glUniform1i(glGetUniformLocation(shaderProgram, "bTexture"), 1);
  glUniform1f(glGetUniformLocation(shaderProgram, "mixLevel"), mix);

  glEnable(GL_DEPTH_TEST);

  /* Uncomment the following line to render in wireframe mode */
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    glUseProgram(shaderProgram);

    glm_translate_make(view, viewTranslateVec);
    glm_perspective(glm_rad(45.0f), WIDTH/HEIGHT, 0.1f, 100.0f, perspective);

    glUniform1f(glGetUniformLocation(shaderProgram, "mixLevel"), mix);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewMatrix"), 1, GL_FALSE, (float*)view);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "perspectiveMatrix"), 1, GL_FALSE, (float*)perspective);

    for (int i = 0; i < 10; ++i) {
      glm_translate_make(model, cubePositions[i]);
      float angle = 20.0f * i;
      glm_rotate(model, glm_rad(angle), modelRotateVec);
      glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelMatrix"), 1, GL_FALSE, (float*)model);
      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteTextures(1, &texture1);
  glDeleteTextures(1, &texture2);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}
