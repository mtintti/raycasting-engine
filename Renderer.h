#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "color.h"
#include <vector>

extern float verticesQuad[2 * 800*800];

class Renderer
{
private:
    /* data */
    const char *description;
    GLuint vertexbuff; //VAO

public:
    Renderer(int window_w, int window_h);
    ~Renderer();
    void drawPlayer();
    void updateInput(glm::vec2& playerPosition);
    void displaypixel(const std::vector<rgb> image);
    void render(const std::vector<rgb> image, std::vector<glm::vec2> sightline);
    void init();
    
    unsigned int quadVAO, quadVBO, colour;
    unsigned int playerVAO, playerVBO, playertranslVBO;
    unsigned int quadRayVAO, quadRayVBO, inviRays; // screen coordinates 1 -> -1 in VBO, ray colors in inviRays
    unsigned int sightlineVAO, sightlineVBO, sightlinesvbo;
    unsigned int texture;
    GLuint shaderProg, playerProg, colorProg, rayProg;
    GLuint sightlineProg;
    GLuint vao;
    GLuint vbo;

    // rayimage
    //GLuint FrameBuffer;
    //GLuint DepthBuffer;
    //GLuint Texture;
    
    


    // --- Shader functions ---
    GLuint Load(const char* path, GLenum type);
    GLuint CreateShader();

    GLuint CreatePlayerShader();
    GLuint CreateSightlineShader();
    GLuint CreateRayQuadShader(); // screen coordinates on the right side where later on we use our rayed image as a texture

    // --- Buffers ---
    GLuint GenerateVertexBuffer();
    GLuint GenerateVertexArray();


    void updateColors(glm::vec3 *colors); //colorChange
    void sightlineInit(); // our line to see where our player looks at currently VAO AND VBO

    void updateSightline(const std::vector<glm::vec2> sightline);


    // uint32_t rayedImage(float discriminaatti); // temp mihin rayed kuva menee -> jos renderer puoleen??
    // uint32_t rayedImage(); last used
    // GLuint rayedImageToScreen(float discriminaatti); last used
    //  uint32_t rayedImage(); // used if float in renderer
    GLuint playerInit(); // playerpos in left ver of maze
    GLuint GenerateQuadForRay(const std::vector<rgb> image);
};

#endif 