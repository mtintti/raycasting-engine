#pragma once
#include "Renderer.h"
#include "Maze.h"
class Game{
    private:
    //var
    //func
    void init();

    public:
    //const/dest
    Game(int window_w, int window_h, int mazeSize, int gameSize);
    virtual ~Game();

    Renderer renderer;
    Maze maze;
    // func
    bool update(double dt);
    void render();
};