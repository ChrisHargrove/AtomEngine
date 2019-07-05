#pragma once
#include "Renderer.h"

#include <GLM/gtc/matrix_transform.hpp>
#include "ScreenManager.h"
#include "Quad.h"
#include "ShaderManager.h"

class PostRenderer : public Renderer
{
public:
    PostRenderer(){};
    virtual ~PostRenderer(){};

    bool Initialize(const std::string& shaderID, const std::string& shaderFileName) override
    {
        m_shaderID = shaderID;

        Shaders::Instance()->AddShader(m_shaderID, shaderFileName);
        
        m_screenFrame = new Quad(true);

        m_frameBuffer.Create(Screen::Instance()->GetSize());
        m_frameBuffer.AddAttachment(TEXTURE);
        m_frameBuffer.AddRenderBuffer(DEPTH_STENCIL);

        Shaders::Instance()->UseShader(m_shaderID);
        Shaders::Instance()->GetShader(m_shaderID)->SetInt("myTexture", 0);

        Screen::Instance()->Enable3D();

        m_matrixBuffer = { Screen::Instance()->GetProjection(), glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)) };
        Shaders::Instance()->SetUniformBufferBinding("Matrices", UniformBufferBinding::MATRICES);
        //Create the UniformBlock buffer for use with View and Projection Matrices.
        m_uniformBuffer.Create(sizeof(m_matrixBuffer));
        m_uniformBuffer.BindBuffer(UniformBufferBinding::MATRICES);
        m_uniformBuffer.SetSubData(&MatriceBuffer::m_projection, &m_matrixBuffer.m_projection);
        m_uniformBuffer.SetSubData(&MatriceBuffer::m_view, &m_matrixBuffer.m_view);

        return m_frameBuffer.IsFrameBufferComplete();
    }

    void Render() override
    {
        m_frameBuffer.Unbind();
        Screen::Instance()->EnableDepthTesting(false);
        Screen::Instance()->Clear(ClearBits::COLOR);

        Shaders::Instance()->UseShader(m_shaderID);
        m_frameBuffer.BindTexture(0);
        m_screenFrame->Render();
        m_frameBuffer.BindTexture();
    }

    void BeginFrame() {
        m_frameBuffer.Bind();
        Screen::Instance()->EnableDepthTesting(true);
        Screen::Instance()->Clear();
    }

    virtual void SetViewMatrix(const glm::mat4& view) {
        m_uniformBuffer.SetSubData(&MatriceBuffer::m_view, &view);
    }

    UniformBuffer& GetUniformBuffer() {
        return m_uniformBuffer;
    }

private:
    Quad* m_screenFrame;
    FrameBuffer m_frameBuffer;

    MatriceBuffer m_matrixBuffer;
    UniformBuffer m_uniformBuffer;
};

