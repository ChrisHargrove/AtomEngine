#include "ScreenManager.h"
#include "LogManager.h"
#include "GLM/gtc/matrix_transform.hpp"



bool ScreenManager::Initialize(std::string title, glm::vec2 size, bool core)
{
    m_size = size;
    
#ifdef DEBUG
    Logger::Instance()->LogDebug("Initializing ScreenManager...");
#endif

    //Attempt to initialize all of SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        std::string temp = SDL_GetError();
        Logger::Instance()->LogError("SDL_Init() Failed: " + temp);
        return false;
    }

    //If no OpenGL version set return a failure.
    if (!m_isVersionSet) {
        Logger::Instance()->LogError("SetVersion() Not Called! - Cannot Initialize Window without OpenGL version.");
        return false;
    }

    //Set OpenGL Version.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_glVersion.x);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_glVersion.y);

    //Set SDL_GL Attributes.
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    //Set SDL Double Buffer.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (core) {
        //Set OpenGL Core mode.
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef DEBUG
        Logger::Instance()->LogDebug("OpenGL Set to Core Profile");
#endif
    }
    else {
        //Set OpenGL Comapatability mode.
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
#ifdef DEBUG
        Logger::Instance()->LogDebug("OpenGL Set to Compatability Profile");
#endif
    }


    //Create SDL Window
    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)m_size.x, (int)m_size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    if (m_window == 0) {
        std::string temp = SDL_GetError();
        Logger::Instance()->LogError("SDL_CreateWindow() Failed: " + temp);
        return false;
    }
#ifdef DEBUG
    Logger::Instance()->LogDebug("Window Created: " + std::to_string(m_size.x) + "x" + std::to_string(m_size.y));
#endif
    
    //Create OpenGL Context
    m_context = SDL_GL_CreateContext(m_window);
    if (m_context == NULL) {
        std::string temp = SDL_GetError();
        Logger::Instance()->LogError("SDL_GL_CreateContext() Failed: " + temp);
        return false;
    }

    //Enable V-Sync
    EnableVSync();

    //Allow Depth Testing
    glEnable(GL_DEPTH_TEST);

    //Initialize GLEW
    GLenum glew_error = glewInit();
    if (GLEW_OK != glew_error) {
        std::string temp;
        temp = (char*)glewGetErrorString(glew_error);
        Logger::Instance()->LogError("glewInit() Failed:" + temp);
        return false;
    }

    //Enable Alpha Blending.
    glEnable(GL_BLEND);
    //Set Alpha Blending Function.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Create Entire window viewport to render to.
    CreateViewport(0, 0, m_size.x, m_size.y);
    //Set default clear colour. - Default is Black.
    SetClearColour(0.0f, 0.0f, 0.0f, 1.0f);

    //By default enable 3D rendering
    Enable3D(true);

    //No failures so return successful.
    return true;
}

bool ScreenManager::EnableVSync(bool enable)
{
    if (enable) {
        if (SDL_GL_SetSwapInterval(1) == 0) {
            Logger::Instance()->LogInfo("VSync Enabled.");
            return true;
        }
    }
    else {
        if (SDL_GL_SetSwapInterval(0) == 0) {
            Logger::Instance()->LogInfo("VSync Disabled.");
            return true;
        }
    }
    Logger::Instance()->LogWarning("EnableVSync() - Swap Interval Not Supported.");
    return false;
}

void ScreenManager::Close()
{
#ifdef DEBUG
    Logger::Instance()->LogDebug("Closing ScreenManager...");
#endif
    SDL_GL_DeleteContext(m_context);	// Free memory to the context
    SDL_DestroyWindow(m_window);		// Free memory to the window
    SDL_Quit();						// Close all SDL subsystems
}

void ScreenManager::Clear()
{
    //Clear the colour buffer and depth buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void ScreenManager::Clear(ClearBits clearBits)
{
    glClear((GLbitfield)clearBits);
}

void ScreenManager::Swap()
{
    //Swap the buffers being rendered to.
    SDL_GL_SwapWindow(m_window);
}

void ScreenManager::SetClearColour(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
}

void ScreenManager::SetClearColour(const glm::vec4 & colour)
{
    glClearColor(colour.r, colour.g, colour.b, colour.a);
}

void ScreenManager::SetVersion(unsigned int major, unsigned int minor)
{
    m_glVersion.x = major;
    m_glVersion.y = minor;
    m_isVersionSet = true;
    Logger::Instance()->LogInfo("OpenGL Version Set To: " + std::to_string(major) + "." + std::to_string(minor));
}

SDL_Window * ScreenManager::GetWindow()
{
    return m_window;
}

SDL_GLContext* ScreenManager::GetContext()
{
    return &m_context;
}

glm::vec2& ScreenManager::GetSize()
{
    return m_size;
}

bool ScreenManager::CaptureMouse(bool capture)
{
    if (capture) {
        if (SDL_SetRelativeMouseMode(SDL_TRUE) != -1) {
#ifdef DEBUG
            Logger::Instance()->LogDebug("Grabbing Mouse and hiding Cursor...");
#endif
            return true;
        }
    }
    else {
        if (SDL_SetRelativeMouseMode(SDL_FALSE) != -1) {
#ifdef DEBUG
            Logger::Instance()->LogDebug("Releasing Mouse and showing Cursor...");
#endif
            return true;
        }
    }
    Logger::Instance()->LogWarning("Relative Mouse Mode Not Supported!");
    return false;
}

void ScreenManager::Enable3D(bool enable3D)
{
    m_is3DEnabled = enable3D;
    if (enable3D) {
        glEnable(GL_DEPTH_TEST);
        m_projection = glm::perspective(glm::radians(m_fieldOfView), m_size.x / m_size.y, m_clipPlanes.x, m_clipPlanes.y);
    }
    else {
        glDisable(GL_DEPTH_TEST);
        m_projection = glm::ortho(0.0f, m_size.x, 0.0f, m_size.y, m_clipPlanes.x, m_clipPlanes.y);
    }
}

void ScreenManager::EnableDepthTesting(bool enable)
{
    if (enable) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
}

void ScreenManager::InitializeProjection(float fieldOfView, float zNear, float zFar)
{
    m_fieldOfView = fieldOfView;
    m_clipPlanes.x = zNear;
    m_clipPlanes.y = zFar;
}

glm::mat4 & ScreenManager::GetProjection()
{
    return m_projection;
}

void ScreenManager::CreateViewport()
{
    glViewport(0, 0, (GLsizei)m_size.x, (GLsizei)m_size.y);
}

void ScreenManager::CreateViewport(float w, float h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void ScreenManager::CreateViewport(float x, float y, float w, float h)
{
    glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
}

void ScreenManager::CreateViewport(const glm::vec4 & dimensions)
{
    glViewport((GLint)dimensions.x, (GLint)dimensions.y, (GLsizei)dimensions.z, (GLsizei)dimensions.w);
}
