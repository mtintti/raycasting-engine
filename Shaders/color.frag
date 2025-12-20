//#version 330 core //FragColor = vec4( 255.0, 0.0, 0.0, 1.0);
#version 430 compatibility
out vec4 FragColor;

in vec3 fColor;

void main()
{
    //FragColor = vec4(fColor, 1.0);
    FragColor = vec4(fColor, 1.0);
}

//