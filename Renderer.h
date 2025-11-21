#include <GLFW/glfw3.h>

class Renderer
{
private:
    /* data */
    GLFWwindow *window;
    const char *description;
    GLuint vertexbuff; //VAO

public:
    Renderer(int window_w, int window_h);
    ~Renderer();
    void render();
    void init();

};


