
#include "Display.h"
#include "Renderer.h"
#include "SDL.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "Matrix.h"
#include "bunny.cpp"
#include "VertexShader.h"




class Renderer;
class VertexShader;

void Display::createRenderer()
{

  //create sdl2 objects

  SDL_Init(SDL_INIT_VIDEO);
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_Log("Error: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  main_window = SDL_CreateWindow("software renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_ALLOW_HIGHDPI);
  if (main_window == NULL)
  {
    SDL_Log("Window error: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }


  SDL_Surface* screen_surface = SDL_GetWindowSurface(main_window);
  SDL_Event event;
  Renderer this_renderer(main_window, screen_width, screen_height);

  VertexShader vertex_shader(screen_width, screen_height);

  RasterizerVertex < Vector4 > Vertex0[3];

  bool still_running = true;


  int frameCount = 0;
  DWORD ticks = GetTickCount();

  while (still_running == true)
  {

    while (SDL_PollEvent(&event))
    {

      if (event.type == SDL_QUIT)
        still_running = false;
    }
    this_renderer.clear();
    //SDL_UpdateWindowSurface(main_window);

    SDL_LockSurface(screen_surface);

    using namespace std;

    const BUNNY_POINT* bunnyVtx = NULL;
    const int* bunnyIdx = NULL;
    int nv = getBunnyPoints(bunnyVtx);
    int ni = getBunnyFaces(bunnyIdx);


    RasterizerVertex < Vector4 > Vertex0[3];

    Matrix m2s(kTrivial);
    vertex_shader.modelToScreen(m2s);

    // for (int i = 0; i < 4; i += 4)
    for (int i = 0; i < ni; i += 4)
    {
      const int* vi = bunnyIdx + i + 1; // skip hardcoded 3
      Vertex0[0] = { {bunnyVtx[vi[0]].x, bunnyVtx[vi[0]].y, bunnyVtx[vi[0]].z ,1}, { 1,0,0,1 } };
      Vertex0[1] = { {bunnyVtx[vi[1]].x, bunnyVtx[vi[1]].y , bunnyVtx[vi[1]].z ,1}, { 1,0,0,1 } };
      Vertex0[2] = { {bunnyVtx[vi[2]].x, bunnyVtx[vi[2]].y , bunnyVtx[vi[2]].z ,1}, { 1,0,0,1 } };

      vertex_shader.transform_triangle(Vertex0, m2s);
      //add lighting
     // this_renderer.intensity =  vertex_shader.calculateLightIntencity(Vertex0, { 0.0f, 0.0f, 0.7f, 0.0f });  
      this_renderer.render_triangle(Vertex0);

    }

    // this_renderer.put_pixel_to_surface();
     //  this_renderer.render_triangle(screen_surface, Vertex0, matrix);
    SDL_UnlockSurface(screen_surface);
    SDL_UpdateWindowSurface(main_window);


    //fps
    ++frameCount;
    DWORD t = GetTickCount() - ticks;
    if (t > 5000)
    {
      int nf = (int)((double)(frameCount * t) / 5000.0);
      printf("%d frames over 5 sec (%.2f per second)\n", nf, 0.2 * nf);
      ticks = GetTickCount();
      frameCount = 0;
    }
  }
  this_renderer.clear();
  SDL_DestroyWindow(main_window);
  SDL_Quit();


}


