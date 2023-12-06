#pragma once
#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <math.h>

Camera::Camera() : eye_position(3, 0.0f), projection_matrix(4, 1.0f, true), view_matrix(4, 1.0f, true)
{
}

void Camera::set_view_matrix(Petr_Math::Matrix viewMat)
{
    view_matrix = viewMat;
}

void Camera::set_view_matrix(Petr_Math::Vector position, Petr_Math::Vector target, Petr_Math::Vector up)
{
    Petr_Math::Vector f = (target - position).normalize();
    Petr_Math::Vector s = f.cross(up).normalize();
    Petr_Math::Vector u = s.cross(f).normalize();
    float viewMatrixValues[16] = {
        s[0], u[0], -f[0], 0.0f,
        s[1], u[1], -f[1], 0.0f,
        s[2], u[2], -f[2], 0.0f,
        -s.dot(position), -u.dot(position), f.dot(position), 1.0f
    };
    /*Petr_Math::Vector zaxis = (target - position).normalize(); //look direction to center
    Petr_Math::Vector xaxis = zaxis.cross(up).normalize();
    Petr_Math::Vector yaxis = xaxis.cross(zaxis).normalize();
    
    float viewMatrixValues[16] = {
        xaxis[0], xaxis[1], xaxis[2], -xaxis.dot(position),
        yaxis[0], yaxis[1], yaxis[2], -yaxis.dot(position),
        zaxis[0], zaxis[1], zaxis[2], zaxis.dot(position),
        0.0f, 0.0f, 0.0f, 1.0f
    };*/
    view_matrix = Petr_Math::Matrix(4, 4, viewMatrixValues).transpose();
    return;
}



Petr_Math::Matrix Camera::get_view_matrix() const {
    return view_matrix;
}