#pragma once
#include "Renderer.h"
#include "ShaderManager.h"
#include "Skybox.h"

class SkyboxRenderer : public Renderer
{
public:
    SkyboxRenderer() {};
    virtual ~SkyboxRenderer() {}

    virtual bool Initialize(const std::string& shaderID, const std::string& shaderFileName) override
    {
        m_shaderID = shaderID;

        Shaders::Instance()->AddShader(m_shaderID, shaderFileName);

        Shaders::Instance()->UseShader(m_shaderID);
        Shaders::Instance()->GetCurrentShader()->SetMat4("model", glm::mat4(1.0f));
        Shaders::Instance()->GetCurrentShader()->SetInt("skybox", 0);

        return true;
    }

    virtual void Render() override
    {
        Shaders::Instance()->UseShader(m_shaderID);
        m_skybox.Render();
    }

    void SetSkybox(const std::string& filename) {
        m_skybox.Load(filename);
    }

private:
    Skybox m_skybox;
};

