//
// scene.hpp for OpenGL in /home/nicolaspolomack/demos/C++/OpenGL/compute/include
//
// Made by nicolaspolomack
// Login   <nicolas.polomack@epitech.eu>
//
// Started on  Sat Sep 15 09:31:55 2017 nicolaspolomack
// Last update Sat Sep 15 09:41:49 2017 nicolaspolomack
//

#ifndef SCENE_HPP_
# define SCENE_HPP_

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

typedef struct {
    glm::vec3   pos;
    glm::vec3   rot;
    float       fov;
    float       reflectDepth;
} Camera;

typedef struct {
    float       type;
    glm::vec3   pos;
    glm::vec3   rot;
    glm::vec4   color;
    float       radius;
    float       diffuse;
    float       specular;
    float       shininess;
    float       reflect;
} Object;

typedef struct {
    glm::vec3   pos;
    glm::vec4   color;
} Light;

typedef struct {
    Camera      camera;
    float       nb_objects;
    Object      *objects;
    float       nb_lights;
    Light       *lights;
} Scene;

#endif /* !SCENE_HPP_ */
