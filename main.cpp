
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Game.cpp"
#include <chrono>
#include <iostream>
#include <thread>

float initobjects[] = {
    -0.6f, -0.4f, 1.f, 0.f, 0.f,
    0.6f, -0.4f, 0.f, 1.f, 0.f,
    0.f, 0.6f, 0.f, 0.f, 1.f};
//  "#version 330 core"
static const char *vertextext =
    "#version 430 core"
    "layout(location = 0) in vec2 vPos;"
    "layout(location = 1) in vec3 vCol;"

    "uniform mat4 MVP;"

    "out vec3 color;"

    "void main()"
    "{"
    "gl_Position = MVP * vec4(vPos, 0.0, 1.0);"
    "color = vCol;"
    "}";

static const char *fragmentext =
    "#version 430 core\n"
    "in vec3 color;\n"
    "out vec4 FragColor;\n"

    "void main()\n"
    "{\n"
    "FragColor = vec4(color, 1.0);\n"
    "}\n";

// -------------------------------
// Debug callback function (global)
// -------------------------------
void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar *message,
                            const void *userParam)
{
    // ignore non-significant error/warning codes (optional)
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl
              << std::endl;
}

const char *glErrorString(GLenum error)
{
    switch (error)
    {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    default:
        return "Unknown Error";
    }
}

#define GL_CHECK(x)                                            \
    do                                                         \
    {                                                          \
        x;                                                     \
        GLenum err;                                            \
        while ((err = glGetError()) != GL_NO_ERROR)            \
        {                                                      \
            std::cerr << "OpenGL error " << glErrorString(err) \
                      << " at " << __FILE__ << ":" << __LINE__ \
                      << " for call " << #x << std::endl;      \
        }                                                      \
    } while (0)

#define INFOforDEBUG true
#define Cell 72
#define mazeSize 20
#define gameSize 20

bool running = true;
int window_w = 1200;
int window_h = 800;

int main(int argc, char **argv)
{

    double FPS = 60.0;
    double nanosecondsFrame = 1000000000.0 / FPS;
    auto timelast = std::chrono::steady_clock::now();
    double framesHavePast = 0.0;
    double elapsed = 0.0;

#if INFOforDEBUG
    double elapsedSecondsInTotal = 0.0;
    int invidualframe = 0;
#endif
    //bool running = true;
    //int window_w = 1200;
    //int window_h = 800;
    Game game(window_w, window_h, mazeSize, gameSize); // {WINDOW_W, WINDOW_H}
    while (running)
    {
        auto curr = std::chrono::steady_clock::now();
        elapsed = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(curr - timelast).count();
        // timelast = curr; what is this doing here??
        // framesHavePast += elapsed / nanosecondsFrame;
        timelast = curr;
        framesHavePast += elapsed;

        // not needd for now, unthrottheled test, timelast = curr;

        // if(elapsed >= 1.0){ // causes our fps to be unthrottled?
        if (framesHavePast >= nanosecondsFrame)
        {
            running = game.update(nanosecondsFrame * ((double)Cell / 36));
            game.render();
            //elapsed = 0.0;
#if INFOforDEBUG
            invidualframe++;
#endif

            framesHavePast -= nanosecondsFrame;
        }
#if INFOforDEBUG
        elapsedSecondsInTotal += elapsed / 1000000000.0;
        if (elapsedSecondsInTotal >= 1.0)
        {
            std::cout << "----\n";
            std::cout << "fps " << double(invidualframe) / elapsedSecondsInTotal << "\n";
            elapsedSecondsInTotal = 0.0;
            invidualframe = 0;
            std::cout << "----\n";
        }
#endif
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
    return 0;

    /*
     GLFWwindow *window;
     //GLuint vertexshader, vertexbuffer, fragmenshader;
     const char *description;

     if (!glfwInit())
     {
         glfwGetError(&description);
         printf("Error: %s\n", description);
         exit(EXIT_FAILURE);
     };

     glfwInit();
     // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

     window = glfwCreateWindow(window_w, window_h, "Split view demo", NULL, NULL);
     if (!window)
     {
         fprintf(stderr, "Failed to open GLFW window\n");

         glfwTerminate();
         exit(EXIT_FAILURE);
     }

     // glfwSetInputMode(windows[i], GLFW_STICKY_KEYS, GLFW_TRUE);

     // glfwMakeContextCurrent(windows[i]);
     glfwMakeContextCurrent(window);

     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
     {
         std::cerr << "Failed to initialize GLAD" << std::endl;
         return -1;
     }
     // gladLoadGL(glfwGetProcAddress);
     // gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
     // glClearColor(colors[i].r, colors[i].g, colors[i].b, 1.f);
     // void framebuffer_size_callback(GLFWwindow * window, int width, int height);

     /* old -> * / glBlitFramebuffer()
     GLuint texture;
     glad_glGenTextures(1, &texture);
     glad_glBindTexture(GL_TEXTURE_2D, texture);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1200, 800, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

     glad_glBindTexture(GL_TEXTURE_2D, 0);

     // VISSIIN SE DEST TEXTUURI?
     GLuint depth;
     glGenTextures(1, &depth);
     glad_glBindTexture(GL_TEXTURE_2D, depth);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1200, 800, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
     glad_glBindTexture(GL_TEXTURE_2D, 0);

     // RAKENNETAAN SE FRAMEBUFFERI
     GLuint fb;
     glGenFramebuffers(1, &fb);
     glad_glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)fb);
     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

     GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
     if (status != GL_FRAMEBUFFER_COMPLETE)
         // Error meille että nehdään missä mättää

         glBindFramebuffer(GL_FRAMEBUFFER, 0);
 */

    /*glGenBuffers(1, &vertexbuffer);
    glad_glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(initobjects), initobjects, GL_STATIC_DRAW);

    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertextext, NULL);
    glCompileShader(vertexshader);

    std::cout << "vertexshader done\n";
    // errorit kasaan
    int success;
    char infoLog[512];
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    } //

    fragmenshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmenshader, 1, &fragmentext, NULL);
    glCompileShader(fragmenshader);
    std::cout << "fragmentshader done\n";

    unsigned int program;
    program = glCreateProgram();
    glAttachShader(program, vertexshader);
    glAttachShader(program, fragmenshader);
    glLinkProgram(program);
    std::cout << "program created, attached and linked!\n";*/

    /*vbo and vao
    std::cout << "staring vbo.. \n";
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(initobjects), initobjects, GL_STATIC_DRAW);
    std::cout << "vbo done.. \n";

    std::cout << "staring vao.. \n";
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glad_glBindVertexArray(VAO);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // color vertex attribute
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2* sizeof(float)));
    glEnableVertexAttribArray(1);

    glad_glEnableVertexAttribArray(0);

    GLint shadererror;
    glGetShaderiv(fragmenshader, GL_COMPILE_STATUS, &shadererror);
    if(shadererror != GL_TRUE){
        GLsizei lengthLog = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fragmenshader, 1024, &lengthLog, message);
        std::cout << "Error in shader: "<< fragmenshader << " length of log: "<< lengthLog <<" and message " << message;

    }
    std::cout << "vao done..\n ";

    auto glstatus = glGetError();
    if (glstatus != GL_NO_ERROR)
    {
        std::cout << "Error in GL call: " << glstatus << std::endl;
    };*/

    /* // alku framebuffer
     GLuint sfbo, render;
     glGenFramebuffers(1, &sfbo);
     GLenum statusglen1 = glGetError();
     if (statusglen1 != GL_NO_ERROR)
     {
         std::cout << "Error in GL call1: " << glstatus << std::endl;
     };


     std::cout << "Starting framebuffers\n ";
     glBindFramebuffer(GL_FRAMEBUFFER, sfbo);
     glGenRenderbuffers(1, &render);
     glBindRenderbuffer(GL_RENDERBUFFER, render);
     glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, 1200, 800); // tätä ei nähdä, -> offscreen
     glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, render);
     glBindFramebuffer(GL_FRAMEBUFFER, 0);

     glBindFramebuffer(GL_READ_FRAMEBUFFER, sfbo);
     GLenum statusglen3 = glGetError();
     if (statusglen3 != GL_NO_ERROR)
     {
         std::cout << "Error in GL call3: " << glstatus << std::endl;
     };

     // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dfbo);
     glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
     GLenum statusglen4 = glGetError();
     if (statusglen4 != GL_NO_ERROR)
     {
         std::cout << "Error in GL call4: " << glstatus << std::endl;
     };
     std::cout << "blitz...\n ";
     glBlitFramebuffer(0, 0, 1200 /*sfbo width, 800 /*sfbo height, 0, 0, 700, 800, GL_COLOR_BUFFER_BIT, GL_LINEAR);
     GLenum statusglen5 = glGetError();
     if (statusglen5 != GL_NO_ERROR)
     {
         std::cout << "Error in GL call5: " << glstatus << std::endl;
     };
     // glBindBuffer(GL_FRAMEBUFFER, 0);
     // GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
     // GL_CHECK(glViewport(0, 0, 1024, 768));
     GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
     glViewport(0, 0, 1024, 768);
     std::cout << "framebuffers and blitz done..\n ";*/

    // glBindFramebuffer(GL_FRAMEBUFFER, sfbo); // or sfbo
    //  origin frameerror

    /*GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {

        std::cout << "OpenGL error: " << err;
        GL_CHECK(err);
    }*/

    // glUseProgram(program); do we need to have this twice in total??
    /* -- Not in curr use -- GLint mvpLoc = glGetUniformLocation(program, "MVP");
    if (mvpLoc != -1)
    {
        // identity matrix
        const float I[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, I);
    } -- Not in curr use --*/

    /*int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        std::cout << "OpenGL Debug Context enabled!" << std::endl;

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    else
    {
        std::cout << "Debug context not available." << std::endl;
    }*/

    /* old 'render' cycle while (!glfwWindowShouldClose(window))
    {


        glBlitFramebuffer(0, 0, 1200, 800, 0, 0, 600, 800, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        // scaled height to half of window

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;*/
}
/*
for (;;)
{
    for (int i = 0; i < 2; i++)
    {
        glfwMakeContextCurrent(windows[i]);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(windows[i]);

        if (glfwWindowShouldClose(windows[i]) ||
            glfwGetKey(windows[i], GLFW_KEY_ESCAPE))
        {
            glfwTerminate();
            exit(EXIT_SUCCESS);
        }
    }

    glfwWaitEvents();
}*/
