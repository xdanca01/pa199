#pragma once
#include "RenderObject.hpp"
#include "PetrMath/PolarCoordinates.hpp"

class Brick{
public:
    //Vec3
    RenderObject render;
    Petr_Math::PolarCoordinates polarCoords;
    bool active;
    int life;
    float height;
    Brick(RenderObject ren, Petr_Math::PolarCoordinates PC, float h) : render(ren), polarCoords(PC.radius, PC.angle), active(true), height(h),life(2)
    {
    
    };
    void setActive(bool active)
    {
        this->active = active;
    }
    void hit()
    {
        life -= 1;
        render.modifyColor(Petr_Math::Vector(0.5f, 0.5f, 0.5f));
        if (life <= 0)
        {
            setActive(false);
            render.modifyColor(Petr_Math::Vector(0.0f, 0.0f, 0.0f));
        }
    }
};