#include "DepthBuffer.h"
#include "SDL.h"
#include <limits>

DepthBuffer::DepthBuffer(int width, int height)
{
    this->size = width * height;
    this->width = width;
    this->zbuffer = new float[this->size];
    this->maxvalue = std::numeric_limits<double>::max();

    //fill buffer
    for (int i = 0; i < this->size; i++) {
        this->zbuffer[i] = this->maxvalue;
    }
}

void DepthBuffer::clearzbuffer()
{
    
        for (int i = 0; i < this->size; i++) {
            this->zbuffer[i] = this->maxvalue;
        }
    
}

void DepthBuffer::put_pixel_buffer(int x, int y, double depth)
{
    int depth_index = (y * this->width) + x;
    if (depth_index > this->size) { return;  }
    this->zbuffer[depth_index] = depth;
}



bool DepthBuffer::closer(int x, int y, double depth)
{
	//return false;
    int depth_index = (y * this->width) + x;
    if (depth_index > this->size) { return false; }

    bool result;
    result = false;

    if (this->zbuffer[depth_index] > depth)  { result = true; }

    return result;
}
