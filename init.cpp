//
// init.cpp for OpenGL in /home/nicolaspolomack/demos/C++/OpenGL/compute
//
// Made by nicolaspolomack
// Login   <nicolas.polomack@epitech.eu>
//
// Started on  Sat Sep 15 12:48:55 2017 nicolaspolomack
// Last update Wed Sep 26 16:08:45 2017 nicolaspolomack
//

#include <sstream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "scene.hpp"

void        initGL() {
    glClearDepth(1);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glCullFace(GL_FRONT_AND_BACK);
    glClearColor(0, 0, 0, 1);
}

void        initBuffers(GLuint *vao, GLuint *buffer) {
    GLfloat vertices[] = {
        -1.f, -1.f,
        0.f, 1.f,
        -1.f, 1.f,
        0.f, 0.f,
        1.f, -1.f,
        1.f, 1.f,
        -1.f, 1.f,
        0.f, 0.f,
        1.f, -1.f,
        1.f, 1.f,
        1.f, 1.f,
        1.f, 0.f
    };

    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void    initTexture(GLuint *texture, int width, int height) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, *texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void        initScene(Scene **scene, char *file) {
    *scene = (Scene *)malloc(sizeof(Scene));
    (*scene)->camera.pos = glm::vec3(0, 0, 0);
    (*scene)->camera.rot = glm::vec3(0, 0, 0);
    (*scene)->camera.fov = 70;
    (*scene)->camera.reflectDepth = 10;
    
    (*scene)->nb_objects = 3;
    (*scene)->objects = (Object *)malloc(sizeof(Object) * (*scene)->nb_objects);

    (*scene)->objects[0].type = 0; // Sphere
    (*scene)->objects[0].pos = glm::vec3(0, 0, 20);
    (*scene)->objects[0].rot = glm::vec3(0, 0, 0);
    (*scene)->objects[0].color = glm::vec4(1, 0.2, 0.2, 1);
    (*scene)->objects[0].radius = 5;
    (*scene)->objects[0].diffuse = 1;
    (*scene)->objects[0].specular = 3;
    (*scene)->objects[0].shininess = 50;
    (*scene)->objects[0].reflect = 0.5;

    (*scene)->objects[2].type = 0; // Sphere
    (*scene)->objects[2].pos = glm::vec3(15, 0, 20);
    (*scene)->objects[2].rot = glm::vec3(0, 0, 0);
    (*scene)->objects[2].color = glm::vec4(0.2, 0.2, 1, 1);
    (*scene)->objects[2].radius = 5;
    (*scene)->objects[2].diffuse = 1;
    (*scene)->objects[2].specular = 3;
    (*scene)->objects[2].shininess = 50;
    (*scene)->objects[2].reflect = 0.5;

    (*scene)->objects[1].type = 1; // Plane
    (*scene)->objects[1].pos = glm::vec3(0, -3, 0);
    (*scene)->objects[1].rot = glm::vec3(0, 0, 0);
    (*scene)->objects[1].color = glm::vec4(1, 1, 1, 1);
    (*scene)->objects[1].diffuse = 1;
    (*scene)->objects[1].specular = 1;
    (*scene)->objects[1].shininess = 30;
    (*scene)->objects[1].reflect = 0.5;
    
    (*scene)->nb_lights = 1;
    (*scene)->lights = (Light *)malloc(sizeof(Light) * (*scene)->nb_lights);
    (*scene)->lights[0].pos = glm::vec3(10, 10, 0);
    (*scene)->lights[0].color = glm::vec4(1, 1, 1, 1);
}

void    setSceneUniforms(GLuint computeProgram, Scene *scene) {
    
    glUniform3fv(glGetUniformLocation(computeProgram, "uCamera.pos"), 1, glm::value_ptr(scene->camera.pos));
    glUniform3fv(glGetUniformLocation(computeProgram, "uCamera.rot"), 1, glm::value_ptr(scene->camera.rot));
    glUniform1f(glGetUniformLocation(computeProgram, "uCamera.fov"), scene->camera.fov);
    glUniform1f(glGetUniformLocation(computeProgram, "uCamera.reflectDepth"), scene->camera.reflectDepth);
    
    glUniform1f(glGetUniformLocation(computeProgram, "uNbObjects"), scene->nb_objects);
    
    for (int i = 0; i < scene->nb_objects; i++) {
        std::ostringstream os;
        os << "uObjects[" << i << "]";
        glUniform1f(glGetUniformLocation(computeProgram, os.str().append(".type").c_str()), scene->objects[i].type);
        glUniform3fv(glGetUniformLocation(computeProgram, os.str().append(".pos").c_str()), 1, glm::value_ptr(scene->objects[i].pos));
        glUniform3fv(glGetUniformLocation(computeProgram, os.str().append(".rot").c_str()), 1, glm::value_ptr(scene->objects[i].rot));
        glUniform4fv(glGetUniformLocation(computeProgram, os.str().append(".color").c_str()), 1, glm::value_ptr(scene->objects[i].color));
        glUniform1f(glGetUniformLocation(computeProgram, os.str().append(".radius").c_str()), scene->objects[i].radius);
        glUniform1f(glGetUniformLocation(computeProgram, os.str().append(".diffuse").c_str()), scene->objects[i].diffuse);
        glUniform1f(glGetUniformLocation(computeProgram, os.str().append(".specular").c_str()), scene->objects[i].specular);
        glUniform1f(glGetUniformLocation(computeProgram, os.str().append(".shininess").c_str()), scene->objects[i].shininess);
        glUniform1f(glGetUniformLocation(computeProgram, os.str().append(".reflect").c_str()), scene->objects[i].reflect);
    }

    glUniform1f(glGetUniformLocation(computeProgram, "uNbLights"), scene->nb_lights);
    
    for (int i = 0; i < scene->nb_lights; i++) {
        std::ostringstream os;
        os << "uLights[" << i << "]";
        glUniform3fv(glGetUniformLocation(computeProgram, os.str().append(".pos").c_str()), 1, glm::value_ptr(scene->lights[0].pos));
        glUniform4fv(glGetUniformLocation(computeProgram, os.str().append(".color").c_str()), 1, glm::value_ptr(scene->lights[0].color));
    }
}