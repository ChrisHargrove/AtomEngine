/*!
    * \class Buffer "Buffer.h"
    * \brief Contains all data for GPU buffers.
    * 
    * Handles the creation of all GPU buffers, writing data to them and their destruction.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "AlignedAllocation.h"
#include "Types.h"

#include <string>
#include <vector>
#include <GLEW/glew.h>

/*!
    * \enum BufferType
    * Is used to specify what type of buffer is being created. Eg. VBO, EBO etc.
*/
enum ATOM_API BufferType {
    VAO = 0, /*!< Specifies a Vertex Array Object. */
    VBO,     /*!< Specifies a Vertex Buffer Object. */
    EBO,      /*!< Specifies a Element Buffer Object. */
    FBO,    /*!< Specifies a Frame Buffer Object. */
};

/*!
    * \enum DrawType
    * Is Used to decide what type of memory the data should be stored inside the GPU.
*/
enum ATOM_API DrawType {
    STATIC = GL_STATIC_DRAW,    /*!< Specifies that the data should be stored in static memory. */
    DYNAMIC = GL_DYNAMIC_DRAW,  /*!< Specifies that the data should be stored in dynamic memory. */
};

/*!
    * \enum VariableType
    * Is used to specify what type of data the buffers will be using.
*/
enum ATOM_API VariableType {
    VT_BYTE = GL_BYTE,              /*!< Specifies that it uses a byte data type. */
    VT_UBYTE = GL_UNSIGNED_BYTE,    /*!< Specifies that it uses a unsigned byte data type. */
    VT_SHORT = GL_SHORT,            /*!< Specifies that it uses a short data type. */
    VT_USHORT = GL_UNSIGNED_SHORT,  /*!< Specifies that it uses a unsigned short data type. */
    VT_FLOAT = GL_FLOAT             /*!< Specifies that it uses a float data type. */
};

/*!
    * \enum BufferAttribute
    * Is used to specify what type of attribute you will be adding to a buffer. This way every buffer has
    * a standard layout for particular attribute types.
*/
enum ATOM_API BufferAttribute {
    POSITION = 0,   /*!< Specifies Position as Attribute 0. */
    NORMAL,         /*!< Specifies Normals as Attribute 1. */
    UV,             /*!< Specifies Texture Coords as Attribute 2. */
    TANGENT,        /*!< Specifies Tangents as Attribute 3. */
    BITANGENT       /*!< Specifies Bitangents as Attribute 4. */
};

/*!
    * \enum AttachmentType
    * Is used to specify the type of attachment to add when using Frame Buffer Objects.
*/
enum ATOM_API AttachmentType {
    TEXTURE = 0,    /*!< Specifies a texture attachment. */
    DEPTH,          /*!< Specifies a depth texture attachment. */
    STENCIL,        /*!< Specifies a stencil texture attachment. */
    DEPTH_STENCIL,  /*!< Specifies a depth and stencil attachment. */
};

class ATOM_API Buffer : public AlignedAllocation<BYTE16>
{
public:
    Buffer();
    ~Buffer();

    /*!
        * \brief Create a buffer of type specified.
        * \param type The buffer type you wish to create.
        *
        * Creates a buffer of the type chosen by the user using the buffer type enum.
    */
    void Create(BufferType type);

    /*!
        * \brief Binds the buffer to be the one currently being worked on.
        *
        * Binds the buffer to be the currently in use, for adding attribute pointers or filling
        * with data.
    */
    void Bind();

    /*!
        * \brief Unbinds the buffer s no longer the currently in use.
    */
    void Unbind();

    /*!
        * \brief Fills the buffer with data.
        * \param dataSize The size of the data in bytes.
        * \param data Pointer to the data being copied.
        * \param type The draw type used for memory, eg. dynamic or static.
        *
        * Fills the buffer with the data provided. Does not allow for the use of changing parts of the data
        * it will jsut overrite all data in the buffer.
    */
    void FillBuffer(int dataSize, const void* data, DrawType type);

    /*!
        * \brief Detroys the buffer.
        *
        * Wll destroy the buffer releasing all data associated with the buffer and then deletes
        * the buffer from the OpenGL context.
    */
    void Destroy();
    
    /*!
        * \brief Enables attribute pointers inside the GPU.
        * \param locationID Which index is used for the data inside the GPU program.
        * \param size The size of the data in bytes.
        * \param type What type of data is inside the buffer.
        * \param stride The stride from each vertex to the next in bytes.
        * \param offset How far from the beginning is this data inside the vertex in bytes.
    */
    void AddAttributePointer(unsigned int locationID, int size, VariableType type, int stride, int offset = 0);

    /*!
    * \brief Enables attribute pointers inside the GPU.
    * \param attribute Which index is used for the data inside the GPU program.
    * \param size The size of the data in bytes.
    * \param type What type of data is inside the buffer.
    * \param stride The stride from each vertex to the next in bytes.
    * \param offset How far from the beginning is this data inside the vertex in bytes.
    */
    void AddAttributePointer(BufferAttribute attribute, int size, VariableType type, int stride, int offset = 0);

    /*!
        * \brief Gets the buffer ID.
        * \return Returns the Buffer ID.
    */
    unsigned int GetID() const;

private:

    unsigned int m_ID;  /*!< The buffer ID. */
    BufferType m_type;  /*!< The type of buffer that it is. Eg. VAO, VBO etc. */

};

class ATOM_API RenderBuffer : public AlignedAllocation<BYTE16> {
public:
    RenderBuffer();
    ~RenderBuffer();

    void Create(AttachmentType type, glm::vec2 size);
    void Bind();
    void Unbind();
    void Destroy();

    unsigned int GetID();

private:
    unsigned int m_ID;
    glm::vec2 m_size;
};

class ATOM_API FrameBuffer : public AlignedAllocation<BYTE16> {
public:
    FrameBuffer();
    ~FrameBuffer();

    /*!
    * \brief Create a buffer of type specified.
    * \param type The buffer type you wish to create.
    *
    * Creates a buffer of the type chosen by the user using the buffer type enum.
    */
    void Create(glm::vec2 size);

    /*!
    * \brief Binds the buffer to be the one currently being worked on.
    *
    * Binds the buffer to be the currently in use, for adding attribute pointers or filling
    * with data.
    */
    void Bind();

    /*!
    * \brief Unbinds the buffer s no longer the currently in use.
    */
    void Unbind();

    /*!
        * \brief Detroys the buffer.
        *
        * Wll destroy the buffer releasing all data associated with the buffer and then deletes
        * the buffer from the OpenGL context.
    */
    void Destroy();

    /*!
        * \brief Gets the buffer ID.
        * \return Returns the Buffer ID.
    */
    unsigned int GetID() const;

    /*!
        * \brief Checks too see if the frame buffer is ready to use.
        * \return Whether or not the frame buffer is ready for use.
        *
        * This function will only work with FrameBuffer objects.
    */
    bool IsFrameBufferComplete();

    void AddAttachment(AttachmentType type);

    void AddRenderBuffer(AttachmentType type);

private:
    unsigned int m_ID;  /*!< The buffer ID. */

    glm::vec2 m_size; /*!< The size of the frame buffer. */

    std::vector<unsigned int> m_colorAttachmentIDs;
    unsigned int m_depthAttachmentID;
    unsigned int m_stencilAttachmentID;

    std::vector<RenderBuffer> m_renderBuffers;
};
