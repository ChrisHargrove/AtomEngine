#pragma once

#include "RigidBody.h"

enum class Axis
{
    X = 0,
    Y,
    Z
};

struct EndPoint
{
    float m_position;
    RigidBody* m_body;
    bool m_start;

    EndPoint(float position, RigidBody* body, bool start)
    {
        m_position = position;
        m_body = body;
        m_start = start;
    }
};
