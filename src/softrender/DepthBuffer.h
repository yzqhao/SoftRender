#pragma once

class DepthBuffer {
public:
	DepthBuffer(int width, int height);
	~DepthBuffer();

	void setDepth(int x, int y, float depth);
	float getDepth(int x, int y);
    bool depthTest(int x, int y, float depth) const;
    bool depthTestAndWrite(int x, int y, float depth) const;

    void clear(float depth);
private:
    int mWidth, mHeight;
    float* mDepthBuffer;
};