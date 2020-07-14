#pragma once

#include <string>
#include <glm/fwd.hpp>

class Shader {
private:
    unsigned int _vertexShaderId{};
    unsigned int _fragmentShaderId{};
    unsigned int _programId{};

public:
    Shader(const char *vertexShader, const char *fragmentShader);
    ~Shader();
    Shader(const Shader &other) = delete;
    Shader& operator=(const Shader &other) = delete;
    Shader(Shader &&other) noexcept = default;
    Shader& operator=(Shader &&other) noexcept = default;

    void use() const;

    void set(const std::string &name, bool value) const;
    void set(const std::string &name, int value) const;
    void set(const std::string &name, float value) const;
    void set(const std::string &name, const glm::mat4 &value) const;

private:
    enum class ShaderType {
        VERTEX,
        FRAGMENT
    };

    static unsigned int createShader(const char *const *source, ShaderType type);

    void linkProgram();

    int getUniformLocation(const std::string &name) const;
};


