/* See if you can transform the camera class in such a way that it becomes a true fps camera where you cannot fly; you can only look around while staying on the xz plane */
/* NOTE: I modified this so LShift makes you fly down and Space makes you fly up */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/affine.h>
#include <cglm/io.h>
#include <cglm/types.h>
#include <cglm/cam.h>
#include <cglm/vec3.h>

#include "shader.h"
#include "stb_image.h"

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 1200.0
#define HEIGHT 900.0
#define TEXTURE_PATH1 "./assets/container.jpg"
#define TEXTURE_PATH2 "./assets/awesomeface.png"

float mix = 0.5;
int wireFrame = GL_FALSE;
vec3 cameraPos = { 0.0f, 0.0f, 3.0f };
vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
vec3 cameraUp = {0.0f, 1.0f, 0.0f};
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float yaw = -90.0f;
float pitch = 0;
float fov = 45.0f;
float lastXPos = 400;
float lastYPos = 600;
const float sensitivity = 0.1f;
bool firstMouse = true;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  #pragma unused(window)
  glViewport(0, 0, width, height);
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
  #pragma unused(window)

  if (firstMouse) {
    lastXPos = xpos;
    lastYPos = ypos;
    firstMouse = false;
  }
  float xOffset = xpos - lastXPos;
  float yOffset = lastYPos - ypos;
  lastXPos = xpos;
  lastYPos = ypos;

  xOffset *= sensitivity;
  yOffset *= sensitivity;

  yaw += xOffset;
  pitch += yOffset;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  }
  if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  vec3 direction = { 0.0f, 0.0f, 0.0f };
  direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
  direction[1] = sin(glm_rad(pitch));
  direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
  glm_vec3_normalize(direction);
  glm_vec3_copy(direction, cameraFront);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  #pragma unused(window, xoffset)
  fov -= (float)yoffset;
  if (fov < 1.0f) {
    fov = 1.0f;
  }
  if (fov > 70.0f) {
    fov = 70.0f;
  }
}

void processInput(GLFWwindow* window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
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

  const float cameraSpeed = 2.5f * deltaTime;
  vec3 movementVec;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    glm_vec3_scale(cameraFront, cameraSpeed, movementVec);
    movementVec[1] = 0.0f;
    glm_vec3_add(cameraPos, movementVec, cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    glm_vec3_scale(cameraFront, cameraSpeed, movementVec);
    movementVec[1] = 0.0f;
    glm_vec3_sub(cameraPos, movementVec, cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    vec3 cameraRight;
    glm_cross(cameraFront, cameraUp, cameraRight);
    glm_vec3_normalize(cameraRight);
    glm_vec3_scale(cameraRight, cameraSpeed, movementVec);
    glm_vec3_sub(cameraPos, movementVec, cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    vec3 cameraRight;
    glm_cross(cameraFront, cameraUp, cameraRight);
    glm_vec3_normalize(cameraRight);
    glm_vec3_scale(cameraRight, cameraSpeed, movementVec);
    glm_vec3_add(cameraPos, movementVec, cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    vec3 yVec = { 0.0f, cameraSpeed, 0.0f };
    glm_vec3_sub(cameraPos, yVec, cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    vec3 yVec = { 0.0f, cameraSpeed, 0.0f };
    glm_vec3_add(cameraPos, yVec, cameraPos);
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

  // GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGl Tutorial", glfwGetPrimaryMonitor(), NULL);
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

  /* Uncomment to print out the GLFW Version and OpenGL Version */
  // printf("GLFW Version: %s\n", glfwGetVersionString());
  // printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, cursor_pos_callback);
  glfwSetScrollCallback(window, scroll_callback);

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
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    glUseProgram(shaderProgram);

    vec3 cameraTarget;
    glm_vec3_add(cameraFront, cameraPos, cameraTarget);
    glm_lookat(cameraPos, cameraTarget, cameraUp, view);
    glm_perspective(glm_rad(fov), WIDTH/HEIGHT, 0.1f, 100.0f, perspective);

    glUniform1f(glGetUniformLocation(shaderProgram, "mixLevel"), mix);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewMatrix"), 1, GL_FALSE, (float*)view);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "perspectiveMatrix"), 1, GL_FALSE, (float*)perspective);

    for (int i = 0; i < 10; ++i) {
      glm_translate_make(model, cubePositions[i]);
      // float angle = 20.0f * i;
      float angle = glfwGetTime() * 25.0f + (20.0f * i);
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
