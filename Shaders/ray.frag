
#version 430 core
in vec3 tofragment;
in vec2 texCoord;

uniform sampler2D ourImage;

out vec4 FragColor;

void main() {
    
    //FragColor = texture(ourImage, texCoord);
    //FragColor =vec4(tofragment,1.0);
    FragColor = vec4(tofragment,1.0);
}
