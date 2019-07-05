#include "Skybox.h"

#include "ShaderManager.h"

Skybox::Skybox() :
m_cubemapName(""),
m_texture(nullptr),
m_drawCount(0)
{
}


Skybox::~Skybox()
{
    m_vertexArray.Destroy();
    m_vertexBuffer.Destroy();
    delete m_texture;

    //TODO: Need to change this so it doesn't delete resources, and that resource manager gets used instead!!.
}

void Skybox::Load(const std::string& fileName)
{
    if(fileName.empty())
    {
        m_texture = new CubeMap(m_cubemapName);
    }else
    {
        m_texture = new CubeMap(fileName);
    }
    

    m_vertices = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    m_vertexArray.Create(VAO);
    m_vertexBuffer.Create(VBO);

    m_vertexArray.Bind();
    m_vertexBuffer.FillBuffer(sizeof(float) * m_vertices.size(), &m_vertices[0], STATIC);
    m_vertexBuffer.AddAttributePointer(BufferAttribute::POSITION, 3, VT_FLOAT, 3 * sizeof(float));

    m_drawCount = 36;
    m_vertexArray.Unbind();
}

void Skybox::Render()
{
    glDepthFunc(GL_LEQUAL);

    
    m_vertexArray.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture->GetID());
    glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    m_vertexArray.Unbind();

    glDepthFunc(GL_LESS);

}

std::string Skybox::GetName()
{
    return m_cubemapName;
}
