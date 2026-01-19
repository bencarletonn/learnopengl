#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSourceOne = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const char *fragmentShaderSourceTwo = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";


int main() {
  // Initialise and configure glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create a window object
  GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // register callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // GLAD manages function ptrs for OpenGL, load OpenGL function ptrs for MacOS
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // fragment shader one (orange)
  unsigned int fragmentShaderOne = glCreateShader(GL_FRAGMENT_SHADER);
  unsigned int fragmentShaderTwo = glCreateShader(GL_FRAGMENT_SHADER);
  unsigned int shaderProgramOne = glCreateProgram();
  unsigned int shaderProgramTwo = glCreateProgram();
  glShaderSource(fragmentShaderOne, 1, &fragmentShaderSourceOne, NULL);
  glShaderSource(fragmentShaderTwo, 1, &fragmentShaderSourceTwo, NULL);
  glCompileShader(fragmentShaderOne);
  glCompileShader(fragmentShaderTwo);

  glAttachShader(shaderProgramOne, vertexShader);
  glAttachShader(shaderProgramOne, fragmentShaderOne);
  glLinkProgram(shaderProgramOne);

  glAttachShader(shaderProgramTwo, vertexShader);
  glAttachShader(shaderProgramTwo, fragmentShaderTwo);
  glLinkProgram(shaderProgramTwo);

  glUseProgram(shaderProgramOne);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShaderOne);
  glDeleteShader(fragmentShaderTwo);

  float verticesTriangleOne[] = {
     0.5f,  0.5f, 0.0f, // top right
     0.5f, -0.5f, 0.0f, // bottom right
     0.0f, -0.5f, 0.0f, // middle
  };

  float verticesTriangleTwo[] = {
     0.0f, -0.5f, 0.0f, // middle
    -0.5f,  0.5f, 0.0f, // top left
    -0.5f, -0.5f, 0.0f, // bottom left
  };

  unsigned int VAOs[2], VBOs[2];
  glGenVertexArrays(2, VAOs);
  glGenBuffers(2, VBOs);

  // first
  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangleOne), verticesTriangleOne, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // second
  glBindVertexArray(VAOs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangleTwo), verticesTriangleTwo, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // render commands
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw the object
    glUseProgram(shaderProgramOne);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glUseProgram(shaderProgramTwo);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional, de-allocate resources
  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  glDeleteProgram(shaderProgramOne);
  glDeleteProgram(shaderProgramTwo);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // Set the viewport dimensions
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
