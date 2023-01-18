/* Create two shader programs where the second program uses a different fragment shader that outputs the color yellow; draw both triangles again where one outputs the color yellow */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

const char* vertexShaderSource = "#version 330 core\n"
  "layout(location = 0) in vec3 aPos;\n"
  "void main(){\n"
  "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
  "}\0";

const char* fragmentShaderSourceOrange = "#version 330 core\n"
  "out vec4 fragColor;\n"
  "void main(){\n"
  "fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";

const char* fragmentShaderSourceYellow = "#version 330 core\n"
  "out vec4 fragColor;\n"
  "void main(){\n"
  "fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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

  float firstTriangle[] = {
    -0.75f, -0.5f, 0.0f, // bot right
    -0.50f,  0.5f, 0.0f, // top middle
    -0.25f, -0.5f, 0.0f, // bot left
  };
  float secondTriangle[] = {
     0.25f, -0.5f, 0.0f, // bot right
     0.50f,  0.5f, 0.0f, // top middle
     0.75f, -0.5f, 0.0f, // bot left
  };

  GLuint VAOs[2];
  glGenVertexArrays(2, VAOs);

  GLuint VBOs[2];
  glGenBuffers(2, VBOs);

  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAOs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  checkCompilationStatus(GL_VERTEX_SHADER, vertexShader);

  GLuint fragmentShaderOrange;
  fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSourceOrange, NULL);
  glCompileShader(fragmentShaderOrange);
  checkCompilationStatus(GL_FRAGMENT_SHADER, fragmentShaderOrange);

  GLuint fragmentShaderYellow;
  fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
  glCompileShader(fragmentShaderYellow);
  checkCompilationStatus(GL_FRAGMENT_SHADER, fragmentShaderYellow);

  GLuint shaderProgramOrange;
  shaderProgramOrange = glCreateProgram();
  glAttachShader(shaderProgramOrange, vertexShader);
  glAttachShader(shaderProgramOrange, fragmentShaderOrange);
  glLinkProgram(shaderProgramOrange);
  checkLinkStatus(shaderProgramOrange);

  GLuint shaderProgramYellow;
  shaderProgramYellow = glCreateProgram();
  glAttachShader(shaderProgramYellow, vertexShader);
  glAttachShader(shaderProgramYellow, fragmentShaderYellow);
  glLinkProgram(shaderProgramYellow);
  checkLinkStatus(shaderProgramYellow);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShaderOrange);
  glDeleteShader(fragmentShaderYellow);

  /* Uncomment the following line to render in wireframe mode */
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgramOrange);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(shaderProgramYellow);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  glDeleteProgram(shaderProgramOrange);
  glDeleteProgram(shaderProgramYellow);
  glfwTerminate();
  return 0;
}
