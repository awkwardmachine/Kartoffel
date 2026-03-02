//
// Created by nil on 02/03/2026.
//

#ifndef KARTOFFEL_TEXTURE_HPP
#define KARTOFFEL_TEXTURE_HPP

#include <string>

enum class TextureFilter {
    Nearest,
    Linear,
    NearestMipmap,
    LinearMipmap,
};

class Texture {
public:
    Texture();
    ~Texture();

    bool        Load(const std::string &path, TextureFilter filter = TextureFilter::LinearMipmap);
    void        Bind(unsigned int slot = 0) const;
    static void Unbind();
    void        Cleanup();

    [[nodiscard]] unsigned int GetId() const { return id_; }
    [[nodiscard]] bool         IsLoaded() const { return loaded_; }

private:
    unsigned int id_;
    bool         loaded_;

    static void ApplyFilter(TextureFilter filter);
};

#endif // KARTOFFEL_TEXTURE_HPP
