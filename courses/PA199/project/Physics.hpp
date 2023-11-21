#pragma once

#include "PetrMath/Vector.hpp"
#include "PetrMath/PolarCoordinates.hpp"
#include "glad/glad.h"
#include <vector>


class Physics {
public:
    Petr_Math::Vector positionBall;
    float angleWidthBricks;
    float widthBricks;
    float angleWidthPaddle;
    float widthPaddle;
    float radiusGround;
    std::vector<Petr_Math::PolarCoordinates> positionsP;
    std::vector<Petr_Math::PolarCoordinates> positionsB;
    Petr_Math::Vector movement;
    float speed;

    Physics& operator =(Physics const& phys)
    {
        positionBall = Petr_Math::Vector(phys.positionBall);
        angleWidthBricks = phys.angleWidthBricks;
        widthBricks = phys.widthBricks;
        angleWidthPaddle = phys.angleWidthPaddle;
        widthPaddle = phys.widthPaddle;
        radiusGround = phys.radiusGround;
        positionsB = phys.positionsB;
        positionsP = phys.positionsP;
        movement = Petr_Math::Vector(phys.movement);
        speed = phys.speed;
        return *this;
    }


    Physics() : positionBall(0), positionsP(), positionsB(), movement(0) {};
    Physics(Petr_Math::Vector Pb, std::vector<Petr_Math::PolarCoordinates> PosPaddles, float Wp, float PhiP,
        std::vector<Petr_Math::PolarCoordinates> PosBricks, float Wb, float PhiB, float Rg, float ballSpeed) : positionBall(Pb),
        angleWidthBricks(PhiB), widthBricks(Wb), angleWidthPaddle(PhiP), widthPaddle(Wp), radiusGround(Rg), positionsB(PosBricks),
        positionsP(PosPaddles), movement(Pb.opposite()), speed(ballSpeed)
    {
        movement = Petr_Math::Vector(movement[0], 0.0f, movement[1]);
        movement = movement.normalize() * speed;
    }

    Petr_Math::Vector CheckCollision()
    {
        float distanceFromCenter = sqrt(positionBall[0] * positionBall[0] + positionBall[1] * positionBall[1]);
        Petr_Math::Vector result(4, 0.0f);
        //GameOver
        if (distanceFromCenter - positionBall[2] > radiusGround)
        {
            //TODO GAME OVER
        }
        //Possible collision with paddle
        else if (distanceFromCenter + positionBall[2] >= positionsP[0].radius - widthPaddle/2 &&
                 distanceFromCenter - positionBall[2] <= positionsP[0].radius + widthPaddle/2)
        {
            result = paddlePhase(positionsP, widthPaddle, angleWidthPaddle);
            result = Petr_Math::Vector(result[0], result[1], result[2], 1.0f);
        }
        //Possible collision with bricks
        else if (distanceFromCenter + positionBall[2] >= positionsB[0].radius - widthBricks/2 &&
                 distanceFromCenter - positionBall[2] <= positionsB[0].radius + widthBricks/2)
        {
            result = paddlePhase(positionsB, widthBricks, angleWidthBricks);
            result = Petr_Math::Vector(result[0], result[1], result[2], 0.0f);
        }
        return result;
    }

    float minDifference(float first, float second)
    {
        if (first > 180.0f)
        {
            first = -(360.0f - first);
        }
        if (second > 180.0f)
        {
            second = -(360.0f - second);
        }
        return abs(first - second);
    }

    bool onLeft(float O, float Op)
    {
        if (O > Op) return false;
        return true;
    }

    Petr_Math::Vector closestPointOnLine(Petr_Math::Vector START, Petr_Math::Vector END, Petr_Math::Vector POINT)
    {
        float t = (POINT - START).dot(END - START) / (END - START).dot(END - START);
        return START + (END - START) * t;
    }

    Petr_Math::Vector paddlePhase1(float Rp)
    {
        auto Pb = Petr_Math::Vector(positionBall[0], 0.0f, positionBall[1]);
        float lengthP = Pb.magnitude();
        auto n = Pb / lengthP;
        return lengthP < Rp ? n.opposite() : n;
    }

    Petr_Math::Vector paddlePhase2(float radiusBall, float angleBall, float Rp, float Op, float Wp, float angleWidthPaddle)
    {
        int sign = onLeft(angleBall, Op) ? 1 : -1;
        //Clockwise
        auto A = Petr_Math::PolarCoordinates(Rp - Wp, Op - sign * angleWidthPaddle).toCartesian();
        auto B = Petr_Math::PolarCoordinates(Rp + Wp, Op - sign * angleWidthPaddle).toCartesian();
        auto Pb = Petr_Math::Vector(positionBall[0], 0.0f, positionBall[1]);
        Petr_Math::Vector collisionPoint = closestPointOnLine(A, B, Pb);
        auto tmp = Pb - collisionPoint;
        return tmp.magnitude() > 0 && tmp.magnitude() <= radiusBall ? tmp / tmp.magnitude() : Petr_Math::Vector(3, 0.0f);
    }

    Petr_Math::Vector paddlePhase(std::vector<Petr_Math::PolarCoordinates> positions, float widthPaddle, float angleWidthPaddle)
    {
        Petr_Math::PolarCoordinates ball(positionBall);
        float Rp = positions[0].radius;
        float Op = positions[0].angle;
        for (int i = 1; i < positions.size(); ++i)
        {
            auto Opp = positions[i].angle;
            auto Rpp = positions[i].radius;
            if (minDifference(ball.angle, Opp) < minDifference(ball.angle, Op))
            {
                Rp = Rpp;
                Op = Opp;
            }
        }
        //Case 1
        if (minDifference(ball.angle, Op) <= angleWidthPaddle)
        {
            return paddlePhase1(Rp);
        }
        //Case 2
        else
        {
            return paddlePhase2(positionBall[2], ball.angle, Rp, Op, widthPaddle, angleWidthPaddle);
        }
    }

    Petr_Math::Vector moveBall(float paddlesSpeed)
    {
        auto n = this->CheckCollision();
        Petr_Math::Vector Vp(n[2], 0.0f, -n[0]);
        //TODO check moving and set right speed based on rotation (+- angle)
        Vp = Vp.normalize() * positionsP[0].radius *  paddlesSpeed;
        //Vp = Petr_Math::Vector(3, 0.0f);
        //n[3] is returned bool that says what type of collision happened
        if (n[3] < 0.5f)
        {
            Vp = Petr_Math::Vector(3, 0.0f);
        }
        //make it vec3
        n = Petr_Math::Vector(n[0], 0.0f, n[2]);
        n = n.normalize();
        auto deltaV = movement - Vp.normalize();
        //Friction coef
        float Up = 0.5f;
        if (deltaV.dot(n) < 0.0f)
        {
            auto deltaVn = n * n.dot(deltaV);
            auto deltaVt = deltaV - deltaVn;
            auto deltaVtt = Petr_Math::Vector(3, 0.0f);
            if (deltaVt.magnitude() > 0.0f)
            {
                deltaVtt = (deltaVt / deltaVt.magnitude()) * (-Up) * fminf(deltaVn.magnitude(), deltaVt.magnitude());
            }
            auto Vres = movement + (deltaVn.opposite() * 2) + deltaVtt;
            movement = Vres.normalize() * speed;
        }
        positionBall[0] += movement[0];
        positionBall[1] += movement[2];
        return movement;
    }
};