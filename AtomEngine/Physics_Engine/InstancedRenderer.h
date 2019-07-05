#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <map>
#include <vector>
#include <GLM/gtc/matrix_transform.hpp>

#include "ScreenManager.h"
#include "POD_Mesh.h"
#include "Renderer.h"
#include "ShaderManager.h"

class InstancedRenderer : public Renderer
{
public:
    InstancedRenderer() : Renderer()
    {
    }

    ~InstancedRenderer(){};

    virtual bool Initialize(const std::string& shaderID, const std::string& shaderFileName) override
    {
        m_shaderID = shaderID;

        Shaders::Instance()->AddShader(m_shaderID, shaderFileName);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
       

        return true;
    }


    virtual void AddToBuffer(std::vector<POD_SubMesh*>& meshList, const glm::mat4& transform) {
        for (auto i : meshList) {
            m_renderBuffer[i].push_back(transform);
        }
    }

    virtual void Render() override
    {
        Shaders::Instance()->UseShader(m_shaderID);

        for(auto& mesh : m_renderBuffer) {
            mesh.first->FillInstanceBuffer(mesh.second);
            mesh.first->Bind();
            glDrawElementsInstanced(GL_TRIANGLES, mesh.first->GetDrawCount(), GL_UNSIGNED_INT, 0, mesh.second.size());
            mesh.first->Unbind();
            mesh.second.clear();
        }
    }

private:
    std::map<POD_SubMesh*, std::vector<glm::mat4>> m_renderBuffer;
};

