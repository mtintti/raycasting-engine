
#version 430 compatibility

layout (location = 5) in vec2 aPos;
layout (location = 6) in vec3 aColor;
layout (location = 7) in vec2 aOffset;


out vec3 fColor;

void main()
{
    
    vec2 pos = aPos + aOffset;
    fColor = aColor;
    gl_Position = vec4(pos, 0.0, 1.0);
}