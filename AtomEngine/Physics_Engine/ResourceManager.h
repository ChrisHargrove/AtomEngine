#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "AlignedAllocation.h"
#include "Singleton.h"
#include "Texture.h"

#include <map>
#include <string>

class ATOM_API ResourceManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<ResourceManager>;

    bool LoadTexture(const std::string& fileName, const std::string& textureName);
    bool LoadTextureSurface(const std::string& fileName, std::string& textureName);
    bool ConvertTextures();

    Texture* GetTexture(const std::string& textureName);

private:
    ResourceManager() {};
    ResourceManager(const ResourceManager&) {};
    //ResourceManager& operator=(const ResourceManager&) {};

    std::map<std::string, Texture*> m_textures;
};

template ATOM_API ResourceManager* Singleton<ResourceManager>::Instance();

typedef Singleton<ResourceManager> Resource;

