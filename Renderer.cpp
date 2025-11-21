#include <glad/glad.h>
#include "Renderer.h"
#include "gobals.h"
#include <iostream>
#include <vector>
#include <fstream> // load() funct
#include <sstream> // load() funct
#include "Maze.h"


Renderer::Renderer(int window_w, int window_h)
{
    std::cout << "Game initialized with window size " << window_w << " x " << window_h << std::endl;

    if (!glfwInit())
    {
        glfwGetError(&description);
        printf("Error: %s\n", description);
        exit(EXIT_FAILURE);
    };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_w, window_h, "Split view demo", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window\n");

        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    std::cout << "game window done -> next data init ";
    init();
}

void Renderer::init()
{
    // trying to create init render data
    // test of maze shown on screen
    std::cout << "\n data init: VertexArray ";
    GLuint VertexArrayID; // VBO
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    std::cout << "\n data init: VertexBuffer ";
    // GLuint vertexbuff; // VAO
    glGenBuffers(1, &vertexbuff);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuff);
    /*std::vector<int> convertedOpenGl; // muutetaan tilemap(sprites) 1d muotoon että pystytään käyttämään sitä in OpenGl

    for (const auto &f : tilemap)
    {
        convertedOpenGl.insert(convertedOpenGl.end(), f.begin(), f.end());
    };*/
    CreateShader();
    GenerateVertexBuffer();
    GenerateVertexArray();
}

GLuint Load(const char* path, GLenum type){
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();

    std::string src = ss.str();
    const char* cstr = src.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &cstr, nullptr);
    glCompileShader(shader);

    // check errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cout << "Shader error: " << log << "\n";
    }
    return shader;
}

GLuint CreateShader(){
    
    GLuint vert = Load("Shaders/Genom.vert", GL_VERTEX_SHADER); 
    GLuint geom = Load("Shaders/Genom.genom", GL_GEOMETRY_SHADER); 
    GLuint frag = Load("/Shaders/Genom.frag", GL_FRAGMENT_SHADER); 

    //GLenum type;
    //GLuint shader = glCreateShader(type);
    GLuint shaderProg;
    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vert);
    glAttachShader(shaderProg, geom);
    glAttachShader(shaderProg, frag);
    glLinkProgram(shaderProg);
    return shaderProg;
    //glShaderSource(shader, 1, tilemap, nullptr)


}

Renderer::~Renderer()
{
    glfwTerminate();
}

bool escPressed = false;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        escPressed = true;
        running = false;
        glfwTerminate();
    }
}

void Renderer::render()
{

    // std::cout << "Render::render called" << std::endl;
    // std::cout << "esc pressed: " << escPressed;

    // Draw the triangle !
    /*std::cout << "\n data init done - drawing pictures";
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);*/

    glBindVertexArray(vertexbuff);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    processInput(window);
    // glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
    glfwPollEvents();
}
