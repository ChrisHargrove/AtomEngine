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

FrameBuffer::FrameBuffer() : m_ID(0)
{
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Create(glm::vec2 size)
{
    glGenFramebuffers(1, &m_ID);
    Bind();
    m_size = size;
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

GLuint FrameBuffer::GetTexture(unsigned int index)
{
    if (index >= m_colorAttachmentIDs.size()) {
        Logger::Instance()->LogError("[FRAMEBUFFER] Attempted to retrieve texture that doesn't exist, check index value!");
        return 0;
    }
    return m_colorAttachmentIDs[index];
}

void FrameBuffer::BindTexture(int index)
{
    if (index <= -1) {
        glBindTexture(GL_TEXTURE_2D, index);
        return;
    }
    if (index >= m_colorAttachmentIDs.size()) {
        Logger::Instance()->LogError("[FRAMEBUFFER] Attempted to bind texture that doesn't exist, check index value!");
        glBindTexture(GL_TEXTURE_2D, 0);
        return;
    }
    glBindTexture(GL_TEXTURE_2D, m_colorAttachmentIDs[index]);
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Destroy()
{
    glDeleteFramebuffers(1, &m_ID);
}


bool FrameBuffer::IsFrameBufferComplete()
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        Logger::Instance()->LogDebug("FrameBuffer Completed Succesfully!");
        Unbind();
        return true;
    }
    Logger::Instance()->LogError("FrameBuffer Failed Completion!");
    Unbind();
    return false;
}


void FrameBuffer::AddAttachment(AttachmentType type)
{
    unsigned int textureID;
    switch (type) {
    case TEXTURE:
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_size.x, m_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
        
        m_colorAttachmentIDs.push_back(textureID);
        break;
    case DEPTH:
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_size.x, m_size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
        m_depthAttachmentID = textureID;
        break;
    case STENCIL:
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, m_size.x, m_size.y, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
        m_stencilAttachmentID = textureID;
        break;
    case DEPTH_STENCIL:
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_size.x, m_size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
        m_depthAttachmentID = m_stencilAttachmentID = textureID;
        break;
    default:
        Logger::Instance()->LogError("Attachment Type not valid!");
    }
}

void FrameBuffer::AddRenderBuffer(AttachmentType type)
{
    RenderBuffer* buffer = new RenderBuffer();
    buffer->Create(type, m_size);

    switch (type) {
    case DEPTH_STENCIL:
        m_renderBuffers.push_back(buffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffers[0]->GetID());
        break;
    }
}

unsigned int FrameBuffer::GetID() const
{
    return m_ID;
}

RenderBuffer::RenderBuffer() : m_ID(0)
{
}

RenderBuffer::~RenderBuffer()
{
}

void RenderBuffer::Create(AttachmentType type, glm::vec2 size)
{
    glGenRenderbuffers(1, &m_ID);
    m_size = size;
    Bind();

    switch (type) {
    case DEPTH_STENCIL:
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
        break;
    default:
        Logger::Instance()->LogError("RenderBuffer AttachmentType not supported!");
    }
}

void RenderBuffer::Bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
}

void RenderBuffer::Unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBuffer::Destroy()
{
    glDeleteRenderbuffers(1, &m_ID);
}

unsigned int RenderBuffer::GetID()
{
    return m_ID;
}

void UniformBuffer::Create(int size)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::Destroy()
{
    glDeleteBuffers(1, &m_ID);
}

void UniformBuffer::BindBuffer(UniformBufferBinding bindingPoint)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_ID);
}

void UniformBuffer::SetData(void* data, unsigned int size)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetSubData(void * data, unsigned int offset, unsigned int size)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

unsigned int UniformBuffer::GetID()
{
    return m_ID;
}
