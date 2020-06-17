#include "ShaderProgram.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <sstream>

ShaderProgram::~ShaderProgram() {
    if (_programId != 0) {
        glDeleteProgram(_programId);
    }
}


void ShaderProgram::createVertexShader(const char *const *source) {
    _vertexShaderId = createShader(source, ShaderType::VERTEX);
}

void ShaderProgram::createFragmentShader(const char *const *source) {
    _fragmentShaderId = createShader(source, ShaderType::FRAGMENT);
}

void ShaderProgram::linkProgram() {
    _programId = glCreateProgram();
    if (_programId == 0) {
        throw std::runtime_error("Could not create shader program.");
    }

    glAttachShader(_programId, _vertexShaderId);
    glAttachShader(_programId, _fragmentShaderId);
    glLinkProgram(_programId);

    // mark the shaders for deletion once the shader program is deleted
    glDeleteShader(_vertexShaderId);
    glDeleteShader(_fragmentShaderId);

    int success;
    char infoLog[1024];
    glGetProgramiv(_programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_programId, sizeof(infoLog), nullptr, infoLog);
        std::stringstream errMsg;
        errMsg << "Could not link shader program:\n" << infoLog;
        throw std::runtime_error(errMsg.str());
    }

    glValidateProgram(_programId);
    glGetProgramiv(_programId, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_programId, sizeof(infoLog), nullptr, infoLog);
        std::stringstream errMsg;
        errMsg << "Error while validating shader code:\n" << infoLog;
        throw std::runtime_error(errMsg.str());
    }
}

void ShaderProgram::bind() {
    glUseProgram(_programId);
}

void ShaderProgram::unbind() {
    glUseProgram(0);
}

unsigned int ShaderProgram::createShader(const char *const *source, ShaderType type) {
    auto openglShaderType = type == ShaderType::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
    unsigned int shaderId = glCreateShader(openglShaderType);
    glShaderSource(shaderId, 1, source, nullptr);
    glCompileShader(shaderId);

    int success;
    char infoLog[1024];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::stringstream errMsg;
        errMsg << "Could not compile " << (type == ShaderType::VERTEX ? "vertex" : "fragment") << " shader:\n"
            << infoLog;
        throw std::runtime_error(errMsg.str());
    }

    return shaderId;
}

