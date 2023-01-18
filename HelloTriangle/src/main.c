#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

const char* vertexShaderSource = "#version 330 core\n"
  "layout(location = 0) in vec3 aPos;\n"
  "void main(){\n"
  "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
  "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
  "out vec4 fragColor;\n"
  "void main(){\n"
  "fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  #pragma unused(window)
  glViewport(0, 0, width, height);
}

void checkCompilationStatus(GLenum shaderType, GLuint shader){
  int compilationSuccess;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationSuccess);
  if(!compilationSuccess) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    fprintf(stderr, "Failed to compile %s shader\n", shaderType == GL_VERTEX_SHADER? "vertex" : "fragment");
    fprintf(stderr, "%s\n", infoLog);
  }
}

void checkLinkStatus(GLuint shaderProgram) {
  int compilationStatus;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &compilationStatus);
  if(!compilationStatus) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    fprintf(stderr, "Failed to link program\n");
    fprintf(stderr, "%s\n", infoLog);
  }
}

void processInput(GLFWwindow* window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

int main(void) {
  if(!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGl Tutorial", NULL, NULL);
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
     0.5f,  0.5f, 0.0f, // Top Right
     0.5f, -0.5f, 0.0f, // Bottom Right
    -0.5f, -0.5f, 0.0f, // Bottom Left
    -0.5f,  0.5f, 0.0f, // Top Left
  };

  unsigned int indicies[] = {
    0, 1, 3,
    1, 2, 3,
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  GLuint eob;
  glGenBuffers(1, &eob);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  checkCompilationStatus(GL_VERTEX_SHADER, vertexShader);

  GLuint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  checkCompilationStatus(GL_FRAGMENT_SHADER, fragmentShader);

  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  checkLinkStatus(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  /* Uncomment the following line to render in wireframe mode */
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}
