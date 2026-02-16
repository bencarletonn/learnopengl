#include <learnopengl/camera.h>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

// construct with scalars
Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
  Position = glm::vec3(posX, posY, posZ);
  WorldUp = glm::vec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
};

// returns the view matrix calculated using Euler angles and the LookAt matrix
glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
};

// processes input received from any keyboard-like input system. Abstracts on
// top of windowing systems
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
  float velocity = MovementSpeed * deltaTime;
  if (direction == Camera_Movement::FORWARD)
    Position += Front * velocity;
  if (direction == Camera_Movement::BACKWARD)
    Position -= Front * velocity;
  if (direction == Camera_Movement::LEFT)
    Position -= Right * velocity;
  if (direction == Camera_Movement::RIGHT)
    Position += Right * velocity;
};

// processes input received from a mouse input system. Expects the offset value
// in both the x and y direction
void Camera::ProcessMouseMovement(float xoffset, float yoffset,
                                  GLboolean constrainPitch) {
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset;
  Pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    Pitch = std::min(Pitch, 89.0f);
    Pitch = std::max(Pitch, -89.0f);
  }
  // update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
};

// processes input received from a mouse scroll-wheel event. Only requires input
// on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset) {
   Zoom -= (float)yoffset;
   Zoom = std::max(Zoom, 1.0f);
   Zoom = std::min(Zoom, 45.0f);
};

void Camera::updateCameraVectors() {
  // calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  // also re-calculate the Right and Up vector
  Right = glm::normalize(glm::cross(
      Front, WorldUp)); // normalize the vectors, because their length gets
                        // closer to 0 the more you look up or down which
                        // results in slower movement.
  Up = glm::normalize(glm::cross(Right, Front));
};
