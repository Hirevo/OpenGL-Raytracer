//
// display.cpp for OpenGL in /home/nicolaspolomack/demos/C++/OpenGL
//
// Made by nicolaspolomack
// Login   <nicolas.polomack@epitech.eu>
//
// Started on  Wed Sep 5 18:33:19 2017 nicolaspolomack
// Last update Tue Sep 25 00:02:34 2017 nicolaspolomack
//

#include "init.hpp"
#include "display.hpp"

Display::Display(char *title, int _width, int _height) {
    sf::VideoMode mode;
    sf::ContextSettings settings;
    GLenum __glew_status;

    width = _width;
    height = _height;
    mode.bitsPerPixel = 32;
    mode.width = width;
    mode.height = height;
    settings.majorVersion = 4;
    settings.minorVersion = 5;
    settings.attributeFlags = sf::ContextSettings::Core;
    settings.depthBits = 24;
    win = new sf::RenderWindow(mode, title, sf::Style::Close | sf::Style::Fullscreen, settings);
    if (win == 0)
      throw "Window Creation Error !";
    closed = !win;
    if ((__glew_status = glewInit()) != GLEW_OK) {
        std::cerr << "GLEW: " << glewGetErrorString(__glew_status) << std::endl;
        throw "GLEWError";
    }
    win->setVerticalSyncEnabled(true);
}

Display::~Display() {
    win->close();
}

void Display::update(glm::vec3 *pos, glm::vec3 *rot, GLuint *texture, glm::ivec2 *resolution) {
    if (!win || closed) return ;
    win->display();
    while (win->pollEvent(evt)) {
        if (evt.type == sf::Event::Closed || (evt.type == sf::Event::KeyPressed && evt.key.code == sf::Keyboard::Escape))
            this->close();
        else if (evt.type == sf::Event::KeyPressed)
            switch (evt.key.code) {
                case sf::Keyboard::Z:
                    pos->z += 1;
                    break;
                case sf::Keyboard::S:
                    pos->z -= 1;
                    break;
                case sf::Keyboard::Q:
                    pos->x += 1;
                    break;
                case sf::Keyboard::D:
                    pos->x -= 1;
                    break;
                case sf::Keyboard::A:
                    pos->y += 1;
                    break;
                case sf::Keyboard::W:
                    pos->y -= 1;
                    break;
                case sf::Keyboard::Left:
                    rot->y += 1;
                    break;
                case sf::Keyboard::Right:
                    rot->y -= 1;
                    break;
                case sf::Keyboard::Up:
                    resolution->x += 16;
                    resolution->y += 9;
                    glDeleteTextures(1, texture);
                    initTexture(texture, resolution->x, resolution->y);
                    break;
                case sf::Keyboard::Down:
                    resolution->x -= 16;
                    resolution->y -= 9;
                    glDeleteTextures(1, texture);
                    initTexture(texture, resolution->x, resolution->y);
                    break;
            }
    }
}

sf::RenderWindow *Display::getWindow() {
    return win;
}

void Display::close() {
    if (!win) return ;
    closed = true;
    win->close();
}

bool Display::isClosed() {
    return closed;
}

int Display::getWidth() {
    return width;
}

int Display::getHeight() {
    return height;
}