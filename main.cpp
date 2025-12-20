
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
  
    Game game(window_w, window_h, mazeSize, gameSize); // {WINDOW_W, WINDOW_H}
    while (running)
    {
        auto curr = std::chrono::steady_clock::now();
        elapsed = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(curr - timelast).count();
        timelast = curr;
        framesHavePast += elapsed;

        // not needd for now, unthrottheled test, timelast = curr;

        // if(elapsed >= 1.0){ // causes our fps to be unthrottled?
        if (framesHavePast >= nanosecondsFrame)
        {
            running = game.update(nanosecondsFrame * ((double)Cell / 36));
            
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
};