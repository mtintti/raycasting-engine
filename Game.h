#pragma once
#include "Maze.h"
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "color.h"

extern glm::vec2 playerTranslations[6];
extern glm::vec2 playerPosition;
extern glm::vec2 nextPlayerPosition;

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
    glm::vec3 originForRay;
    float discriminaatti; //original
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
    //uint32_t pixel(glm::vec2 coord); //coords for projection picture creation
    void projection();
    //uint32_t rayedImage(float discriminaatti); // temp mihin rayed kuva menee -> jos renderer puoleen??
    void render();
    
    void setpixel(int x, int y, float r, float g, float b); // rgb uint32_t

    

};