
#version 430 core
layout (location = 4) in vec2 pPos;
//uniform vec2 uPlayerPos;
uniform vec2 uniformPlayerPosition;

void main()
{
    gl_Position = vec4(pPos + uniformPlayerPosition, 0.0, 1.0);
    //gl_Position = uniformPlayerPosition * vec4(pPos, 0, 1.0);
    //gl_PointSize = 20.0;
}
