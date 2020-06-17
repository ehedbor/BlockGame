#pragma once

#include <string_view>

class ShaderProgram {
private:
    unsigned int _vertexShaderId{};
    unsigned int _fragmentShaderId{};
    unsigned int _programId{};

public:
    ShaderProgram() = default;
    ~ShaderProgram();

    void createVertexShader(const char *const *source);
    void createFragmentShader(const char *const *source);

    void linkProgram();

    void bind();
    void unbind();

private:
    enum class ShaderType {
        VERTEX,
        FRAGMENT
    };

    [[nodiscard]]
    static unsigned int createShader(const char *const *source, ShaderType type);
};


