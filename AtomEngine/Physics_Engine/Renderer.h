#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "POD_Transform.h"
#include <map>
#include <vector>
#include "UniformBufferTypes.h"
#include "POD_Mesh.h"
#include "ScreenManager.h"

class Renderer
{
public:
    Renderer()
    {
        m_matrixBuffer = { Screen::Instance()->GetProjection(), glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)) };
        //Create the UniformBlock buffer for use with View and Projection Matrices.
        /*m_uniformBuffer.Create(sizeof(m_matrixBuffer));
        m_uniformBuffer.BindBuffer(UniformBufferBinding::MATRICES);
        m_uniformBuffer.SetSubData(&MatriceBuffer::m_projection, &m_matrixBuffer.m_projection);
        m_uniformBuffer.SetSubData(&MatriceBuffer::m_view, &m_matrixBuffer.m_view);*/
    }

    ~Renderer(){};

    void AddToBuffer(std::vector<POD_SubMesh*>& meshList, const glm::mat4& transform) {
        for (auto i : meshList)
        {
            m_renderBuffer[i].push_back(transform);
        }
    }

    void Render() {
        for(auto& mesh : m_renderBuffer) {
            mesh.first->FillInstanceBuffer(mesh.second);
            mesh.first->Bind();
            glDrawElementsInstanced(GL_TRIANGLES, mesh.first->m_drawCount, GL_UNSIGNED_INT, 0, mesh.second.size());
            mesh.first->Unbind();
            mesh.second.clear();
        }
    }

    UniformBuffer& GetUniformBuffer() {
        return m_uniformBuffer;
    }

private:
    MatriceBuffer m_matrixBuffer;
    UniformBuffer m_uniformBuffer;
    std::map<POD_SubMesh*, std::vector<glm::mat4>> m_renderBuffer;
};

