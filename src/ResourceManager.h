#pragma once

#include <string>
#include <unordered_map>

#include "Shader.h"
#include "Texture.h"

class ResourceManager {
private:
    std::unordered_map<std::string, Shader> _shaders{};
    std::unordered_map<std::string, Texture> _textures{};
    std::string _resourceRoot;

public:
    ResourceManager(const std::string &resourceRoot);

    const Shader &loadShader(const std::string &name, const std::string &vertexShader, const std::string &fragmentShader);
    const Shader &getShader(const std::string &name);

    const Texture &loadTexture(const std::string &name, const std::string &relativeFilePath, bool hasAlpha);
    const Texture &getTexture(const std::string &name);
};