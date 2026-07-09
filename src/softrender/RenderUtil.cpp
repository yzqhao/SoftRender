#include "RenderUtil.h"

namespace RenderUtil {
    void drawLine(int x0, int y0, int x1, int y1, Image& image, BColor color) {
        bool steep = false;
        if (std::abs(x0-x1)<std::abs(y0-y1)) { // if the line is steep, we transpose the image
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0>x1) { // make it left−to−right
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        for (int x=x0; x<=x1; x++) {
            float t = (x-x0)/(float)(x1-x0);
            int y = y0*(1.-t) + y1*t;
            if (steep) {
                image.set(y, x, color); // if transposed, de−transpose
            } else {
                image.set(x, y, color);
            }
        }
    }

    static Vector3f barycentric(Vector3f A, Vector3f B, Vector3f C, Vector3f P) {
        Vector3f s[2];
        for (int i = 2; i--; ) {
            s[i][0] = C[i] - A[i];
            s[i][1] = B[i] - A[i];
            s[i][2] = A[i] - P[i];
        }
        Vector3f u = cross(s[0], s[1]);
        if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
            return Vector3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
        return Vector3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
    }
 
    static void drawTriangle(Vector3f* pts, RenderData& rd, BColor color) {
        Image* pimg = rd.mImage;
        Vector2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
        Vector2f bboxmax(-std::numeric_limits<float>::max(),  -std::numeric_limits<float>::max());
        Vector2f clamp(pimg->get_width()-1, pimg->get_height()-1);
        for (int i=0; i<3; i++) {
            bboxmin.x = std::max(0.0f, std::min(bboxmin.x, pts[i].x));
            bboxmin.y = std::max(0.0f, std::min(bboxmin.y, pts[i].y));

            bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, pts[i].x));
            bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, pts[i].y));
        }
        Vector3f P;
        for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
            for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
                Vector3f bc_screen  = barycentric(pts[0], pts[1], pts[2], P);
                if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
                P.z = 0;
                for (int i = 0; i < 3; i++) P.z += pts[i][2] * bc_screen[i];
                if (rd.mDepthBuffer->depthTestAndWrite(P.x, P.y, P.z)) {
                    pimg->set(P.x, P.y, color);
                }
            }
        }
    }

    static Vector3f world2screen(Vector3f v, int width, int height) {
        return Vector3f(int((v.x+1.)*width/2.+.5), int((v.y+1.)*height/2.+.5), v.z);
    }

    void draw(RenderData& rd, BColor color) {
        auto model = rd.mModel;
        const int width = rd.mImage->get_width();
        const int height = rd.mImage->get_height();
        const Vector3f light_dir(0,0,-1);
        for (int i = 0; i < model->nfaces(); i++) {
            std::vector<int> face = model->face(i);
            Vector3f pts[3];
            for (int i = 0; i < 3; i++) pts[i] = world2screen(model->vert(face[i]), width, height);
            int col = rand() % 255;
            drawTriangle(pts, rd, BColor(col, col, col, 255));
        }
    }
}