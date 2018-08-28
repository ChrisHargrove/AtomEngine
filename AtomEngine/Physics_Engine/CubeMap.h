/*!
    * \class CubeMap "CubeMap.h"
    * \brief A cubemap for use as a skybox, or other sampling uses.
    *
    * Loads a collection of 6 textures using SDL_Surface and then converts all 6 textures to 
    * an OpenGL cubemap texture. This can then be used to texture a skybox, or for other purposes
    * such as refraction and reflection purposes.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <SDL/SDL.h>

class ATOM_API CubeMap
{
public:
    CubeMap();

    /*!
        * \brief Creates and Loads a cubemap.
        * \param fileName The name of the files you are loading.
        *
        * Loads all image files using SDL2 API and then converts them into an OpenGL cubemap texture.
    */
    CubeMap(const std::string& fileName);
    ~CubeMap();

    /*!
        * \brief Gets the cubemap texture's ID created by OpenGL.
        * \return The ID of the cubemap texture.
    */
    unsigned int GetID();

    /*!
        * \brief Loads the texture files.
        * \param fileName The name of the files being loaded.
        * \return Wether or not the textures were loaded successfully or not.
        *
        * Loads the files using the SDL2 API and then converts them into an OpenGL cubemap texture.
    */
    bool Load(const std::string& fileName);

    /*!
        * \brief Loads the textures only as SDL_Surfaces.
        * \param fileName The name of the file to be loaded.
        * \return Wether the file was loaded successfully or not.
        *
        * Used for loading only the SDL_Surface, which can be converted to OpenGL textures at a later time.
        * This is primarily for use with multithreading, as Textures must be loaded on same thread of execution
        * that the OpenGL context was created on.
    */
    bool LoadSurfaces(const std::string& fileName);

    /*!
        * \brief Converts the cubemap to an OpenGL texture.
        * \return Wether the textures were converted successfully or not.
    */
    bool Convert();

private:
    unsigned int m_ID;                  /*!< The texture ID created by OpenGL. */

    std::vector<std::string> m_faces;   /*!< A list of face names for use during loading. */
    std::vector<SDL_Surface*> m_images; /*!< The SDL_Surfaces to store the initial images. */
};

