#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/camera.h>
#include <learnopengl/shaders.h>

#include <iostream>

const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Camera related variables
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX = SRC_WIDTH / 2.0f;
float lastY = SRC_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

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
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // hide the cursoe and capture it (mouse camera movements, updating pitch + yaw)
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // GLAD manages function ptrs for OpenGL, load OpenGL function ptrs for MacOS
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  std::string VERTEX_PATH = "assets/shaders/basic.vert";
  std::string SHADER_PATH = "assets/shaders/basic.frag";
  Shader lightingShader(VERTEX_PATH.c_str(), SHADER_PATH.c_str());

  // Seperate shader for light source cube since we want illusion that it is emitting
  // the light.
  std::string LIGHT_SOURCE_VERTEX_PATH = "assets/shaders/light-source.vert";
  std::string LIGHT_SOURCE_SHADER_PATH = "assets/shaders/light-source.frag";
  Shader lightCubeShader(LIGHT_SOURCE_VERTEX_PATH.c_str(), LIGHT_SOURCE_SHADER_PATH.c_str());

  // Enable wireframe mode
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // enable depth testing
  glEnable(GL_DEPTH_TEST);

  float vertices[] = {
    // positions
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
  };


  unsigned int VAOs[2]; // 0 -> object, 1 -> light source
  unsigned int VBO;
  glGenVertexArrays(1, &VAOs[0]);
  glGenBuffers(1, &VBO);

  // bind Vertex Array object
  glBindVertexArray(VAOs[0]);
  // copy vertices array in a buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // set the vertex attributes pointers
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // unbind the VBO, call to glVertexAttribPointer has already registered VBO as
  // the vertex attribute's bound vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // unbind the VAO
  glBindVertexArray(0);

  glGenVertexArrays(1, &VAOs[1]);
  glBindVertexArray(VAOs[1]);
  // we only need to bind the VBO, the container's VBO's data already contains the data
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // set the vertex attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  lightingShader.use(); // Need to activate/use shader before setting uniforms
  lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

  // cube position in world space
  glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);
  // light source cube position in world space
  glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

  // render loop
  while (!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    processInput(window);

    // render commands
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // create a view matrix (world space -> view space)
    glm::mat4 view = camera.GetViewMatrix();

    // create a projection matrix (view space -> clip space)
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SRC_WIDTH / SRC_HEIGHT, 0.1f, 100.0f);

    // container local -> world
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePosition);

    // render the container 
    lightingShader.use();
    lightingShader.setMat4("view", view);
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("model", model);

    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // light source local -> world
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));

    // render the light source
    lightCubeShader.use();
    lightCubeShader.setMat4("model", model);
    lightCubeShader.setMat4("view", view);
    lightCubeShader.setMat4("projection", projection);

    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // unbind current vertex array object
    glBindVertexArray(0);

    // swap buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional, de-allocate resources
  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  const float cameraSpeed = 2.5f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // Set the viewport dimensions
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

