#include "Material.h"



SimpleMaterial::SimpleMaterial() :
    m_ambient(glm::vec3(.2f, .2f, .2f)),
    m_diffuse(glm::vec3(.5f, .5f, .5f)),
    m_specular(glm::vec3(1, 1, 1)),
    m_shininess(0.9f)
{
}

SimpleMaterial::SimpleMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
    m_ambient(ambient),
    m_diffuse(diffuse),
    m_specular(specular),
    m_shininess(shininess)
{
}


SimpleMaterial::~SimpleMaterial()
{
}
