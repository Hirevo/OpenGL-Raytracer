//
// init.hpp for OpenGL in /home/nicolaspolomack/demos/C++/OpenGL/compute/include
//
// Made by nicolaspolomack
// Login   <nicolas.polomack@epitech.eu>
//
// Started on  Sat Sep 15 12:51:16 2017 nicolaspolomack
// Last update Sat Sep 15 12:51:16 2017 nicolaspolomack
//

#ifndef INIT_HPP_
# define INIT_HPP_

# include <sstream>
# include <GL/glew.h>
# include <GL/gl.h>
# include <GL/glext.h>
# include "scene.hpp"

void    initGL();
void    initBuffers(GLuint *vao, GLuint *buffer);
void    initTexture(GLuint *texture, int width, int height);
void    initScene(Scene **scene, char *file);
void    setSceneUniforms(GLuint computeProgram, Scene *scene);

#endif /* !INIT_HPP_ */
