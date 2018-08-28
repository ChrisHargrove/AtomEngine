#include "ResourceManager.h"

bool ResourceManager::LoadTexture(const std::string & fileName, const std::string & textureName)
{
    auto found = m_textures.find(textureName);
    if (found != m_textures.end()) {
        Texture* loading = new Texture(fileName);
        m_textures.emplace(std::make_pair(textureName, loading));
        return true;
    }
    return false;
}

bool ResourceManager::LoadTextureSurface(const std::string & fileName, std::string & textureName)
{
    auto found = m_textures.find(textureName);
    if (found != m_textures.end()) {
        Texture* loading = new Texture();
        loading->LoadSurface(fileName);
        m_textures.emplace(std::make_pair(textureName, loading));
        return true;
    }
    return false;
}

bool ResourceManager::ConvertTextures()
{
    for (auto texture : m_textures) {
        if (!texture.second->Convert()) {
            return false;
        }
    }
    return true;
}

Texture * ResourceManager::GetTexture(const std::string & textureName)
{
    auto found = m_textures.find(textureName);
    if (found != m_textures.end()) {
        return m_textures.at(textureName);
    }
    return nullptr;
}
