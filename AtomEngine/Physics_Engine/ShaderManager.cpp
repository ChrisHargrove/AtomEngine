#include "ShaderManager.h"
#include "LogManager.h"


void ShaderManager::Shutdown()
{
    //loop through container and delete all shaders.
    for (auto const& shader : m_shaderList) {
        delete shader.second;
    }
    //once all shaders are deleted, empty the container.
    m_shaderList.clear();
}

bool ShaderManager::AddShader(const std::string & name, const std::string & fileName)
{
    auto found = m_shaderList.find(name);
    if (found != m_shaderList.end()) {
        Logger::Instance()->LogWarning("Shader: " + name + " already exists.");
        Logger::Instance()->LogWarning("Shader: " + name + " has not been added.");
        return false;
    }
    else {
        Shader* newShader = new Shader(fileName);
        m_shaderList.emplace(std::make_pair(name, newShader));
#ifdef DEBUG
        Logger::Instance()->LogDebug("Shader: " + name + " Added successfully!");
#endif
    }
    return true;
}

bool ShaderManager::UseShader(const std::string & name)
{
    auto found = m_shaderList.find(name);
    if (found != m_shaderList.end()) {
        m_currentShader = name;
        m_shaderList.at(m_currentShader)->Use();
        return true;
    }
    Logger::Instance()->LogWarning("Shader: " + name + " does not exist!");
    Logger::Instance()->LogWarning("Shader: " + name + " cannot be used. Shader program has not changed.");
    return false;
}

Shader * ShaderManager::GetShader(const std::string & name)
{
    auto found = m_shaderList.find(name);
    if (found != m_shaderList.end()) {
        if (m_currentShader != name) UseShader(name);
        return m_shaderList.at(name);
    }
    Logger::Instance()->LogWarning("Shader: " + name + " does not exist!");
    Logger::Instance()->LogWarning("GetShader(" + name + ") return nullptr instead!");
    return nullptr;
}

Shader * ShaderManager::GetCurrentShader()
{
    if (m_currentShader != "") {
        return m_shaderList.at(m_currentShader);
    }
    return nullptr;
}

std::map<std::string, Shader*>* ShaderManager::GetShaderList()
{
    return &m_shaderList;
}
