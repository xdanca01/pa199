#pragma once
#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <math.h>

Camera::Camera() : eye_position(3, 0.0f), projection_matrix(4, 1.0f, true)
{
}

Petr_Math::Matrix Camera::get_view_matrix() const {
    Petr_Math::Vector zaxis = (Petr_Math::Vector(3, 0.0f) - eye_position).normalize(); //look direction to center
    Petr_Math::Vector up(0.0f, 1.0f, 0.0f);
    Petr_Math::Vector xaxis = zaxis.cross(up).normalize();
    Petr_Math::Vector yaxis = xaxis.cross(zaxis);
    float viewMatrixValues[16] = { 
        xaxis[0], xaxis[1], xaxis[2], -xaxis.dot(eye_position),
        yaxis[0], yaxis[1], yaxis[2], -yaxis.dot(eye_position),
        zaxis[0], zaxis[1], zaxis[2], -zaxis.dot(eye_position),
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return Petr_Math::Matrix(4, 4, viewMatrixValues);
}