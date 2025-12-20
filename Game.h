#pragma once
#include "Maze.h"
#include <GLFW/glfw3.h>
#include "Renderer.h"

extern glm::vec2 playerTranslations[6];
extern glm::vec2 playerPosition;
extern glm::vec2 nextPlayerPosition;

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
    int lastRow = 0;    // for less print statement only if change row
    int lastColumn = 0; // for less print statement only if change column
    // func
    bool update(double dt);
    void render();
};