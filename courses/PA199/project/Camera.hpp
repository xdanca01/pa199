#pragma once
#include "PetrMath/Matrix.hpp"
#include "PetrMath/Vector.hpp"

class Camera{
public:
    //Vec3
    Petr_Math::Vector eye_position;
    Petr_Math::Matrix view_matrix;
    Petr_Math::Matrix projection_matrix;
    Camera();
public:
    void set_view_matrix(Petr_Math::Vector position, Petr_Math::Vector target, Petr_Math::Vector up);
    //4x4
    Petr_Math::Matrix get_view_matrix() const;
};