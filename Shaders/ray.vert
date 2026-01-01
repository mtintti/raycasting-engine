
#version 430 core

layout (location = 5) in vec2 QuadPos;
layout (location = 6) in vec3 pixelcolor;
//layout (location = 6) in vec2 textureCoordinates;

out vec3 tofragment;
out vec2 texCoord;

void main(){
    gl_Position = vec4(QuadPos.x, QuadPos.y, 0.0, 1.0);
    tofragment = pixelcolor;
    //texCoord = textureCoordinates;
    
}