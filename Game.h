#pragma once
#include "Maze.h"
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "color.h"

extern glm::vec2 playerTranslations[6];
extern glm::vec3 playerPosition; // was glm::vec2
extern glm::vec3 nextPlayerPosition;

//extern std::vector<std::vector<double>> rcolor;
//extern std::vector<std::vector<double>> gcolor;
//extern std::vector<std::vector<double>> bcolor;
//extern std::vector<uint32_t> image;
extern bool renderProjection;
extern bool projectionisDone;
extern std::vector<rgb> image;

class Game{
    private:
    //var
    //func
    void init();

    void playerinit();

public:
    //const/dest
    Game(int window_w, int window_h, int mazeSize, int gameSize);
    virtual ~Game();

    Renderer renderer;
    Maze maze;
    glm::vec3 camera;
    void colorAddNeighbours(float up, float right, float south, float left, int ourInstIndex);
    int nextmovedColumn;
    int nextmovedRow;
    int lastRow = 0;    // for less print statement only if change row
    int lastColumn = 0; // for less print statement only if change column
    glm::vec3 rayDirection;
    //glm::vec3 originForRay;
    glm::vec3 origin; // where our player is currently in 3d world
    glm::vec3 sphereCenter;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::vec3 cameraDirection = {0.0f, 0.0f, 1.0f}; // where is our players camera/ view pointing right now
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float discriminaatti; //original
    double dist;
    //glm::vec2 sightline;
    std::vector<glm::vec2> sightline;
    glm::vec2 previousPos = {-1, -1};
    glm::vec2 currentPos={0,0};
    float mouseSensitivity = 0.2f;
    glm::vec2 sightlinedirection = {0.0f, 0.0f};

    float yn;
    float yx;
    float playersize;

    float cellSize; 
    float deltax, deltay, angle;
    float pi = 3.1415926535;
    float pi2 = pi/2;
    float pi3 = 3*pi/2;
    int r, mx, my, mp, dof;
    float rX, rY, rayAngle, xo, yo, distanceTotal;
    float offset = 0.0174533;
    float playerAngle = 0.0f;
    int debugtileX;
    int debugtileY;

    glm::vec2 startPoint;
    glm::vec2 raysPoint;
    float traveled;

    glm::vec3 raydirection;
    //float raydirection;
    //float sightlinedirection = 0.f;
    //float x = sightlinedirection; // rayX
    //float y = sightlinedirection; // rayY


    //uint32_t rgbColors;
    //float rcolors[800*800*3];
    //float gcolors[800*800*3];
    //float bcolors[800*800*3];
    
    static constexpr int w = 800;
    static constexpr int h = 800;
    //std::vector<double> image;
    std::vector<rgb> image;
    
    /*struct rgb {
        uint32_t red;
        uint32_t green;
        uint32_t blue;
    };*/
    
    
    
    //uint32_t image[w*h];
    
    //glm::vec3 image;
    double nX = 0; 
    double nY = 0;

    glm::vec3 color;
    
    // func
    bool update(double dt);
    float distance(float ax, float ay, float bx, float by, float ang);
    // uint32_t pixel(glm::vec2 coord); //coords for projection picture creation
    void projection();
    //uint32_t rayedImage(float discriminaatti); // temp mihin rayed kuva menee -> jos renderer puoleen??
    void render();
    
    void setpixel(int x, int y, float r, float g, float b); // rgb uint32_t

    

};