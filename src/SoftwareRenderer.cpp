
#include "../include/Renderer.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

//main loop

using namespace sf;


int main() {
    Renderer renderer(800, 600); 
    renderer.run();
    return 0;
}