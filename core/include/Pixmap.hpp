#pragma once

#include "Config.hpp"
#include "Color.hpp"
#include "Math.hpp"

class Pixmap
{
public:
    Pixmap();
    ~Pixmap();
    Pixmap(int w, int h, int components);
    Pixmap(int w, int h, int components, unsigned char *data);

    Pixmap(const Pixmap &image, const IntRect &crop);
    Pixmap(const Pixmap &other) = delete;
    Pixmap &operator=(const Pixmap &other) = delete;
    Pixmap(Pixmap &&) = delete;

    void SetPixel(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a);
    void SetPixel(u32 x, u32 y, u32 rgba);

    u32 GetPixel(u32 x, u32 y) const;
    Color GetPixelColor(u32 x, u32 y) const;

    void Fill(u8 r, u8 g, u8 b, u8 a);
    void Fill(u32 rgba);

    bool Save(const char *file_name);

    void Clear();

    bool Load(const char *file_name);
    bool LoadFromMemory(const unsigned char *buffer, unsigned int bytesRead);

    void FlipVertical();
    void FlipHorizontal();

    unsigned char *pixels;
    int components;
    int width;
    int height;
};
