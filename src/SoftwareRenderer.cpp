
//#include "../include/Renderer.h"
//#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


using namespace sf;

int main()
{
    sf::VideoMode mode({ 800, 600 });
    RenderWindow window(sf::VideoMode({ 512, 512 }), "window", sf::Style::Resize | sf::Style::Close);
    //sf::RenderWindow window(mode, "Software Renderer");
    //RenderWindow window(VideoMode({ 200, 200 }), "SFML works!");   //RenderWindow window()
    //sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "SFML works!");
  //  sf::RenderWindow window(sf::VideoMode({ 800u, 600u }), "SFML Raw Mouse Input", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setVerticalSyncEnabled(true);

    CircleShape shape(100.f, 3);
    shape.setPosition({100.0f, 100.0f});
    shape.setFillColor(Color::Magenta);

    while (window.isOpen())
    {
        //Event event();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

          //  window.clear(Color::Blue);
          //  window.draw(shape);
         //   window.display();
        }
    }
   return 0;
}

