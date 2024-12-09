#include "Pixmap.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"


Pixmap::Pixmap()
{

    width = 0;
    height = 0;
    components = 0;
    pixels = nullptr;
}

Pixmap::~Pixmap()
{
    if (pixels)
    {
        free(pixels);
    }
}

Pixmap::Pixmap(const Pixmap &image, const IntRect &crop)
{
    width = crop.width;
    height = crop.height;
    components = image.components;
    pixels = (unsigned char *)malloc(width * height * components);
    for (int y = (int)crop.y, offsetSize = 0; y < (int)(crop.y + crop.height); y++)
    {
        memcpy(pixels + offsetSize, ((unsigned char *)image.pixels) + (y * image.width + (int)crop.x) * components, (int)crop.width * components);
        offsetSize += ((int)crop.width * components);
    }
}

Pixmap::Pixmap(int w, int h, int components)
{

    width = w;
    height = h;
    this->components = components;
    pixels = (unsigned char *)malloc(w * h * components);
}

Pixmap::Pixmap(int w, int h, int components, unsigned char *data)
{

    pixels = (unsigned char *)malloc(w * h * components);
    width = w;
    height = h;
    this->components = components;
    memcpy(pixels, data, w * h * components);
}

void Pixmap::Clear()
{
    if (pixels)
    {
        for (int i = 0; i < width * height * components; i++)
        {
            pixels[i] = 0;
        }
    }
}

void Pixmap::SetPixel(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a)
{

    if (x >= (u32)width || y >= (u32)height)
        return;

    if (components == 1)
    {
        Vec3 coln((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
        unsigned char gray = (unsigned char)((coln.x * 0.299f + coln.y * 0.587f + coln.z * 0.114f) * 255.0f);
        ((u8 *)pixels)[y * width + x] = gray;
    }
    else if (components == 2)
    {
        Vec3 coln((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
        u8 gray = (u8)((coln.x * 0.299f + coln.y * 0.587f + coln.z * 0.114f) * 255.0f);

        ((u8 *)pixels)[(y * width + x) * 2] = gray;
        ((u8 *)pixels)[(y * width + x) * 2 + 1] = a;
    }
    else if (components == 3)
    {

        ((u8 *)pixels)[(y * width + x) * 3] = r;
        ((u8 *)pixels)[(y * width + x) * 3 + 1] = g;
        ((u8 *)pixels)[(y * width + x) * 3 + 2] = b;
    }
    else if (components == 4)
    {
        ((u8 *)pixels)[(y * width + x) * 4] = r;
        ((u8 *)pixels)[(y * width + x) * 4 + 1] = g;
        ((u8 *)pixels)[(y * width + x) * 4 + 2] = b;
        ((u8 *)pixels)[(y * width + x) * 4 + 3] = a;
    }
}

void Pixmap::SetPixel(u32 x, u32 y, u32 rgba)
{

    if (x >= (u32)width || y >= (u32)height)
        return;

    u8 r = rgba;
    u8 g = rgba >> 8;
    u8 b = rgba >> 16;
    u8 a = rgba >> 24;
    SetPixel(x, y, r, g, b, a);
}

u32 Pixmap::GetPixel(u32 x, u32 y) const
{

    if (x >= (u32)width || y >= (u32)height)
        return 0;

    if (components == 1)
    {
        return pixels[y * width + x];
    }
    else if (components == 2)
    {
        return pixels[(y * width + x) * 2] | (pixels[(y * width + x) * 2 + 1] << 8);
    }
    else if (components == 3)
    {
        return pixels[(y * width + x) * 3] | (pixels[(y * width + x) * 3 + 1] << 8) | (pixels[(y * width + x) * 3 + 2] << 16);
    }
    else if (components == 4)
    {
        return pixels[(y * width + x) * 4] | (pixels[(y * width + x) * 4 + 1] << 8) | (pixels[(y * width + x) * 4 + 2] << 16) | (pixels[(y * width + x) * 4 + 3] << 24);
    }

    return 0;
}

Color Pixmap::GetPixelColor(u32 x, u32 y) const
{

    Color color = Color::BLACK;

    if ((x < (u32)width) && (y < (u32)height))
    {

        if (components == 1)
        {
            color.r = (u8)((u8 *)pixels)[y * width + x];
            color.g = (u8)((u8 *)pixels)[y * width + x];
            color.b = (u8)((u8 *)pixels)[y * width + x];
            color.a = 255;
        }
        else if (components == 2)
        {
            color.r = (u8)((u8 *)pixels)[(y * width + x) * 2];
            color.g = (u8)((u8 *)pixels)[(y * width + x) * 2];
            color.b = (u8)((u8 *)pixels)[(y * width + x) * 2];
            color.a = (u8)((u8 *)pixels)[(y * width + x) * 2 + 1];
        }
        else if (components == 3)
        {
            color.r = (u8)((u8 *)pixels)[(y * width + x) * 3];
            color.g = (u8)((u8 *)pixels)[(y * width + x) * 3 + 1];
            color.b = (u8)((u8 *)pixels)[(y * width + x) * 3 + 2];
            color.a = 255;
        }
        else if (components == 4)
        {
            color.r = (u8)((u8 *)pixels)[(y * width + x) * 4];
            color.g = (u8)((u8 *)pixels)[(y * width + x) * 4 + 1];
            color.b = (u8)((u8 *)pixels)[(y * width + x) * 4 + 2];
            color.a = (u8)((u8 *)pixels)[(y * width + x) * 4 + 3];
        }
    }
    return color;
}

void Pixmap::Fill(u8 r, u8 g, u8 b, u8 a)
{
    for (u32 y = 0; y < (u32)height; y++)
    {
        for (u32 x = 0; x < (u32)width; x++)
        {
            SetPixel(x, y, r, g, b, a);
        }
    }
}

void Pixmap::Fill(u32 rgba)
{
    for (u32 y = 0; y < (u32)height; y++)
    {
        for (u32 x = 0; x < (u32)width; x++)
        {
            SetPixel(x, y, rgba);
        }
    }
}

bool Pixmap::Load(const char *file_name)
{

    pixels = stbi_load(file_name, &width, &height, &components, 0);

    if (pixels == nullptr)
    {
        LogError("Failed to load image: %s", file_name);
        return false;
    }

    LogInfo( "PIXMAP: Load image: %s (%d,%d) bpp:%d", file_name, width, height, components);

    return true;
}

bool Pixmap::LoadFromMemory(const unsigned char *buffer, unsigned int bytesRead)
{

    pixels = stbi_load_from_memory(buffer, bytesRead, &width, &height, &components, 0);
    if (pixels == nullptr)
    {
        LogError("Failed to load image from memory");
        return false;
    }
    return true;
}

bool Pixmap::Save(const char *file_name)
{
    if (pixels == nullptr)
    {
        LogError("Failed to save image: %s", file_name);
        return false;
    }

    // if (System::Instance().IsFileExtension(file_name, ".bmp"))
    //     return stbi_write_bmp(file_name, width, height, components, pixels);
    // else if (System::Instance().IsFileExtension(file_name, ".tga"))
    //     return stbi_write_tga(file_name, width, height, components, pixels);
    // else if (System::Instance().IsFileExtension(file_name, ".png"))
    //     return stbi_write_png(file_name, width, height, components, pixels, 0);
    // else
    // {
    //     LogError("Failed to save image: %s ", file_name);
    //     return false;
    // }
    return false;
}

void Pixmap::FlipVertical()
{
    if (pixels == nullptr)
    {
        LogError("Failed to flip image");
        return;
    }
    int rowSize = width * components;
    unsigned char *row = (unsigned char *)malloc(rowSize);
    for (int y = 0; y < height / 2; y++)
    {
        unsigned char *src = ((unsigned char *)pixels) + y * rowSize;
        unsigned char *dst = ((unsigned char *)pixels) + (height - y - 1) * rowSize;
        memcpy(row, src, rowSize);
        memcpy(src, dst, rowSize);
        memcpy(dst, row, rowSize);
    }
    free(row);
}

void Pixmap::FlipHorizontal()
{
    if (pixels == nullptr)
    {
        LogError("Failed to flip image");
        return;
    }
    int rowSize = width * components;
    unsigned char *row = (unsigned char *)malloc(rowSize);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            unsigned char *src = ((unsigned char *)pixels) + y * rowSize + x * components;
            unsigned char *dst = ((unsigned char *)pixels) + y * rowSize + (width - x - 1) * components;
            memcpy(row, src, components);
            memcpy(src, dst, components);
            memcpy(dst, row, components);
        }
    }
    free(row);
}