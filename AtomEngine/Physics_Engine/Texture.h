/*!
    * \class Texture "Texture.h"
    * \brief an image for use by OpenGL
    * 
    * Loads an Image from a file as an SDL_Surface using the SDL2 API. It then goes on to
    * convert this to an OpenGL texture and the releases the data from the SDL_Surface.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <string>
#include <SDL/SDL_image.h>
#include <GLM/glm.hpp>

class ATOM_API Texture
{
public:
    Texture();

    /*!
        * \brief Creates and Loads texture.
        * \param fileName The name of the file you are loading.
        *
        * Loads the file using SDL2 API and then converts it into an OpenGL texture.
    */
    Texture(const std::string& fileName);
    ~Texture();

    /*!
        * \brief Gets the texture's ID created by OpenGL.
        * \return The ID of the texture.
    */
    unsigned int GetID();

    /*!
        * \brief Gets the size of the texture.
        * \return Returns the size as a 2 component vector.
    */
    glm::vec2& GetSize();

    /*!
        * \brief Loads the texture.
        * \param fileName The name of the file being loaded.
        * \return Whether the file loaded successfully or not.
        *
        * Loads the file using SDL and then converts it into an OpenGL texture.
    */
    bool Load(const std::string& fileName);

    /*!
        * \brief Loads the texture only as SDL_Surface.
        * \param fileName The name of the file to be loaded.
        * \return Wether the file was loaded successfully or not.
        *
        * Used for loading only the SDL_Surface, which can be converted to OpenGL textures at a later time. 
        * This is primarily for use with multithreading, as Textures must be loaded on same thread of execution
        * that the OpenGL context was created on.
    */
    bool LoadSurface(const std::string& fileName);

    /*!
        * \brief Converts the texture to an OpenGL texture.
        * \return Wether the texture was converted successfully or not.
    */
    bool Convert();

    /*!
        * \brief Unloads the data and destroys the OpenGL texture.
        * \return Whether the texture was unloaded successfully or not.
    */
    bool Unload();

private:
    unsigned int m_ID;  /*!< The texture ID created by OpenGL. */
    glm::vec2 m_size;   /*!< The size of the texture image. */

    SDL_Surface* m_image; /*!< The SDL_Surface to store the initial image. */
};

