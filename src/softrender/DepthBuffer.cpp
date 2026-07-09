#include "DepthBuffer.h"

#include <cassert>
#include <memory>


DepthBuffer::DepthBuffer(int width, int height) {
	int depthBufferSize = sizeof(float) * width * height;
	float defaultDepth = 0.0f;

	assert(width > 0 && height > 0);

	mWidth = width;
	mHeight = height;
	mDepthBuffer = (float*)malloc(depthBufferSize);

	clear(defaultDepth);
}

DepthBuffer::~DepthBuffer()
{
	free(mDepthBuffer);
}


void DepthBuffer::setDepth(int x, int y, float depth) {
	int index = y * mWidth + x;
	mDepthBuffer[index] = depth;
}

float DepthBuffer::getDepth(int x, int y)
{
	int index = y * mWidth + x;
	return mDepthBuffer[index];
}

bool DepthBuffer::depthTest(int x, int y, float depth) const
{
    int index = y * mWidth + x;
	return mDepthBuffer[index] > depth;
}

bool DepthBuffer::depthTestAndWrite(int x, int y, float depth) const
{
    int index = y * mWidth + x;
	if (mDepthBuffer[index] < depth) {
		mDepthBuffer[index] = depth;
		return true;
	}
	return false;
}

void DepthBuffer::clear(float depth) {
	int numPixels = mWidth * mHeight;
	int i;
	for (i = 0; i < numPixels; i++) {
		mDepthBuffer[i] = depth;
	}
}