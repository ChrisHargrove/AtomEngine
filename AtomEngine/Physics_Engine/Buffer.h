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

/*!
    * \enum UniformBufferBinding
    * Is used to specify where the Uniform and shader bind together.
*/
enum ATOM_API UniformBufferBinding {
    MATRICES = 0,   /*!< Specifies that the Matrice UniformBlock binds to point 0. */
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

/*!
    * \class RenderBuffer "Buffer.h"
    * \brief Used as an attachment to a framebuffer.
    *
    * Handles the creation and management of a render buffer object foor use as a
    * framebuffer attachment.
*/
class ATOM_API RenderBuffer : public AlignedAllocation<BYTE16> {
public:
    RenderBuffer();
    ~RenderBuffer();

    /*!
        * \brief Creates a RenderBuffer
        * \param type The type oof render buffer you wish to create.
        * \param size The size of the render buffer, usually the same size as the framebuffer.
        *
        * Will create a render buffer with a given size and type. The type being similar to a
        * texture buffer attachment type.
    */
    void Create(AttachmentType type, glm::vec2 size);

    /*!
        * \brief Binds the RenderBuffer.
        *
        * Binds the buffer so associated calls to Renderbuffer storage are dont to the active render buffer.
    */
    void Bind();

    /*!
        * \brief Unbinds the RenderBuffer so it is no longer active.
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
    unsigned int GetID();

private:
    unsigned int m_ID;  /*!< The RenderBuffer ID. */
    glm::vec2 m_size;   /*!< The size of the RenderBuffer. */
};

/*!
    * \class FrameBuffer "Buffer.h"
    * \brief Creates a new FrameBuffer for rendering to that isnt the screen.
    *
    * Handles the creation and management oof the FrameBuffer, also handles the addition of
    * frame buffer attachments.
*/
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
        * \brief Gets the texture ID at the specified index.
        * \param index The index from which you wish to recieve the texture ID.
        * \return Returns the texture ID. Returns 0 on error.
        *
        * Gets a texture ID at the specified index, if the index is larger than total number of texture attachments
        * outputs an error and returns a 0. Which will unbind a texture.
    */
    GLuint GetTexture(unsigned int index);

    /*!
        * \brief Binds a texture from the specified index.
        * \param index The index at which you wish to bind the texture ID.
        *
        * Attempts to bind a texture at the specified index, if the index is greater than the number of texture attachments
        * it will output an error, and unbind the active texture unit.
        * If no index is specified it will just unbind the texture unit.
    */
    void BindTexture(int index = -1);

    /*!
        * \brief Unbinds the buffer so no longer the currently in use.
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

    /*!
        * \brief Adds a frame buffer attachment to the FrameBuffer
        * \param type The type of attachment you wish to add.
        * 
        * Adds a specified attachment to the FrameBuffer for later use.
    */
    void AddAttachment(AttachmentType type);

    /*!
        * \brief Adds a render buffer to the FrameBuffer.
        * \param type The type of RenderBuffer you wish to attach.
        *
        * Creates a new RenderBuffer of the given type and then attaches it to the FrameBuffer.
    */
    void AddRenderBuffer(AttachmentType type);

private:
    unsigned int m_ID;  /*!< The buffer ID. */

    glm::vec2 m_size; /*!< The size of the frame buffer. */

    std::vector<unsigned int> m_colorAttachmentIDs; /*!< List of color attachment ID's. */
    unsigned int m_depthAttachmentID;               /*!< The depth texture ID if one is attached. */
    unsigned int m_stencilAttachmentID;             /*!< The stencil texture ID if one is attached. */

    std::vector<RenderBuffer*> m_renderBuffers;      /*!< List of RenderBuffer attachments. */
};

class ATOM_API UniformBuffer {
public:

    void Create(int size);
    void Destroy();

    void BindBuffer(UniformBufferBinding bindingPoint);
    void SetData(void* data, unsigned int size);

    unsigned int GetID();

private:
    unsigned int m_ID; /*!< The buffer ID. */
};
