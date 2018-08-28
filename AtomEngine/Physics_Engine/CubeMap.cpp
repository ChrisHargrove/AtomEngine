#include "CubeMap.h"
#include "Utilities.h"
#include "LogManager.h"

#include <GLEW/glew.h>
#include <SDL\SDL_image.h>

CubeMap::CubeMap()
{
    m_faces.reserve(6); //Reserve The memory for the 6 face strings.
    m_faces.push_back("Right");
    m_faces.push_back("Left");
    m_faces.push_back("Top");
    m_faces.push_back("Bottom");
    m_faces.push_back("Front");
    m_faces.push_back("Back");
}

CubeMap::CubeMap(const std::string & fileName)
{
    m_faces.reserve(6); //Reserve The memory for the 6 face strings.
    m_faces.push_back("Right");
    m_faces.push_back("Left");
    m_faces.push_back("Top");
    m_faces.push_back("Bottom");
    m_faces.push_back("Front");
    m_faces.push_back("Back");
    Load(fileName);
}


CubeMap::~CubeMap()
{
    glDeleteTextures(1, &m_ID);
}

unsigned int CubeMap::GetID()
{
    return m_ID;
}

bool CubeMap::Load(const std::string & fileName)
{
    //Create the texture.
    glGenTextures(1, &m_ID);
    //Bind Texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    std::string name = SplitFront(fileName, ".");
    std::string extension = SplitEnd(fileName, ".", true);
    std::string filePath = "Assets/Textures/CubeMaps/" + name;

    SDL_Surface* image;
    for (int i = 0; i < (int)m_faces.size(); i++) {
        if (image = IMG_Load((filePath + "_" + m_faces[i] + extension).c_str())) {
            //Check to see if image was loaded.
            if (!image) {
                Logger::Instance()->LogError("Image: " + filePath + "_" + m_faces[i] + extension + " Failed to load!");
                glDeleteTextures(1, &m_ID); //If image failed to load delete OpenGL texture.
                return false;
            }
            //Find image format.
            GLenum format;
            if (image->format->Amask > 0) {
                format = GL_RGBA;
            }
            else {
                format = GL_RGB;
            }
            //Generate texture
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);

        }
        if (image) { //After image has been converted then free the memory for the image so it can be reused.
            SDL_FreeSurface(image);
        }
    }
    //Set up linear filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Set up texture wrap parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    Logger::Instance()->LogInfo("CubeMap: " + name + " Loaded Successfully!");

    return true;
}

bool CubeMap::LoadSurfaces(const std::string & fileName)
{
    std::string name = SplitFront(fileName, ".");
    std::string extension = SplitEnd(fileName, ".", true);
    std::string filePath = "Assets/Textures/CubeMaps/" + name;

    SDL_Surface* image;

    for (int i = 0; i < (int)m_faces.size(); i++) {
        if (image = IMG_Load((filePath + "_" + m_faces[i] + extension).c_str())) {
            //Check to see if image was loaded.
            if (!image) {
                Logger::Instance()->LogError("Image: " + filePath + "_" + m_faces[i] + extension + " Failed to load!");
                return false;
            }
            Logger::Instance()->LogInfo("Texture: " + fileName + "_" + m_faces[i] + extension + " Loaded Successfully!");
        }
        m_images.push_back(image);
    }
    Logger::Instance()->LogInfo("CubeMap: " + fileName + " Loaded Successfully!");
    return true;
}

bool CubeMap::Convert()
{
    //Create the texture.
    glGenTextures(1, &m_ID);
    //Bind Texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    for (int i = 0; i < m_images.size(); i++) {
        if (m_images[i]) {
            //Find image format.
            GLenum format;
            if (m_images[i]->format->Amask > 0) {
                format = GL_RGBA;
            }
            else {
                format = GL_RGB;
            }

            //Generate texture
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, m_images[i]->w, m_images[i]->h, 0, format, GL_UNSIGNED_BYTE, m_images[i]->pixels);

        }
        if (m_images[i]) { //After image has been converted then free the memory for the image so it can be reused.
            SDL_FreeSurface(m_images[i]);
        }
    }

    //Set up linear filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Set up texture wrap parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Logger::Instance()->LogInfo("CubeMap: Loaded Successfully!");
    return true;
}
