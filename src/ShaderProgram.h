#pragma once

#include <string_view>

class ShaderProgram {
private:
    unsigned int _vertexShaderId{};
    unsigned int _fragmentShaderId{};
    unsigned int _programId{};

public:
    ShaderProgram(const char *const *vertexShader, const char *const *fragmentShader);
    ~ShaderProgram();
    ShaderProgram(const ShaderProgram &other) = delete;
    ShaderProgram& operator=(const ShaderProgram &other) = delete;
    ShaderProgram(ShaderProgram &&other) noexcept = default;
    ShaderProgram& operator=(ShaderProgram &&other) noexcept = default;

    void use() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    enum class ShaderType {
        VERTEX,
        FRAGMENT
    };

    void createVertexShader(const char *const *source);
    void createFragmentShader(const char *const *source);

    void linkProgram();

    [[nodiscard]]
    static unsigned int createShader(const char *const *source, ShaderType type);

    [[nodiscard]]
    int getUniformLocation(const std::string &name) const;
};


