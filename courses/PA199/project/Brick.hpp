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
    float wantedHeight;
    Brick(RenderObject ren, Petr_Math::PolarCoordinates PC, float h) : render(ren), polarCoords(PC.radius, PC.angle), active(true), height(h),life(2)
    {
        wantedHeight = height;
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

    void moveOnHeight(float deltaTime, float speed)
    {
        if (height > wantedHeight)
        {
            float diff = fminf(height - wantedHeight, deltaTime * speed);
            height -= diff;
            Petr_Math::Matrix newModel(4, 1.0f, true);
            Petr_Math::Vector spd(0.0f, -1.0f, 0.0f);
            newModel.translate(spd * diff);
            render.model = render.model * newModel;
        }
    }
};