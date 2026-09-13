#pragma once

#include <filesystem>
#include <glad/glad.h> // include glad to get all the required OpenGL headers
  
#include <string>


struct Shader
{
    // constructor reads and builds the shader
    Shader();
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&) noexcept = default;
    Shader& operator=(Shader&&) noexcept = default;

    // assemble the shader
    void initShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

    // use/activate the shader
    void use() const;
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;

    // getters
    [[nodiscard]] GLint getUniformLocation(const std::string& name) const;

    void deleteShader();

    [[nodiscard]] unsigned int getProgramID() const {
        return ID;
    }

private:
    unsigned int ID;
    bool isLoaded;
};



// uniform location lekérdezése a paraméterben megadott programon
inline GLint ul(const GLuint programID, const GLchar* uniformName ) noexcept
{
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetUniformLocation.xhtml
    return glGetUniformLocation( programID, uniformName );
}
// uniform location lekérdezése az aktív programon
inline GLint ul(const GLchar* uniformName) noexcept
{
    GLint prog; glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    if (prog == 0) {
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 1, GL_DEBUG_SEVERITY_HIGH, -1, "Trying to get uniform location but no shader is active.");
        return -1;
    }
    return ul(prog, uniformName);
}