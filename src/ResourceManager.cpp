#include "ResourceManager.h"
#include <stdexcept>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace fs = std::filesystem;

namespace Shaders {
    extern const char *vertex;
    extern const char *fragment;
}

ResourceManager::ResourceManager(const std::string &resourceRoot) : _resourceRoot(resourceRoot) {
}

const Shader &ResourceManager::loadShader(const std::string &name,
    const std::string &vertexShader, const std::string &fragmentShader) {
    if (vertexShader != "vertex") {
        throw std::runtime_error("Unknown vertex shader '" + vertexShader + "'");
    }
    if (fragmentShader != "fragment") {
        throw std::runtime_error("Unknown fragment shader '" + fragmentShader + "'");
    }
    Shader shader(Shaders::vertex, Shaders::fragment);

    auto [pair, wasInserted] = _shaders.emplace(name, std::move(shader));
    if (!wasInserted) {
        throw std::runtime_error("Could not load shader '" + name + "': A shader with this name already exists.");
    }
    return pair->second;
}

const Shader &ResourceManager::getShader(const std::string &name) {
    return _shaders.find(name)->second;
}

const Texture &ResourceManager::loadTexture(
    const std::string &name, const std::string &relativeFilePath, bool hasAlpha) {
    auto fullFilePath = fs::path(_resourceRoot).append(relativeFilePath).string();
    int width, height, channels;
    std::uint8_t *data = stbi_load(fullFilePath.c_str(), &width, &height, &channels, 0);

    Texture texture(data, width, height, hasAlpha);
    stbi_image_free(data);

    auto [pair, wasInserted] = _textures.emplace(name, std::move(texture));
    if (!wasInserted) {
        throw std::runtime_error("Could not load texture '" + name + "': A texture with this name already exists.");
    }
    return pair->second;
}

const Texture &ResourceManager::getTexture(const std::string &name) {
    return _textures.find(name)->second;
}

