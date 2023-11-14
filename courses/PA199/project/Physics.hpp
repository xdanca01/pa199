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
    Physics() : positionBall(0), positionsP(), positionsB() {};
    Physics(Petr_Math::Vector Pb, std::vector<Petr_Math::PolarCoordinates> PosPaddles, float Wp, float PhiP,
        std::vector<Petr_Math::PolarCoordinates> PosBricks, float Wb, float PhiB, float Rg) : positionBall(Pb),
        angleWidthBricks(PhiB), widthBricks(Wb), angleWidthPaddle(PhiP), widthPaddle(Wp), radiusGround(Rg), positionsB(PosBricks),
        positionsP(PosPaddles)
    {
        
    }

    Petr_Math::Vector CheckCollision()
    {
        float distanceFromCenter = sqrt(positionBall[0] * positionBall[0] + positionBall[1] * positionBall[1]);
        Petr_Math::Vector result(3, 0.0f);
        //GameOver
        if (distanceFromCenter - positionBall[2] > radiusGround)
        {
            //TODO GAME OVER
        }
        //Possible collision with paddle
        else if (distanceFromCenter + positionBall[2] >= positionsP[0].radius - widthPaddle &&
                 distanceFromCenter - positionBall[2] <= positionsP[0].radius + widthPaddle)
        {
            result = paddlePhase(positionsP, widthPaddle, angleWidthPaddle);
        }
        //Possible collision with bricks
        else if (distanceFromCenter + positionBall[2] >= positionsB[0].radius - widthBricks &&
                 distanceFromCenter - positionBall[2] <= positionsB[0].radius + widthBricks)
        {
            //TODO check collision with bricks
        }
        return result;
    }

    float minDifference(float first, float second)
    {
        return abs(first - second);
    }

    bool onLeft(float O, float Op)
    {
        if (O > Op) return true;
        return false;
    }

    Petr_Math::Vector closestPointOnLine(Petr_Math::Vector START, Petr_Math::Vector END, Petr_Math::Vector POINT)
    {
        float t = (POINT - START).dot(END - START) / (END - START).dot(END - START);
        if (t > 1.0f)
        {
            t = 1.0f;
        }
        else if (t < 0.0f)
        {
            t = 0.0f;
        }
        return START + (END - START) * t;
    }

    Petr_Math::Vector paddlePhase1(float Rp)
    {
        float lengthP = positionBall.magnitude();
        auto n = positionBall / lengthP;
        return lengthP < Rp ? n.opposite() : n;
    }

    Petr_Math::Vector paddlePhase2(float radiusBall, float angleBall, float Rp, float Op, float Wp, float angleWidthPaddle)
    {
        int sign = onLeft(angleBall, Op) ? 1 : -1;
        auto A = Petr_Math::PolarCoordinates(Rp - Wp, Op + sign * angleWidthPaddle).toCartesian();
        auto B = Petr_Math::PolarCoordinates(Rp + Wp, Op + sign * angleWidthPaddle).toCartesian();
        Petr_Math::Vector collisionPoint = closestPointOnLine(A, B, positionBall);
        auto tmp = positionBall - collisionPoint;
        return tmp.magnitude() > 0 && tmp.magnitude() < radiusBall ? tmp / tmp.magnitude() : Petr_Math::Vector(3, 0.0f);
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
};