//
// display.hpp for OpenGL in /home/nicolaspolomack/demos/C++/OpenGL/include
//
// Made by nicolaspolomack
// Login   <nicolas.polomack@epitech.eu>
//
// Started on  Wed Sep 5 18:33:50 2017 nicolaspolomack
// Last update Tue Sep 25 00:00:31 2017 nicolaspolomack
//

#ifndef DISPLAY_HPP_
# define DISPLAY_HPP_

# include <iostream>

# include <SFML/Window.hpp>
# include <SFML/Graphics.hpp>

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

# include <GL/glew.h>
# include <GL/gl.h>
# include <GL/glext.h>

class Display {
public:
    Display(char *title, int _width, int _height);
    ~Display();
    void                update(glm::vec3 *pos, glm::vec3 *rot, GLuint *texture, glm::ivec2 *resolution);
    void                close();
    sf::RenderWindow    *getWindow();
    bool                isClosed();
    int                 getWidth();
    int                 getHeight();

protected:
    

private:
    sf::RenderWindow    *win;
    sf::Event           evt;
    int                 width;
    int                 height;
    bool                closed;

};

#endif /* !DISPLAY_HPP_ */
