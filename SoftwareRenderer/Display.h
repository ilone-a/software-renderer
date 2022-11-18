
//#
#pragma once

#include "SDL.h"
#include "RasterizerVertex.h"
#include "Renderer.h"
#include "windows.h"
#include "GeometryPrimitives.h"


//creates renderer windxw

class Display
{
public:

  const int screen_width = 800;
  const int screen_height = 600;


private:

  SDL_Window* main_window;

public:
  void createRenderer();


  void rectangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, Renderer renderer2)
  {

    for (int y = y1; y < y2; ++y) {
      for (int x = x1; x < x2; ++x)
      {

        renderer2.put_pixel_to_surface(x * 100 + screen_width / 2, y * 100 + screen_height / 2, 255, 0, 0);
      }
    }
  };
};

