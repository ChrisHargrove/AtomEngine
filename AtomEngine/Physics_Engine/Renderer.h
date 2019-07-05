#pragma once
#include "Buffer.h"
#include "UniformBufferTypes.h"

class Renderer
{
public:
    Renderer() {
        };

    virtual ~Renderer(){};

    virtual bool Initialize(const std::string& shaderID, const std::string& shaderFileName) = 0;
    virtual void Render() = 0;

    

protected:
    std::string m_shaderID;

    
};

