#include "Texture.h"
#include "LogManager.h"

#include <GLEW/glew.h>



Texture::Texture()
{
}


Texture::Texture(const std::string & fileName)
{
    Load(fileName);
}

Texture::~Texture()
{
    Unload();
}

unsigned int Texture::GetID()
{
    return m_ID;
}

glm::vec2 & Texture::GetSize()
{
    return m_size;
}

bool Texture::Load(const std::string & fileName)
{
    std::string filePath = "Assets/Textures/" + fileName;
    if ((m_image = IMG_Load(filePath.c_str()))) {
        if (!m_image) { // Check to see if file was loaded.
            Logger::Instance()->LogError("Failed to Load: " + filePath);
            return false;
        }
        m_size = glm::vec2(m_image->w, m_image->h);

        //Check image format.
        GLenum format;
        if (m_image->format->Amask > 0) {
            format = GL_RGBA;
        }
        else {
            format = GL_RGB;
        }

        //Create OpenGL texture
        glGenTextures(1, &m_ID);
        //Bind texture
        glBindTexture(GL_TEXTURE_2D, m_ID);
        //Set texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //Set Linear Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //Generate Texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, (GLsizei)m_size.x, (GLsizei)m_size.y, 0, format, GL_UNSIGNED_BYTE, m_image->pixels);
        //Generate MipMap
        glGenerateMipmap(GL_TEXTURE_2D);

        //Once OpenGL Texture has been generated and filled with data free
        //the SDL surface as it is no longer needed.
        if (m_image) {
            SDL_FreeSurface(m_image);
        }

        Logger::Instance()->LogInfo("Texture: " + fileName + " Loaded Successfully!");
        return true;
    }
    Logger::Instance()->LogError("Texture: " + fileName + "Failed To Load!");
    return false;
}

bool Texture::LoadSurface(const std::string & fileName)
{
    std::string filePath = "Assets/Textures/" + fileName;
    if ((m_image = IMG_Load(filePath.c_str()))) {
        if (!m_image) { // Check to see if file was loaded.
            Logger::Instance()->LogError("Failed to Load: " + filePath);
            return false;
        }
        m_size = glm::vec2(m_image->w, m_image->h);
        Logger::Instance()->LogInfo("Texture: " + fileName + " Loaded Successfully!");
        return true;
    }
    Logger::Instance()->LogError("Texture: " + fileName + "Failed To Load!");
    return false;
}

bool Texture::Convert()
{
    if (m_image) {
        //Check image format.
        GLenum format;
        if (m_image->format->Amask > 0) {
            format = GL_RGBA;
        }
        else {
            format = GL_RGB;
        }

        //Create OpenGL texture
        glGenTextures(1, &m_ID);
        //Bind texture
        glBindTexture(GL_TEXTURE_2D, m_ID);
        //Set texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //Set Linear Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //Generate Texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, (GLsizei)m_size.x, (GLsizei)m_size.y, 0, format, GL_UNSIGNED_BYTE, m_image->pixels);
        //Generate MipMap
        glGenerateMipmap(GL_TEXTURE_2D);

        //Once OpenGL Texture has been generated and filled with data free
        //the SDL surface as it is no longer needed.
        if (m_image) {
            SDL_FreeSurface(m_image);
        }

        return true;
    }
    Logger::Instance()->LogError("Texture: Image Couldn't be converted!");
    return false;
}

bool Texture::Unload()
{
    glDeleteTextures(1, &m_ID);
    return true;
}
