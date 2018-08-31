#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <GLM/glm.hpp>

enum class BufferAttribute {
    POSITION = 0,
    NORMAL,
    UV,
    TANGENT,
    BITANGENT
};

struct ComplexVertex {
    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec2 m_UV;
    glm::vec3 m_tangent;
    glm::vec3 m_bitangent;
};

