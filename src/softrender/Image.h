#pragma once
#include "Color.h"

#include <fstream>

class Image {
protected:
    unsigned char* data;
    int width;
    int height;
    int bytespp;

public:
    enum Format {
        GRAYSCALE = 1, RGB = 3, RGBA = 4
    };

    Image();
    Image(int w, int h, int bpp);
    bool write_tga_file(const char* filename, bool rle = true);
    bool flip_horizontally();
    bool flip_vertically();
    bool scale(int w, int h);
    BColor get(int x, int y);
    bool set(int x, int y, BColor c);
    ~Image();
    Image& operator =(const Image& img);
    int get_width();
    int get_height();
    int get_bytespp();
    unsigned char* buffer();
    void clear();
};