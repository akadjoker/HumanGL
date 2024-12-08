
#pragma once
#include "Config.hpp"
#include "Math.hpp"
#include <iostream>

class Camera        
{
private:
    Vec3 position;
    Vec3 front;
    Vec3 up;
    Vec3 right;
    Vec3 worldUp;

    float yaw;
    float pitch;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

public:
    Camera(
        float aspect = 800.0f / 600.0f,
        const Vec3 &pos = Vec3(0.0f, 1.5f, 12.0f),
        const Vec3 &up = Vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f) : position(pos),
                              worldUp(up),
                              yaw(yaw),
                              pitch(pitch),
                              fov(45.0f),
                              aspectRatio(aspect),
                              nearPlane(0.1f),
                              farPlane(1000.0f)
    {
        updateCameraVectors();
    }

    Mat4 getViewMatrix() const
    {
        return Mat4::LookAt(position, position + front, up);
    }

    Mat4 getProjectionMatrix() const
    {
        return Mat4::Perspective(fov * M_PI / 180.0f, aspectRatio, nearPlane, farPlane);
    }

    void move(float distance)
    {
        position = position + (front * distance);
    }

    void strafe(float distance)
    {
        position = position + (right * distance);
    }

    void moveUp(float distance)
    {
        position = position + up * distance;
    }

    Vec3 getPosition() const { return position; }
    Vec3 getFront() const { return front; }
    Vec3 getUp() const { return up; }
    Vec3 getRight() const { return right; }

    void rotate(float xoffset, float yoffset, float sensitivity = 0.1f)
    {
        yaw += xoffset * sensitivity;
        pitch += yoffset * sensitivity;

        yaw = std::fmod(yaw, 360.0f);
        if (yaw < 0)
            yaw += 360.0f;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        updateCameraVectors();
    }

    void zoom(float yoffset)
    {
        fov -= yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }

    void setAspectRatio(float ratio)
    {
        aspectRatio = ratio;
    }

private:
    void updateCameraVectors()
    {
        Vec3 newFront;
        float yawRad = yaw * M_PI / 180.0f;     // Converter para radianos
        float pitchRad = pitch * M_PI / 180.0f; // Converter para radianos

        newFront.setX(std::cos(yawRad) * std::cos(pitchRad));
        newFront.setY(std::sin(pitchRad));
        newFront.setZ(std::sin(yawRad) * std::cos(pitchRad));

        front = newFront.normalize();
        right = front.cross(worldUp).normalize();
        up = right.cross(front).normalize();
    }
};