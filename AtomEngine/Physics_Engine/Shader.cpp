#include "Shader.h"
#include "LogManager.h"

#include <fstream>
#include <sstream>
#include <GLEW/glew.h>

Shader::Shader(const std::string & fileName)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //ensure ifstream objects can throw exceptions.
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        //try to open the files.
        vShaderFile.open("Shaders/" + fileName + ".vert");
        fShaderFile.open("Shaders/" + fileName + ".frag");
        std::stringstream vShaderStream, fShaderStream;
        //read contents of file buffer into respective streams.
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //close files
        vShaderFile.close();
        fShaderFile.close();
        //convert stream into a string.
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        Logger::Instance()->LogError(fileName + ": Shader Files Not Successfully Read!");
    }
    Logger::Instance()->LogDebug(fileName + ": Shader Files Read Successfully!");

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    //vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    //check for problems
    if (!HasCompileErrors(vertex, "VERTEX")) {
        Logger::Instance()->LogDebug("Vertex Shader Compiled OK.");
    }

    //fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    //check for problems
    if (!HasCompileErrors(fragment, "FRAGMENT")) {
        Logger::Instance()->LogDebug("Fragment Shader Compiled OK.");
    }

    //shader program
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    //check for problems
    if (!HasCompileErrors(m_ID, "PROGRAM")) {
        Logger::Instance()->LogDebug("Shader Program Linked Successfully.");
    }

    //delete the shaders as they are no longer needed after linking.
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::Use()
{
    glUseProgram(m_ID);
}

void Shader::UpdateMatrices(const glm::mat4 & model, const glm::mat4 & view)
{
    SetMat4("model", model);
    SetMat4("view", view);
}

void Shader::UpdateMatrices(const glm::mat4 & model, const glm::mat4 & view, const glm::mat4 & projection)
{
    SetMat4("model", model);
    SetMat4("view", view);
    SetMat4("projection", projection);
}

void Shader::SetBool(const std::string & name, bool value)
{
    SetInt(name, (int)value);
}

void Shader::SetInt(const std::string & name, int value)
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string & name, float value)
{
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetVec2(const std::string & name, const glm::vec2 & value)
{
    glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const std::string & name, float x, float y)
{
    glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string & name, const glm::vec3 & value)
{
    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string & name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string & name, const glm::vec4 & value)
{
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string & name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string & name, const glm::mat2 & value)
{
    glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat3(const std::string & name, const glm::mat3 & value)
{
    glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat4(const std::string & name, const glm::mat4 & value)
{
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

unsigned int Shader::GetID() const
{
    return m_ID;
}

bool Shader::HasCompileErrors(unsigned int shaderID, const std::string & type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
            std::string logString(infoLog);
            Logger::Instance()->LogError("Shader Compilation Error: " + type + "\n" + logString);
            return true;
        }
    }
    else {
        glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
            std::string logString(infoLog);
            Logger::Instance()->LogError("Shader Link Error: " + type + "\n" + logString);
            return true;
        }
    }
    return false;
}
