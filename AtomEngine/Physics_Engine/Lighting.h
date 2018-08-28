#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <string>
#include <GLM/glm.hpp>

/*!
    * \class DirectionLight "Lighting.h"
    * \brief Creates a directional light.
    * 
    * Creates a directional light for use in the GPU programs. Is best used to simulate light
    * from orbital bodies such as the sun or moon. As all light travels in one directon without a position.
*/
class ATOM_API DirectionalLight
{
public:
    DirectionalLight();

    /*!
        * \brief Creates a directional Light.
        * \param direction The direction the light faces as a unit length vector.
        * \param ambient The colour of the ambient component of the  light.
        * \param diffuse The colour of the diffuse component of the light.
        * \param specular The colour of the specular component of the light.
        *
        * Creates the directional light and sets up the values used for the light at creation.
    */
    DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
    ~DirectionalLight();

    /*!
        * \brief Sets the light direction.
        * \param direction The new direction for the light.
        *
        * Sets the new direction of the light to match that given,
        * will normalize the direction to be unit length.
    */
    void SetDirection(const glm::vec3& direction);

    /*!
        * \brief Sets the ambient component of the light.
        * \param colour The new colour for the ambient component.
    */
    void SetAmbient(const glm::vec3& colour);

    /*!
    * \brief Sets the diffuse component of the light.
    * \param colour The new colour for the diffuse component.
    */
    void SetDiffuse(const glm::vec3& colour);

    /*!
    * \brief Sets the specular component of the light.
    * \param colour The new colour for the specular component.
    */
    void SetSpecular(const glm::vec3& colour);

private:
    glm::vec3 m_ambient;    /*!< The ambient component of the light. */
    glm::vec3 m_diffuse;    /*!< The diffuse component of the light. */
    glm::vec3 m_specular;   /*!< The specular component of the light. */
    glm::vec3 m_direction;  /*!< The direction of the light. */

};

/*!
    * \class PointLight "Lighting.h"
    * \brief Creates a point light.
    *
    * Creates a point light for use in the GPU programs. This emanates light in all directions from wherever
    * it is positioned. This is the most often used light for simulating obecjts like street lights or flames etc.
*/
class ATOM_API PointLight
{
public:
    PointLight();

    /*!
        * \brief Creates a point light.
        * \param position The Position of the light.
        * \param linear The linear component of the attentuation calculation.
        * \param quadratic The quadratic component of the attentuation calculation.
        * \param ambient The colour of the ambient component of the  light.
        * \param diffuse The colour of the diffuse component of the light.
        * \param specular The colour of the specular component of the light.
        *
        * Will create a point light with all parameters specified.
    */
    PointLight(const glm::vec3& position, float linear, float quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
    ~PointLight();

    /*!
        * \brief Sets the position of the light.
        * \param position The new position of the light.
    */
    void SetPosition(const glm::vec3& position);

    /*!
        * \brief Sets the ambient component of the light.
        * \param colour The new colour for the ambient component.
    */
    void SetAmbient(const glm::vec3& colour);

    /*!
        * \brief Sets the diffuse component of the light.
        * \param colour The new colour for the diffuse component.
    */
    void SetDiffuse(const glm::vec3& colour);

    /*!
        * \brief Sets the specular component of the light.
        * \param colour The new colour for the specular component.
    */
    void SetSpecular(const glm::vec3& colour);

    /*!
        * \brief Sets the linear component of the attenuation calculation.
        * \param value The value of the linear component of the attenuation calculation.
        *
        * Sets how the light attenuation will be calculated in a linear fashion. This will change how
        * light intensity drops over distance. The lower the value the further the light will travel.
    */
    void SetLinear(float value);

    /*!
        * \brief Sets the quadratic component of the attenuation calculation.
        * \param value The value of the quadratic component of the attenuation calculation.
        *
        * Sets how the light attenuation will be calculated in a quadratic fashion. This will change how
        * light intensity drops over distance. The lower the value the further the light will travel.
    */
    void SetQuadratic(float value);

private:
    glm::vec3 m_ambient;    /*!< The ambient component of the light. */
    glm::vec3 m_diffuse;    /*!< The diffuse component of the light. */
    glm::vec3 m_specular;   /*!< The specular component of the light. */
    glm::vec3 m_position;   /*!< The position of the light. */

    float m_linear;         /*!< The linear component of the attenuation calculation. */
    float m_quadratic;      /*!< The quadratic component of the attenuation calculation. */
    const float ATTENUATION_CONSTANT = 1.0f; /*!< The attentuation contant of the attenuation calculation. */
};

/*!
    * \class SpotLight "Lighting.h"
    * \brief Creates a spotlight.
    *
    * Creates a light that simulates a light that acts like a torch. It projects its light in a cone
    * in a set direction.
*/
class ATOM_API SpotLight
{
public:
    SpotLight();

    /*!
        * \brief Creates a spotlight.
        * \param position The Position of the light.
        * \param direction The direction of the light.
        * \param linear The linear component of the attentuation calculation.
        * \param quadratic The quadratic component of the attentuation calculation.
        * \param ambient The colour of the ambient component of the  light.
        * \param diffuse The colour of the diffuse component of the light.
        * \param specular The colour of the specular component of the light.
        *
        * Creates a spotlight with all the values supplied but will default the cone size.
    */
    SpotLight(const glm::vec3& position, const glm::vec3& direction, float linear, float quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

    /*!
        * \brief Creates a spotlight.
        * \param position The Position of the light.
        * \param direction The direction of the light.
        * \param innerAngle The angle of the inside cone of light in radians.
        * \param outerAngle The angle of the outer cone of light in radians.
        * \param linear The linear component of the attentuation calculation.
        * \param quadratic The quadratic component of the attentuation calculation.
        * \param ambient The colour of the ambient component of the  light.
        * \param diffuse The colour of the diffuse component of the light.
        * \param specular The colour of the specular component of the light.
        *
        * Creates a spotlight with all the values supplied.
    */
    SpotLight(const glm::vec3& position, const glm::vec3& direction,float innerAngle, float outerAngle, float linear, float quadratic, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

    ~SpotLight();

    /*!
        * \brief Sets the light direction.
        * \param direction The new direction for the light.
        *
        * Sets the new direction of the light to match that given,
        * will normalize the direction to be unit length.
    */
    void SetDirection(const glm::vec3& direction);

    /*!
        * \brief Sets the position of the light.
        * \param position The new position of the light.
    */
    void SetPosition(const glm::vec3& position);

    /*!
        * \brief Sets the ambient component of the light.
        * \param colour The new colour for the ambient component.
    */
    void SetAmbient(const glm::vec3& colour);

    /*!
        * \brief Sets the diffuse component of the light.
        * \param colour The new colour for the diffuse component.
    */
    void SetDiffuse(const glm::vec3& colour);

    /*!
        * \brief Sets the specular component of the light.
        * \param colour The new colour for the specular component.
    */
    void SetSpecular(const glm::vec3& colour);

    /*!
        * \brief Sets the inner cone angle for light.
        * \param angle The angle for the inner cone in radians.
    */
    void SetInnerCutOff(float angle);

    /*!
        * \brief Sets the outer cone angle for light.
        * \param angle The angle for the outer cone in radians.
    */
    void SetOuterCutOff(float angle);

    /*!
        * \brief Sets the linear component of the attenuation calculation.
        * \param value The value of the linear component of the attenuation calculation.
        *
        * Sets how the light attenuation will be calculated in a linear fashion. This will change how
        * light intensity drops over distance. The lower the value the further the light will travel.
    */
    void SetLinear(float value);

    /*!
        * \brief Sets the quadratic component of the attenuation calculation.
        * \param value The value of the quadratic component of the attenuation calculation.
        *
        * Sets how the light attenuation will be calculated in a quadratic fashion. This will change how
        * light intensity drops over distance. The lower the value the further the light will travel.
    */
    void SetQuadratic(float value);

private:
    glm::vec3 m_ambient;    /*!< The ambient component of the light. */
    glm::vec3 m_diffuse;    /*!< The diffuse component of the light. */
    glm::vec3 m_specular;   /*!< The specular component of the light. */
    glm::vec3 m_position;   /*!< The position of the light. */
    glm::vec3 m_direction;  /*!< The direction of the light. */

    float m_cutOffAngle;        /*!< The inner cut off angle for the inside cone of light. */
    float m_outerCutOffAngle;   /*!< The outer cut off angle for the outside cone of light. */
    float m_linear;             /*!< The linear component of the attenuation calculation. */
    float m_quadratic;          /*!< The quadratic component of the attenuation calculation. */
    const float ATTENUATION_CONSTANT = 1.0f; /*!< The attentuation contant of the attenuation calculation. */

};
