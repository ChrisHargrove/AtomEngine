/*!
    * \class ScreenManager "ScreenManager.h"
    * \brief Manager for handling everything to do with the screen.
    *
    * Handles the creation of the screen and initialization of the screen.
    * Also has all the utility functions to manage everything regarding the screen.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <string>
#include <SDL/SDL.h>
#include <GLEW/glew.h>
#include <GLM/glm.hpp>

#include "Singleton.h"
#include "AlignedAllocation.h"

/*!
    * \enum ClearBits
    * Is used to specify which buffer bits to clear.
*/
enum class ATOM_API ClearBits {
    COLOR = GL_COLOR_BUFFER_BIT,        /*!< Specifies the color buffer bit. */
    DEPTH = GL_DEPTH_BUFFER_BIT,        /*!< Specifies the depth buffer bit. */
    STENCIL = GL_STENCIL_BUFFER_BIT,    /*!< Specifies the stencil buffer bit. */
    ACCUM = GL_ACCUM_BUFFER_BIT         /*!< Specifies the accumlation buffer bit. */
};

class ATOM_API ScreenManager : public AlignedAllocation<BYTE16>
{
public:
    friend class Singleton<ScreenManager>;

    /*!
        * \brief Initializes the screen with title and size specified.
        *
        * \param title The title of the window created.
        * \param size The size you wish the window to be.
        * \param core Whether to run OpenGL in core mode or not, Default is true.
        * \return Whether the screen manager is succesfully initialized.
        *
        * Creates an SDL window with an OpenGL context, that defaults to core mode, but can be
        * switched modes. Title and size supplied change the initial window to have that title and size.
    */
    bool Initialize(std::string title, glm::vec2 size, bool core = true);

    /*!
        * \brief Enables V-Sync
        *
        * \param enable Whether to enable V-Sync or not. Defaults to true.
        * \return Whether the window managed to turn on V-Sync.
    */
    bool EnableVSync(bool enable = true);

    /*!
        * \brief Closes the window and frees all memory.
        *
        * Clears all associated memory of the window and releases all OpenGL context data.
    */
    void Close();

    /*!
        * \brief Clears the window.
        *
        * Clears the application window to the clear colour previously set. When no bits are set clears all buffers.
    */
    void Clear();

    /*!
        * \brief Clears the window.
        * \param clearBit Which buffer bits you wish to clear.
        *
        * Clears the application window to the clear colour previously set.
    */
    void Clear(ClearBits clearBits);

    /*!
        * \brief Swaps the buffers for the window.
        *
        * Will swap the windows buffers which then renders everything to the screen.
    */
    void Swap();

    /*!
        * \brief Sets the clear color of the screen.
        *
        * \param red The Red component of the colour.
        * \param green The Green component of the colour.
        * \param blue The Blue component of the colour.
        * \param alpha The transparency of the colour.
        *
        * Sets the clear colour of the window that is drawn every time the window clears.
        * Colour is set using 4 float values.
    */
    void SetClearColour(float red, float green, float blue, float alpha);

    /*!
        * \brief Sets the clear color of the screen.
        *
        * \param colour The colour to set the clear to.
        *
        * Sets the clear colour of the window that is drawn every time the window clears.
        * Colour is set using a 4 component vector, which is RGBA respectively. Defaults to Black.
    */
    void SetClearColour(const glm::vec4& colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    /*!
        * \brief Sets the OpenGL version to use.
        *
        * \param major The Major version of OpenGL to use.
        * \param minor The Minor version of OpenGL to use.
        *
        * Sets the OpenGL version to use with the window. Defaults to OpenGL 3.3.
    */
    void SetVersion(unsigned int major = 3, unsigned int minor = 3);

    /*!
        * \brief Gets a pointer to the SDL Window.
        * \return Pointer to the SDL Window structure.
    */
    SDL_Window* GetWindow();

    /*!
        * \brief Gets a pointer to the SDL GL Context.
        * \return Pointer to the SDL GL Context structure.
    */
    SDL_GLContext* GetContext();

    /*!
        * \brief Gets the size of the window.
        * \return The size of the window as a 2 component vector.
    */
    glm::vec2& GetSize();

    /*!
        * \brief Sets the mouse to be confined to the window.
        *
        * \param capture Whether the mouse should be captured or not. Defaults to true.
        * \return Whether the mouse was captured successfully or not.
        *
        * Sets the mouse to be capture by the window, which also causes all mouse movement to now be relative.
    */
    bool CaptureMouse(bool capture = true);

    /*!
        * \brief Sets the projection matrix.
        *
        * \param enable3D Whether the projection matrix should be 3D or not. Defaults to true.
        *
        * Sets the projection matrix to either Perspective or Orthographic.
    */
    void Enable3D(bool enable3D = true);

    /*!
        * \brief Toggles Depth Testing
        * \param enable Whether or not Depth testing should be enabled.
    */
    void EnableDepthTesting(bool enable = true);

    /*!
        * \brief Sets the initial values for Projection matrices.
        *
        * \param fieldOfView The angle of view in degrees. Default is 90 degrees.
        * \param zNear Where the Near Plane is. Default is 0.1f.
        * \param zFar Where the Far Plane is. Default is 100.0f.
    */
    void InitializeProjection(float fieldOfView = 90.0f, float zNear = 0.1f, float zFar = 100.0f);

    /*!
        * \brief Gets the projection matrix.
        * \return The current projection matrix.
        *
        * Will return the current projection matrix in use whether that be Perspective or Orthographic.
    */
    glm::mat4& GetProjection();

    /*!
        * \brief Creates a viewport to render to.
        *
        * When no parameters are specified Viewport will default to screen size.
    */
    void CreateViewport();

    /*!
        * \brief Creates a viewport to render to.
        *
        * \param w How wide the viewport is on the screen.
        * \param h How tall the viewport is on the screen.
    */
    void CreateViewport(float w, float h);

    /*!
        * \brief Creates a viewport to render to.
        *
        * \param x Where the viewport starts on the screen horizontally.
        * \param y Where the viewport starts on the screen vertically.
        * \param w How wide the viewport is on the screen.
        * \param h How tall the viewport is on the screen.
    */
    void CreateViewport(float x, float y, float w, float h);

    /*!
        * \brief Creates a viewport to render to.
        *
        * \param dimensions The position and size of the viewport.
    */
    void CreateViewport(const glm::vec4& dimensions);


private:
    bool m_isVersionSet = false; /*!< Has the OpenGL version been set. */
    bool m_is3DEnabled = false; /*!< Is the window 3D enabled. */

    float m_fieldOfView; /*!< The viewing angle in degrees. */

    SDL_Window * m_window; /*!< A pointer to the SDL Window. */
    SDL_GLContext m_context; /*!< A pointer to the OpenGL Context. */

    glm::ivec2 m_glVersion; /*!< The OpenGL version used by the window. */
    glm::vec2 m_size; /*!< The size of the window. */
    glm::vec2 m_clipPlanes; /*!< The Near and Far Clipping Plane values. */
    glm::mat4 m_projection; /*!< The window projection. */

private:
    ScreenManager() {};
    ScreenManager(const ScreenManager&);
   // ScreenManager& operator=(const ScreenManager&) {};

};

template ATOM_API ScreenManager* Singleton<ScreenManager>::Instance();

typedef Singleton<ScreenManager> Screen;