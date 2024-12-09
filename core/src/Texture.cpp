#define GL_GLEXT_PROTOTYPES
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include "Texture.hpp"
#include "Device.hpp"
#include "stb_image.h"




Texture::Texture() : HorizontalWrap(WrapMode::Repeat),
                     VerticalWrap(WrapMode::Repeat),
                     MinificationFilter(FilterMode::NearestMipLinear),
                     MagnificationFilter(FilterMode::Linear),
                     MaxAnisotropic(0.0f)
{

    id = 0;
    width = 0;
    height = 0;
}

Texture::~Texture()
{
    if (Texture2D::defaultTexture != 0x0)
    {
        Texture2D::defaultTexture->Release();
        Texture2D::defaultTexture = 0x0;
    }
    Release();
}

void Texture::SetMinFilter(FilterMode filter)
{
    this->MinificationFilter = filter;
    if (id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinificationFilter);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::SetMagFilter(FilterMode filter)
{
    this->MagnificationFilter = filter;
    if (id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagnificationFilter);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::SetWrapS(WrapMode mode)
{
    this->HorizontalWrap = mode;
    if (id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, HorizontalWrap);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::SetWrapT(WrapMode mode)
{
    this->VerticalWrap = mode;
    if (id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, VerticalWrap);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::SetAnisotropicFiltering(float level)
{
    this->MaxAnisotropic = level;
    if (id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, MaxAnisotropic);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::Release()
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
        LogInfo("Texture: [ID %i] Release", id);
        id = 0;
    }
}

void Texture::createTexture()
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
    }
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, HorizontalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, VerticalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinificationFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagnificationFilter);
    if (MaxAnisotropic > 0.0f)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, MaxAnisotropic);
    }
}

void Texture::Use(u32 unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}
void Texture::Update(const Pixmap &pixmap)
{
    if (pixmap.pixels)
    {
        u32 components = pixmap.components;
        u32 width = pixmap.width;
        u32 height = pixmap.height;

        GLenum format = GL_RGBA;
        GLenum glFormat = GL_RGBA;

        switch (components)
        {
        case 1:
        {
            format = GL_R8;
            glFormat = GL_RED;
            break;
        }
        case 2:
        {
            format = GL_RG8;
            glFormat = GL_RG;
            break;
        }
        case 3:
        {
            format = GL_RGB;
            glFormat = GL_RGB;

            break;
        }
        case 4:
        {
            format = GL_RGBA8;
            glFormat = GL_RGBA;
            break;
        }
        }

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE, pixmap.pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
void Texture::Update(const unsigned char *buffer, u16 components, int width, int height)
{
    if (buffer)
    {
        GLenum format = GL_RGBA;
        GLenum glFormat = GL_RGBA;
        switch (components)
        {
        case 1:
        {
            format = GL_R8;
            glFormat = GL_RED;
            break;
        }
        case 2:
        {
            format = GL_RG8;
            glFormat = GL_RG;
            break;
        }
        case 3:
        {
            format = GL_RGB;
            glFormat = GL_RGB;

            break;
        }
        case 4:
        {
            format = GL_RGBA8;
            glFormat = GL_RGBA;
            break;
        }
        }
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE, buffer);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

Texture2D *Texture2D::defaultTexture = 0x0;

Texture2D *Texture2D::GetDefaultTexture()
{
    if (defaultTexture == 0x0)
    {
        defaultTexture = new Texture2D();
        defaultTexture->width = 1;
        defaultTexture->height = 1;
        defaultTexture->components = 4;
        unsigned char data[4] = {255, 255, 255, 255};
        defaultTexture->LoadFromMemory(data, 4, 1, 1);
    }
    return defaultTexture;
}

Texture2D::Texture2D() : Texture()
{
}

Texture2D::Texture2D(const Pixmap &pixmap) : Texture()
{

    components = pixmap.components;
    width = pixmap.width;
    height = pixmap.height;

    GLenum format = GL_RGBA;
    GLenum glFormat = GL_RGBA;
    switch (components)
    {
    case 1:
    {
        format = GL_R8;
        glFormat = GL_RED;
        break;
    }
    case 2:
    {
        format = GL_RG8;
        glFormat = GL_RG;
        break;
    }
    case 3:
    {
        format = GL_RGB;
        glFormat = GL_RGB;

        break;
    }
    case 4:
    {
        format = GL_RGBA8;
        glFormat = GL_RGBA;
        break;
    }
    }

    createTexture();

    if (pixmap.pixels)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE, pixmap.pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        if (components == 1)
        {
            GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
        }
        else if (components == 2)
        {
            GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
        }
    }

    //  glBindTexture(GL_TEXTURE_2D, 0);
    //   Log(0, "TEXTURE2D: [ID %i] Create Opengl Texture2D (%d,%d) bpp:%d", id, width, height, components);
}

bool Texture2D::Load(const Pixmap &pixmap)
{
    components = pixmap.components;
    width = pixmap.width;
    height = pixmap.height;

    GLenum format = GL_RGBA;
    GLenum glFormat = GL_RGBA;
    switch (components)
    {
    case 1:
    {
        format = GL_R8;
        glFormat = GL_RED;
        break;
    }
    case 2:
    {
        format = GL_RG8;
        glFormat = GL_RG;
        break;
    }
    case 3:
    {
        format = GL_RGB;
        glFormat = GL_RGB;

        break;
    }
    case 4:
    {
        format = GL_RGBA8;
        glFormat = GL_RGBA;
        break;
    }
    }

    createTexture();

    if (pixmap.pixels)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE, pixmap.pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        if (components == 1)
        {
            GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
        }
        else if (components == 2)
        {
            GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    // Log(0, "TEXTURE2D: [ID %i] Create Opengl Texture2D (%d,%d) bpp:%d", id, width, height, components);
    return true;
}

bool Texture2D::Load(const char *file_name)
{

    unsigned int bytesRead;
    unsigned char *fileData = LoadDataFile(file_name, &bytesRead);
    if (!fileData)
        return false;

    unsigned char *data = stbi_load_from_memory(fileData, bytesRead, &width, &height, &components, 0);

    if (data == NULL)
    {
        LogError("Texture2D: Failed to load image: %s", file_name);
        return false;
    }

    GLenum format = GL_RGBA;
    GLenum glFormat = GL_RGBA;
    switch (components)
    {
    case 1:
    {
        format = GL_R8;
        glFormat = GL_RED;
        break;
    }
    case 2:
    {
        format = GL_RG;
        glFormat = GL_RG;
        break;
    }
    case 3:
    {
        format = GL_RGB;
        glFormat = GL_RGB;

        break;
    }
    case 4:
    {
        format = GL_RGBA8;
        glFormat = GL_RGBA;
        break;
    }
    }

    createTexture();

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (components == 1)
    {
        GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    }
    else if (components == 2)
    {
        GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    }

    //   glBindTexture(GL_TEXTURE_2D, 0);
    //   Log(0, "TEXTURE2D: [ID %i] Create Opengl Texture2D (%d,%d) bpp:%d", id, width, height, components);

    free(fileData);
    free(data);
    return true;
}

Texture2D::Texture2D(const char *file_name) : Texture()
{
    Load(file_name);
}

bool Texture2D::LoadFromMemory(const unsigned char *buffer, u16 components, int width, int height)
{

    this->width = width;
    this->height = height;
    this->components = components;

    GLenum format = GL_RGBA;
    switch (components)
    {
    case 1:
        format = GL_RED;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;

        return false;
    }

    createTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);

    glGenerateMipmap(GL_TEXTURE_2D);
    if (components == 1)
    {
        GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    }
    else if (components == 2)
    {
        GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    }

    return true;
}
