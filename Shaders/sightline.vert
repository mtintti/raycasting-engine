#version 430 core
layout(location = 7) in vec2 sightpos;
//layout (location = 7) in vec2 offsetInstanced;
uniform vec2 uniformLinePosition;

void main() {
    gl_Position = vec4(sightpos, 0.0, 1.0);
}