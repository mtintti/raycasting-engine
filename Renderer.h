#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"


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
    void render();
    void init();
    
    unsigned int quadVAO, quadVBO, colour;
    unsigned int playerVAO, playerVBO, playertranslVBO;
    GLuint shaderProg, playerProg, colorProg;
    GLuint vao;
    GLuint vbo;
    


    // --- Shader functions ---
    GLuint Load(const char* path, GLenum type);
    GLuint CreateShader();

    GLuint CreatePlayerShader();


    // --- Buffers ---
    GLuint GenerateVertexBuffer();
    GLuint GenerateVertexArray();


    void updateColors(glm::vec3 *colors); //colorChange

    GLuint playerInit(); // playerpos in left ver of maze
};


#endif 