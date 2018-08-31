#include "Buffer.h"
#include "LogManager.h"


Buffer::Buffer() : m_ID(0)
{
}


Buffer::~Buffer()
{
}

void Buffer::Create(BufferType type)
{
    if (m_ID == 0) { //Stops buffer from generating multiple times.
        if (type == VAO) {
            glGenVertexArrays(1, &m_ID);
            m_type = type;
        }
        else {
            glGenBuffers(1, &m_ID);
            m_type = type;
        }
    }
}

void Buffer::Bind()
{
    if (m_type == VAO) {
        glBindVertexArray(m_ID);
    }
    else if (m_type == EBO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }
}

void Buffer::Unbind()
{
    if (m_type == VAO) {
        glBindVertexArray(0);
    }
    else if (m_type == EBO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Buffer::FillBuffer(int dataSize, const void * data, DrawType type)
{
    if (m_type != VAO) {
        Bind();
        if (m_type == EBO) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, type);
        }
        else {
            glBufferData(GL_ARRAY_BUFFER, dataSize, data, type);
        }
    }
    else {
        Logger::Instance()->LogWarning("You can't fill a Vertex Array Object with data!");
    }
}

void Buffer::Destroy()
{
    if (m_type == VAO) {
        glDeleteVertexArrays(1, &m_ID);
    }
    else {
        glDeleteBuffers(1, &m_ID);
    }
}

void Buffer::AddAttributePointer(unsigned int locationID, int size, VariableType type,int stride, int offset)
{
    if (m_type != VAO) {
        glVertexAttribPointer(locationID, size, type, GL_FALSE, stride, (void*)offset);
        glEnableVertexAttribArray(locationID);
    }
    else {
        Logger::Instance()->LogWarning("You can't assign a attribute pointer too a Vertex Array Object!");
    }
}

void Buffer::AddAttributePointer(BufferAttribute attribute, int size, VariableType type, int stride, int offset)
{
    AddAttributePointer((unsigned int)attribute, size, type, stride, offset);
}

unsigned int Buffer::GetID() const
{
    return m_ID;
}
