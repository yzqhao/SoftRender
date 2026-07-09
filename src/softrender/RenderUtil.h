#pragma once

#include "Image.h"
#include "Vector.h"
#include "DepthBuffer.h"
#include "ModelObj.h"

struct RenderData {
    Model* mModel;
    Image* mImage;
    DepthBuffer* mDepthBuffer;
};

namespace RenderUtil {
    void drawLine(int x0, int y0, int x1, int y1, Image& image, BColor color);
    void draw(RenderData& rd, BColor color);
}