#version 330 core

// uniform mat4    uPMatrix;
// uniform mat4    uMVMatrix;

layout (location = 0) in vec2 aPosition;
layout (location = 2) in vec2 aTexCoord;

out vec2    texCoord;

void main() {
    vec4 pos = vec4(aPosition, 1.0F, 1.0F);
    // vec4 mvPos = uMVMatrix * pos;
    // gl_Position = uPMatrix * mvPos;
    gl_Position = pos;

    texCoord = aTexCoord;
}