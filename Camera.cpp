//
// Created by nil on 02/03/2026.
//

#include "Camera.hpp"
#include <iostream>

Camera::Camera() :
    position_(0.0f, 0.0f, 3.0f), front_(0.0f, 0.0f, -1.0f), up_(0.0f, 1.0f, 0.0f), world_up_(0.0f, 1.0f, 0.0f),
    yaw_(-90.0f), pitch_(0.0f), movement_speed_(2.5f), mouse_sensitivity_(0.1f) {
    UpdateVectors();
}

void Camera::UpdateVectors() {
    glm::vec3 front;
    front.x = static_cast<float>(cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)));
    front.y = static_cast<float>(sin(glm::radians(pitch_)));
    front.z = static_cast<float>(sin(glm::radians(yaw_)) * cos(glm::radians(pitch_)));
    front_  = glm::normalize(front);

    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_    = glm::normalize(glm::cross(right_, front_));
}


glm::mat4 Camera::GetViewMatrix() const { return glm::lookAt(position_, position_ + front_, up_); }

glm::mat4 Camera::GetProjectionMatrix(const float aspect_ratio) {
    return glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
}

void Camera::SetPosition(const float x, const float y, const float z) { position_ = glm::vec3(x, y, z); }

void Camera::Move(const float forward, const float right) {
    position_ += front_ * forward * movement_speed_;
    position_ += right_ * right * movement_speed_;
}
