#include <glad/glad.h>
#include "Renderer.h"
#include "gobals.h"
#include <iostream>
#include <vector>
#include <fstream> // load() funct
#include <sstream> // load() funct
#include "Maze.h"
#include "Game.h"
//#include "color.h"

GLFWwindow *window = nullptr;
//float discriminaatti; if using renderer extern

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



void CheckGLError(const char* label) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL Error at " << label << ": " << err << std::endl;
    }
}

//shader creation and rendereing // error testing debug
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

// temp

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
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // comment out if realised

    window = glfwCreateWindow(window_w, window_h, "Split view demo", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window\n");

        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    //error test gldebugOutput
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glEnable(GL_SCISSOR_TEST); // scissor for viewports
    }
        
}



GLuint Renderer::Load(const char* path, GLenum type){
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();

    std::string src = ss.str();
    std::cout << "\n file from load src" << src << "\n";
    const char* cstr = src.c_str();
    if (!file.is_open()) {
        std::cout << "Could be not openned shader file: " << path << "\n";
    }

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &cstr, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cout << "Shader error: " << log << " in path: "<< path<< "\n";
    };
    
    return shader;
    
}

GLuint Renderer::CreateShader(){
    
    GLuint vert = Load("Shaders/Genom.vert", GL_VERTEX_SHADER); 
    GLuint frag = Load("Shaders/Genom.frag", GL_FRAGMENT_SHADER); 
    std::cout << "vert shader ID = " << vert << "\n";
    std::cout << "frag shader ID = " << frag << "\n";

   
    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vert);
    CheckGLError("attach vert");
    glAttachShader(shaderProg, frag);
    CheckGLError("attach frag");
    
    
    glLinkProgram(shaderProg);
    GLint linked;
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[1024];
        glGetProgramInfoLog(shaderProg, 1024, NULL, log);
        std::cout << "SProgram Link was not success!\n" << log << "\n";
    }
    std::cout << "SProgram Link was success!\n";
    
    return shaderProg;

}

GLuint Renderer::CreatePlayerShader(){
    GLuint pvert = Load("Shaders/player.vert", GL_VERTEX_SHADER); 
    GLuint pfrag = Load("Shaders/player.frag", GL_FRAGMENT_SHADER); 
    std::cout << "vert playershader ID = " << pvert << "\n";
    std::cout << "frag playershader ID = " << pfrag << "\n";
   
    playerProg = glCreateProgram();
    CheckGLError("create playerProg");
    glAttachShader(playerProg, pvert);
    CheckGLError("attach vert");
    glAttachShader(playerProg, pfrag);
    CheckGLError("attach frag");
    
    
    glLinkProgram(playerProg);
    CheckGLError("link playerProg");
    GLint linked;
    glGetProgramiv(playerProg, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[1024];
        glGetProgramInfoLog(playerProg, 1024, NULL, log);
        std::cout << "pProgram Link was not success!\n" << log << "\n";
    }
    std::cout << "pProgram Link was success!\n";
    return playerProg;
};


GLuint Renderer::CreateRayQuadShader(){
    
    GLuint rayvert = Load("Shaders/ray.vert", GL_VERTEX_SHADER); 
    GLuint rayfrag = Load("Shaders/ray.frag", GL_FRAGMENT_SHADER); 
    std::cout << "vert shader ID = " << rayvert << "\n";
    std::cout << "frag shader ID = " << rayfrag << "\n";

   
    rayProg = glCreateProgram();
    glAttachShader(rayProg, rayvert);
    CheckGLError("attach rvert");
    glAttachShader(rayProg, rayfrag);
    CheckGLError("attach rfrag");
    
    
    glLinkProgram(rayProg);
    GLint linked;
    glGetProgramiv(rayProg, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[1024];
        glGetProgramInfoLog(rayProg, 1024, NULL, log);
        std::cout << "RProgram Link was not success!\n" << log << "\n";
    }
    std::cout << "RProgram Link was success!\n";
    
    return rayProg;

}

//VBO for tilemap texture
GLuint Renderer::GenerateVertexBuffer(){
    
    glGenBuffers(1, &vbo);
    CheckGLError("vbo");
    std::cout <<"\n VBO ID: "<<vbo << "\n";
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec2) * 441, &translations[0], GL_STATIC_DRAW);
    int size = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    std::cout << "GL VBO buffer size: " << size << " bytes\n";
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    return vbo;
}




//VAO for one tileID
GLuint Renderer::GenerateVertexArray(){

    float quadVertices[] = {
        // positions    
        -0.05f,  0.05f, 
        0.05f, -0.05f, 
        -0.05f, -0.05f, 

        -0.05f,  0.05f, 
         0.05f, -0.05f,
         0.05f,  0.05f, 
    };

    
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &colour);
    glBindVertexArray(quadVAO);


    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, colour);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),0);
    glVertexAttribDivisor(1,1); 

    //  instance data tilemap:ille
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // meidän translated vectori pisteet
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); //on instanced vertex attribute.
    return quadVAO;

};


void Renderer::updateColors(glm::vec3* colors)
{
    std::cout <<"\n";
    std::cout << "\n in updateColors in Renderer class";
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, colour);
    CheckGLError("quadVAO and color buff bind in updateColors");
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec3) * 441,colors);
    CheckGLError("after bufferSubData in updateColors");
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),0);
    CheckGLError("after vertexAttrib in updateColors");
    glVertexAttribDivisor(1,1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CheckGLError("after bindBuffer in updateColors");
 
}


GLuint Renderer::playerInit(){

    float playerVertices[] = {
        // positions     
        -0.05f,  0.05f, 
    };

    //player/camera VA0 
    glGenVertexArrays(1, &playerVAO);
    glBindVertexArray(playerVAO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    return playerVAO;


};
float verticesQuad[2 * 800 * 800];

int hittedr = 0;
int missedr = 0;

GLuint Renderer::GenerateQuadForRay(const std::vector<rgb> image){
    // checking if we can see our hits and misses
    for(int i = 0; i < image.size(); i++){
        if(image[i].red == 1.0f){
            hittedr++;
        };
        if(image[i].red == 0.5f){
            missedr++;
        };
    };
    std::cout << "\n from render hit: " << hittedr;
    std::cout << "\n from render missed: " << missedr;

    //unsigned int quadRayVAO, quadRayVBO;
    glGenBuffers(1, &quadRayVBO); // vertexQuad points
    glBindBuffer(GL_ARRAY_BUFFER, quadRayVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesQuad), verticesQuad, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, quadRayVBO);
    CheckGLError("quadrayVAO bind creation calls");

    // one x and y quadVertex float value with a stride of 2 is one pixel point
    glGenVertexArrays(1, &quadRayVAO);
    glBindVertexArray(quadRayVAO); // overall used for shader
    glBindBuffer(GL_ARRAY_BUFFER, quadRayVBO);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5,2,GL_FLOAT,GL_FALSE, 2 * sizeof(float), 0);
    CheckGLError("quadrayVAO vertex point call");
    
    // invidual rgb colors for pixels sent to shaders
    // setting every 3 float values as one pixels colors
    glGenBuffers(1, &inviRays);
    glBindBuffer(GL_ARRAY_BUFFER, inviRays);
    glBufferData(GL_ARRAY_BUFFER, image.size() * sizeof(rgb), image.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),0); // or offset as -> (void*)(2* sizeof(float))
    CheckGLError("quadrayVAO image rgb colors attached");

    /*glEnableVertexAttribArray(6); for texture
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE,4 * sizeof(float),  (void*)(2 * sizeof(float)));
    glBindVertexArray(0); 
    CheckGLError("quadrayVAO texture coordinates set");*/

    
    return quadRayVAO;

};


void Renderer::init()
{

    CreateShader();
    CreatePlayerShader();
    CreateRayQuadShader();
    if(translations->length() <0){
        std::cout <<"\n translations are not on renderer side!";
    };
    if(translations->length > 0){
        std::cout <<"\n translations are in renderer side " << translations->length() <<" ";
        for(int i = 0; i < translations->length(); i++){
            std::cout <<"t = " << i  << translations[i].x << " , " << translations[i].y;
        }
    };
    GenerateVertexBuffer();
    GenerateVertexArray();
    std::cout << "\n render init";
    std::cout << "\n projection done -> showing in render";
    std::cout <<"player pos for uniform "<< playerPosition.x << " , " << playerPosition.y;

    
};
//extern float discriminaatti;



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
        
    }
}

void Renderer::displaypixel(const std::vector<rgb> image){

    
    //unsigned int texture; maybe soon to be passed colors and pixels?? as a texture to
    // pass on changed window size change??
    glGenFramebuffers(1, &texture);
    glBindFramebuffer(GL_FRAMEBUFFER, texture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 800, 800, 0,GL_RGB, GL_UNSIGNED_BYTE, image.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    /*std::cout <<"\n";
    std::cout << "\n in displaypixel in Renderer class";
    glBindVertexArray(quadRayVAO);
    glBindBuffer(GL_ARRAY_BUFFER, inviRays);
    CheckGLError("quadVAO and invidual ray buff bind in displaypixel");
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(image),&image[0]);
    CheckGLError("after bufferSubData in displaypixel");
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(image),0);
    CheckGLError("after vertexAttrib in displaypixel");
    glVertexAttribDivisor(6,1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CheckGLError("after bindBuffer in displaypixel");*/



    
};

void Renderer::render(const std::vector<rgb> image)
{
    
        glViewport(0, 0, window_w/2, window_h);
        glScissor(0, 0, window_w/2, window_h);
        
        glUseProgram(shaderProg); // tiles
        CheckGLError("maze shaderProg");
        glBindVertexArray(quadVAO); // tiles

        CheckGLError("after quadVAO Bind");
        glDrawArraysInstanced(GL_TRIANGLES,0,6,441);

        // player draw call
        CheckGLError("before playerprog");
        glUseProgram(playerProg);
        
        CheckGLError("after playerprog"); 
        GLint loc = glGetUniformLocation(playerProg, "uniformPlayerPosition");
        CheckGLError("get uniformplayerpos in prog");
        glUniform2f(loc, playerPosition.x, playerPosition.y);
        CheckGLError("after uniform2f call with x,y");
        glBindVertexArray(playerVAO);
        CheckGLError("after playerVAO Bind to prog");
        glPointSize(20);
        glDrawArrays(GL_POINTS, 0, 2);
        CheckGLError("draw points call");

       
        glViewport (window_w/2, 0, window_w/2, window_h);
        glScissor(window_w/2, 0, window_w/2, window_h);
        //glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // green

        glUseProgram(rayProg);
        CheckGLError("rayProg render()");
        //glBindTexture(GL_TEXTURE_2D, texture);
        CheckGLError("render() bindTexture call");
        glPointSize(1);
        glBindVertexArray(quadRayVAO);
        CheckGLError("render() bindvertex quadrayVAO");
        glDrawArrays(GL_POINTS, 0, 800*800);
        CheckGLError("quad drawArrays call after ");

        glfwPollEvents();
        processInput(window);
        glfwSwapBuffers(window);
}
