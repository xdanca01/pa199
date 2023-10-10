#pragma once
#include "PetrMath/Matrix.hpp"
#include "PetrMath/Vector.hpp"

class Camera{
public:
    //Vec3
    Petr_Math::Vector eye_position;
    Petr_Math::Matrix projection_matrix;
    Camera();
public:
    //4x4
    Petr_Math::Matrix get_view_matrix() const;
};