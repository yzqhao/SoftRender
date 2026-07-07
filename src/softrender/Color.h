#pragma once

struct BColor;

class Color
{
public:
    float r, g, b, a;
    Color();
    Color(float R, float G, float B, float A = 255);
    Color(const BColor& tga_color);

    static float MAX_CHANNEL_VALUE;
    static Color White;
    static Color Red;
    static Color Green;
    static Color Blue;
    static Color Black;

    float& operator[](const size_t i);

    float operator[](const size_t i) const;

    Color operator+(const Color& color) const;

    Color operator*(float intensity) const;

    Color operator*(const Color& color) const;
};

struct BColor {
    unsigned char bgra[4];
    unsigned char bytespp;

    BColor() : bgra(), bytespp(1) {
        for (int i = 0; i < 4; i++) bgra[i] = 0;
    }

    BColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255) : bgra(), bytespp(4) {
        bgra[0] = B;
        bgra[1] = G;
        bgra[2] = R;
        bgra[3] = A;
    }

    BColor(unsigned char v) : bgra(), bytespp(1) {
        for (int i = 0; i < 4; i++) bgra[i] = 0;
        bgra[0] = v;
    }


    BColor(const unsigned char* p, unsigned char bpp) : bgra(), bytespp(bpp) {
        for (int i = 0; i < (int)bpp; i++) {
            bgra[i] = p[i];
        }
        for (int i = bpp; i < 4; i++) {
            bgra[i] = 0;
        }
    }

    unsigned char& operator[](const int i) { return bgra[i]; }
};