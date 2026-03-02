//
// Created by nil on 02/03/2026.
//

#ifndef KARTOFFEL_CAMERA_HPP
#define KARTOFFEL_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();

    [[nodiscard]] glm::mat4        GetViewMatrix() const;
    [[nodiscard]] static glm::mat4 GetProjectionMatrix(float aspect_ratio);

    void SetPosition(float x, float y, float z);
    void Move(float forward, float right, float up);
    void Rotate(float yaw_offset, float pitch_offset);

private:
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_{};
    glm::vec3 world_up_;

    float yaw_;
    float pitch_;

    float movement_speed_;
    float mouse_sensitivity_;

    void UpdateVectors();
};


#endif // KARTOFFEL_CAMERA_HPP
