#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <map>
#include <vector>

#include "ScreenManager.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "DebugCuboid.h"
#include <utility>

namespace glm {
    inline bool operator<(const glm::vec3& lhs, const glm::vec3& rhs) {
        return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
    }
}

class DebugRenderer : public Renderer
{
public:
    DebugRenderer() : Renderer()
    {
    }

    ~DebugRenderer() {};

    virtual bool Initialize(const std::string& shaderID, const std::string& shaderFileName) override
    {
        m_shaderID = shaderID;

        Shaders::Instance()->AddShader(m_shaderID, shaderFileName);

        glLineWidth(2);

        return true;
    }

    virtual void AddToBuffer(DebugCuboid* mesh, const glm::vec3& color, const glm::mat4& transform) {
        m_renderBuffer[std::make_pair(mesh, color)].push_back(transform);
    }

    virtual void Render() override
    {
        Shaders::Instance()->UseShader(m_shaderID);

        for (auto& mesh : m_renderBuffer) {
            if (mesh.second.empty()) continue;
            Shaders::Instance()->GetShader(m_shaderID)->SetVec3("aColor", mesh.first.second);

            mesh.first.first->FillInstanceBuffer(mesh.second);
            mesh.first.first->GetVertexArray()->Bind();
            glDrawElementsInstanced(GL_LINE_STRIP, mesh.first.first->GetDrawCount(), GL_UNSIGNED_INT, 0, mesh.second.size());
            mesh.first.first->GetVertexArray()->Unbind();
            mesh.second.clear();
        }
    }

private:
    std::map<std::pair<DebugCuboid*, glm::vec3>, std::vector<glm::mat4>> m_renderBuffer;

};

