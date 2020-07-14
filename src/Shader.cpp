#include "Shader.h"
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std::string_literals;

Shader::Shader(const char *vertexShader, const char *fragmentShader) {
    _vertexShaderId = createShader(&vertexShader, ShaderType::VERTEX);
    _fragmentShaderId = createShader(&fragmentShader, ShaderType::FRAGMENT);
    linkProgram();
}

Shader::~Shader() {
    // checking for _programId == 0 (etc) is useless as glDeleteProgram/Shader does it already
    glDeleteShader(_vertexShaderId);
    glDeleteShader(_fragmentShaderId);
    glDeleteProgram(_programId);
}

void Shader::linkProgram() {
    _programId = glCreateProgram();
    if (_programId == 0) {
        throw std::runtime_error("Could not create shader program.");
    }

    glAttachShader(_programId, _vertexShaderId);
    glAttachShader(_programId, _fragmentShaderId);
    glLinkProgram(_programId);

    int success;
    char infoLog[1024];
    glGetProgramiv(_programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_programId, sizeof(infoLog), nullptr, infoLog);
        throw std::runtime_error("Could not link shader program:\n    "s + infoLog);
    }

    glValidateProgram(_programId);
    glGetProgramiv(_programId, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_programId, sizeof(infoLog), nullptr, infoLog);
        throw std::runtime_error("Error while validating shader code:\n    "s + infoLog);
    }
}

unsigned int Shader::createShader(const char *const *source, ShaderType type) {
    auto openglShaderType = (type == ShaderType::VERTEX) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
    unsigned int shaderId = glCreateShader(openglShaderType);
    glShaderSource(shaderId, 1, source, nullptr);
    glCompileShader(shaderId);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, sizeof(infoLog), nullptr, infoLog);
        std::string shaderName = (type == ShaderType::VERTEX ? "vertex" : "fragment");
        throw std::runtime_error("Could not compile " + shaderName + " shader:\n    " + infoLog);
    }

    return shaderId;
}

void Shader::use() const {
    glUseProgram(_programId);
}

void Shader::set(const std::string &name, bool value) const {
    glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

void Shader::set(const std::string &name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::set(const std::string &name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::set(const std::string &name, const glm::mat4 &value) const {
    auto location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

int Shader::getUniformLocation(const std::string &name) const {
    auto location = glGetUniformLocation(_programId, name.c_str());
    if (location < 0) {
        throw std::runtime_error("Could not find uniform '" + name + "'");
    }
    return location;
}
