#version 430

layout (local_size_x = 1, local_size_y = 1) in;

layout (rgba32f, binding = 0) uniform image2D destTex;

uniform vec2    uSize;
uniform float   uTime;

void        main() {

    vec2    pos = gl_GlobalInvocationID.xy;
    vec2    halfSize = uSize / 2.0;
    float   color = float(int(halfSize.x + ((halfSize.x + uTime) * sin(pos.x / 8.0)) + halfSize.y + ((halfSize.y + uTime) * sin(pos.y / 8.0)))) / 2.0 / 255.0;

    imageStore(destTex, ivec2(pos), vec4(color, color, color, 1));
}