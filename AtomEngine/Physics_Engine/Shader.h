/*!
    * \class Shader "Shader.h"
    * \brief Handles the creation of shaders.
    *
    * Handles the creation of shader programs and the compilation of the,. It also facilitates
    * the passing of data to the GPU once the shader is created.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <string>
#include <GLM/glm.hpp>

#include "AlignedAllocation.h"
#include "Buffer.h"

class ATOM_API Shader : public AlignedAllocation<BYTE16>
{
public:
    /*!
        * \brief Creates the shader program.
        * \param fileName The name of the two shader files to compile.
        *
        * Will load both fragment and vertex files and compile them into a GPU shader program.
    */
    Shader(const std::string& fileName);
    ~Shader();

    /*!
        * \brief Makes this shader program be the one currently in use.
    */
    void Use();

    /*!
        * \brief Updates the model and view matrices.
        * \param model The model matrix.
        * \param view The view matrix.
        *
        * Takes the two matrices and updates the internal GPU program data to match the new values.
    */
    void UpdateMatrices(const glm::mat4& model, const glm::mat4& view);

    /*!
    * \brief Updates the model, view and projection matrices.
    * \param model The model matrix.
    * \param view The view matrix.
    * \param projection The projection matrix.
    *
    * Takes the three matrices and updates the internal GPU program data to match the new values.
    */
    void UpdateMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

    /*!
        * \brief Sets a boolean value inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param value The value to set the internal variable to.
    */
    void SetBool(const std::string& name, bool value);

    /*!
        * \brief Sets an integer value inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param value The value to set the internal variable to.
    */
    void SetInt(const std::string& name, int value);

    /*!
        * \brief Sets a floating point value inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param value The value to set the internal variable to.
    */
    void SetFloat(const std::string& name, float value);

    /*!
        * \brief Sets a 2 component vector value inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param value The value to set the internal variable to.
    */
    void SetVec2(const std::string& name, const glm::vec2& value);

    /*!
        * \brief Sets a 2 componentt vector value inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param x The x component of the vector.
        * \param y The y component of the vector.
    */
    void SetVec2(const std::string& name, float x, float y);

    /*!
        * \brief Sets a 3 component vector value inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param value The value to set the internal variable to.
    */
    void SetVec3(const std::string& name, const glm::vec3& value);

    /*!
        * \brief Sets a 3 component vector value inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param x The x component of the vectoor.
        * \param y The y component of the vectoor.
        * \param z The z component of the vectoor.
    */
    void SetVec3(const std::string& name, float x, float y, float z);

    /*!
        * \brief Sets a 4 component vector value inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param value The value to set the internal variable to.
    */
    void SetVec4(const std::string& name, const glm::vec4& value);

    /*!
        * \brief Sets a 4 component vector value inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param x The x component of the vector.
        * \param y The y component of the vector.
        * \param z The z component of the vector.
        * \param w The w component of the vector.
    */
    void SetVec4(const std::string& name, float x, float y, float z, float w);

    /*!
        * \brief Sets a 2x2 matrix inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param value The value to set the internal variable to.
    */
    void SetMat2(const std::string& name, const glm::mat2& value);

    /*!
        * \brief Sets a 3x3 matrix inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param value The value to set the internal variable to.
    */
    void SetMat3(const std::string& name, const glm::mat3& value);

    /*!
        * \brief Sets a 4x4 matrix inside the GPU program.
        * \param name The name of variable inside the GPU program.
        * \param value The value to set the internal variable to.
    */
    void SetMat4(const std::string& name, const glm::mat4& value);

    void SetKernel(const std::vector<float>& value);

    /*!
        * \brief Binds a UniformBuffer
        * \param name The name of the uniform block inside the shader.
        * \param bindingPoint Which binding point the uniform block uses.
        *
        * Will bind a uniform block to the shader at the specified binding point.
        * NOTE: The binding point for the uniform block and the shader must be the same.
    */
    void BindUniformBuffer(const std::string& name, UniformBufferBinding bindingPoint);

    /*!
        * \brief Gets the ID of the shader program.
    */
    unsigned int GetID() const;

private:
    
    /*!
        * \brief Checks to see if the shader had any compile errors.
        * \param shaderID The ID of the shader program.
        * \param type The type of shader your checking.
        * \return Wether there was a compile error or not.
        * 
        * Will check the current compile operation for errors and if there are errors, will return true.
        * It will also output any errors to the Engine Log.
    */
    bool HasCompileErrors(unsigned int shaderID, const std::string& type);

    unsigned int m_ID; /*!< The shader programs ID. */

};

