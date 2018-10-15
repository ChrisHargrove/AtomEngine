#pragma once

#include "PrimitiveShape.h"
#include "CubeMap.h"

#include <string>
#include <CEREAL/cereal.hpp>
#include <CEREAL/types/string.hpp>
#include <CEREAL/archives/xml.hpp>

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

class ATOM_API Skybox : public PrimitiveShape
{
public:
    Skybox();
    ~Skybox();

    void Load(const std::string& fileName = "");

    void Render() override;

private:
    std::string m_cubemapName;

    CubeMap* m_texture;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Skybox", m_cubemapName));
    }
};

