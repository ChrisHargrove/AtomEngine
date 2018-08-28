#include "Lighting.h"

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(const glm::vec3 & direction, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular)
{
    m_direction = direction;
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::SetDirection(const glm::vec3 & direction)
{
    m_direction = direction;
}

void DirectionalLight::SetAmbient(const glm::vec3 & colour)
{
    m_ambient = colour;
}

void DirectionalLight::SetDiffuse(const glm::vec3 & colour)
{
    m_diffuse = colour;
}

void DirectionalLight::SetSpecular(const glm::vec3 & colour)
{
    m_specular = colour;
}

PointLight::PointLight()
{
}

PointLight::PointLight(const glm::vec3 & position, float linear, float quadratic, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular)
{
    m_position = position;
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_linear = linear;
    m_quadratic = quadratic;
}

PointLight::~PointLight()
{
}

void PointLight::SetPosition(const glm::vec3 & position)
{
    m_position = position;
}

void PointLight::SetAmbient(const glm::vec3 & colour)
{
    m_ambient = colour;
}

void PointLight::SetDiffuse(const glm::vec3 & colour)
{
    m_diffuse = colour;
}

void PointLight::SetSpecular(const glm::vec3 & colour)
{
    m_specular = colour;
}

void PointLight::SetLinear(float value)
{
    m_linear = value;
}

void PointLight::SetQuadratic(float value)
{
    m_quadratic = value;
}

SpotLight::SpotLight()
{
}

SpotLight::SpotLight(const glm::vec3 & position, const glm::vec3 & direction, float linear, float quadratic, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular)
{
    m_position = position;
    m_direction = direction;
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_linear = linear;
    m_quadratic = quadratic;
    m_cutOffAngle = 12.5f;
    m_outerCutOffAngle = 17.5f;
}

SpotLight::SpotLight(const glm::vec3 & position, const glm::vec3 & direction, float innerAngle, float outerAngle, float linear, float quadratic, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular)
{
    m_position = position;
    m_direction = direction;
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_linear = linear;
    m_quadratic = quadratic;
    m_cutOffAngle = innerAngle;
    m_outerCutOffAngle = outerAngle;
}

SpotLight::~SpotLight()
{
}

void SpotLight::SetDirection(const glm::vec3 & direction)
{
    m_direction = direction;
}

void SpotLight::SetPosition(const glm::vec3 & position)
{
    m_position = position;
}

void SpotLight::SetAmbient(const glm::vec3 & colour)
{
    m_ambient = colour;
}

void SpotLight::SetDiffuse(const glm::vec3 & colour)
{
    m_diffuse = colour;
}

void SpotLight::SetSpecular(const glm::vec3 & colour)
{
    m_specular = colour;
}

void SpotLight::SetInnerCutOff(float angle)
{
    m_cutOffAngle = angle;
}

void SpotLight::SetOuterCutOff(float angle)
{
    m_outerCutOffAngle = angle;
}

void SpotLight::SetLinear(float value)
{
    m_linear = value;
}

void SpotLight::SetQuadratic(float value)
{
    m_quadratic = value;
}
