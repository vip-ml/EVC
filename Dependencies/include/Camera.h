#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// Default camera values
const float ORBIT_RADIUS = 5.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float MIN_RADIUS = 1.0f;
const float MAX_RADIUS = 20.0f;

// Camera class definition
class Camera {
public:
    // Camera attributes
    glm::vec3 Target;      // Point the camera is orbiting around
    float Radius;          // Distance from the target
    glm::quat Orientation; // Camera orientation (quaternion)

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor
    Camera(glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), float radius = ORBIT_RADIUS)
        : Target(target), Radius(radius), Orientation(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f))),
        MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {}

    // Returns the view matrix calculated using the current camera parameters
    glm::mat4 GetViewMatrix() const {
        // Calculate the forward vector from the quaternion orientation
        glm::vec3 forward = glm::rotate(Orientation, glm::vec3(0.0f, 0.0f, -1.0f));
        glm::vec3 up = glm::rotate(Orientation, glm::vec3(0.0f, 1.0f, 0.0f));

        // Calculate the camera position based on the radius and forward vector
        glm::vec3 cameraPos = Target - forward * Radius;

        // Generate and return the view matrix
        return glm::lookAt(cameraPos, Target, up);
    }

    // Processes input received from any keyboard-like input system
    //void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    //    // In an orbital camera, we typically do not move the camera itself via keyboard
    //    // This function can be implemented to adjust the target or orbit behavior if needed
    //}

    // Processes mouse movement to update the camera's orientation
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        // Create quaternions for yaw (rotation around Y-axis) and pitch (rotation around X-axis)
        glm::quat qYaw = glm::angleAxis(glm::radians(xoffset), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat qPitch = glm::angleAxis(glm::radians(yoffset), glm::vec3(1.0f, 0.0f, 0.0f));

        // Update the camera's orientation by applying yaw and pitch
        Orientation = glm::normalize(qYaw * Orientation * qPitch);
    }

    // Processes input received from a mouse scroll-wheel event
    void ProcessMouseScroll(float yoffset) {
        Radius -= yoffset;
        if (Radius < MIN_RADIUS)
            Radius = MIN_RADIUS;
        if (Radius > MAX_RADIUS)
            Radius = MAX_RADIUS;
    }

    // Optionally, you can reset the camera's orientation
    void ResetOrientation() {
        Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
    }
};


