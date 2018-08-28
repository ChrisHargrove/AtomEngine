/*!
    * \class ShaderManager "ShaderManager.h"
    * \brief Manages all shader in the engine during execution.
    *
    * Allows for the adddition of new shaders to the engine, and handles their destruction
    * once finished with. Also manages when and how the shaders will be used.
*/

#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "Shader.h"
#include "Singleton.h"
#include "AlignedAllocation.h"

#include <map>

class ATOM_API ShaderManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<ShaderManager>;

    /*!
        * \brief Will shutdown the Shader Manager.
        *
        * Will delete all shaders in the coontainer and clear up all memory associated with them.
    */
    void Shutdown();

    /*!
        * \brief Adds a shader to the manager.
        * \param name The name of the shader, to be used as a Key.
        * \param fileName The file name of shader for loading purposes.
        * \return Whether the shader was added to manager or not.
        *
        * Will create a shader on the heap and then load the shader program. After this
        * the pointer to the shader will be stored in the shader manager until program
        * execution has finished.
    */
    bool AddShader(const std::string& name,const std::string& fileName);

    /*!
        * \brief Makes the shader specified currently in use.
        * \param name The name of the shader, which was chosen upon storage.
        * \return Wether or not the program is now in use.
        *
        * Will make the shader that is specified the one currently being used to render.
    */
    bool UseShader(const std::string& name);

    /*!
        * \brief Gets a pointer to the shader specified.
        * \param name The name of the shader, which was chosen upon storage.
        * \return Returns a pointer to the shader specified. Will return nullptr if shader does
        * not exist.
        *
        * Searches the shader container to see if the shader specified exists, if not will return nullptr. If
        * shader does exist it will return a pointer to it.
    */
    Shader* GetShader(const std::string& name);

    /*!
        * \brief Gets a pointer to the shader container.
        * \return Returns a pointer to the shader container.
    */
    std::map<std::string, Shader*>* GetShaderList();

private:
    std::string m_currentShader; /*!< The shader currently in use. Defaults to empty. */
    std::map<std::string, Shader*> m_shaderList; /*!< The container for all shaders. */

    ShaderManager() {};
    ShaderManager(const ShaderManager&);
    //ShaderManager& operator=(const ShaderManager&) {}
    
};

template ATOM_API ShaderManager* Singleton<ShaderManager>::Instance();

typedef Singleton<ShaderManager> Shaders;

