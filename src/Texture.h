#pragma once

#include <cstdint>

class Texture {
private:
    unsigned int _id;
    unsigned int _width;
    unsigned int _height;

public:
    Texture(std::uint8_t *data, unsigned int width, unsigned int height, bool hasAlpha);
    ~Texture();
    Texture(const Texture &other) = delete;
    Texture &operator=(const Texture &other) = delete;
    Texture(Texture &&other) noexcept = default;
    Texture &operator=(Texture &&other) noexcept = default;

    unsigned int width() const;
    unsigned int height() const;

    void bind() const;
};


