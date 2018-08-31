#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <GLM/glm.hpp>

/*!
    * \struct ComplexVertex Types.h
    * \brief This is a struct defining a complex vertex.
*/
struct ComplexVertex {
    glm::vec3 m_position;   /*!< A 3 component vector containing Position Data.*/
    glm::vec3 m_normal;     /*!< A 3 component vector containing Normal Data.*/
    glm::vec2 m_UV;         /*!< A 2 component vector containing Texture Cooridinate Data.*/
    glm::vec3 m_tangent;    /*!< A 3 component vector containing Tangent Data.*/
    glm::vec3 m_bitangent;  /*!< A 3 component vector containing Bitangent Data.*/
};

