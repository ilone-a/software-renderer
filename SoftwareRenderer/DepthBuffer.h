#pragma once
#include "SDL.h"

class DepthBuffer
{

public: DepthBuffer(int width, int height);

      void clearzbuffer();
      void put_pixel_buffer(int row, int col, double depth);
      bool closer(int row, int col, double depth);

private:
    //
    float* zbuffer;
    int size;
    int width;
    float maxvalue;
};

