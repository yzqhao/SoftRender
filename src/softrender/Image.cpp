#include "Image.h"

#include <iostream>
#include <fstream>

Image::Image() : data(NULL), width(0), height(0), bytespp(0) {
}

Image::Image(int w, int h, int bpp) : data(NULL), width(w), height(h), bytespp(bpp) {
    unsigned long nbytes = width * height * bytespp;
    data = new unsigned char[nbytes];
    memset(data, 0, nbytes);
}

Image::~Image() {
    if (data) delete[] data;
}

bool Image::write_tga_file(const char* filename, bool rle) {
    
    return true;
}

BColor Image::get(int x, int y) {
    if (!data || x < 0 || y < 0 || x >= width || y >= height) {
        return BColor();
    }
    return BColor(data + (x + y * width) * bytespp, bytespp);
}

bool Image::set(int x, int y, BColor c) {
    if (!data || x < 0 || y < 0 || x >= width || y >= height) {
        return false;
    }
    memcpy(data + (x + y * width) * bytespp, c.bgra, bytespp);
    return true;
}

int Image::get_bytespp() {
    return bytespp;
}

int Image::get_width() {
    return width;
}

int Image::get_height() {
    return height;
}

bool Image::flip_horizontally() {
    if (!data) return false;
    int half = width >> 1;
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < height; j++) {
            BColor c1 = get(i, j);
            BColor c2 = get(width - 1 - i, j);
            set(i, j, c2);
            set(width - 1 - i, j, c1);
        }
    }
    return true;
}

bool Image::flip_vertically() {
    if (!data) return false;
    unsigned long bytes_per_line = width * bytespp;
    unsigned char* line = new unsigned char[bytes_per_line];
    int half = height >> 1;
    for (int j = 0; j < half; j++) {
        unsigned long l1 = j * bytes_per_line;
        unsigned long l2 = (height - 1 - j) * bytes_per_line;
        memmove((void*)line, (void*)(data + l1), bytes_per_line);
        memmove((void*)(data + l1), (void*)(data + l2), bytes_per_line);
        memmove((void*)(data + l2), (void*)line, bytes_per_line);
    }
    delete[] line;
    return true;
}

unsigned char* Image::buffer() {
    return data;
}

void Image::clear() {
    memset((void*)data, 0, width * height * bytespp);
}

bool Image::scale(int w, int h) {
    if (w <= 0 || h <= 0 || !data) return false;
    unsigned char* tdata = new unsigned char[w * h * bytespp];
    int nscanline = 0;
    int oscanline = 0;
    int erry = 0;
    unsigned long nlinebytes = w * bytespp;
    unsigned long olinebytes = width * bytespp;
    for (int j = 0; j < height; j++) {
        int errx = width - w;
        int nx = -bytespp;
        int ox = -bytespp;
        for (int i = 0; i < width; i++) {
            ox += bytespp;
            errx += w;
            while (errx >= (int)width) {
                errx -= width;
                nx += bytespp;
                memcpy(tdata + nscanline + nx, data + oscanline + ox, bytespp);
            }
        }
        erry += h;
        oscanline += olinebytes;
        while (erry >= (int)height) {
            if (erry >= (int)height << 1) // it means we jump over a scanline
                memcpy(tdata + nscanline + nlinebytes, tdata + nscanline, nlinebytes);
            erry -= height;
            nscanline += nlinebytes;
        }
    }
    delete[] data;
    data = tdata;
    width = w;
    height = h;
    return true;
}
