#include <learnopengl/camera.h>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
}
// construct with scalars
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {};
// returns the view matrix calculated using Euler angles and the LookAt matrix
glm::mat4 Camera::GetViewMatrix() {};
// processes input received from any keyboard-like input system. Abstracts on top of windowing systems
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {};
// processes input received from a mouse input system. Expects the offset value in both the x and y direction
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {};
// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset) {};
void Camera::updateCameraVectors() {};
