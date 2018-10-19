#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <string>
#include <CEREAL/cereal.hpp>
#include <CEREAL/types/string.hpp>
#include <CEREAL/archives/xml.hpp>

#include "CubeMap.h"
#include "Buffer.h"


class ATOM_API Skybox
{
public:
    Skybox();
    ~Skybox();

    void Load(const std::string& fileName = "");

    void Render();

    std::string GetName();

private:
    std::string m_cubemapName;
    CubeMap* m_texture;

    unsigned int m_drawCount;
    std::vector<float> m_vertices;

    Buffer m_vertexArray;
    Buffer m_vertexBuffer;

    friend class cereal::access;

    template<class Archive>
    void serialize(Archive& archive);
};

template <class Archive>
void Skybox::serialize(Archive& archive)
{
    archive(cereal::make_nvp("Skybox", m_cubemapName));
}
