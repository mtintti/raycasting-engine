
#include "Game.h"
#include <iostream>
#include "gobals.h"
//#include <glad/glad.h> not needed if renderer has All openGl calls
//#include "Maze.cpp" not seems to be needed now that tilemap is external -> if changed to not extern in maze.h = uncomment


//const/dest
Game::Game(int window_w, int window_h, int mazeSize, int gameSize) 
:renderer(window_w, window_h){

    //Renderer renderer(window_w, window_h);
    std::cout << "Game initialized with window size..."  << std::endl;
    std::cout << "game projection initialized with size " << gameSize<< " x " <<gameSize << "\n";
    std::cout << "Game initialized with maze size " << mazeSize << " x " << mazeSize << std::endl;
    maze.creation(mazeSize);
    //init();
    
}

void Game::init() {
    //Maze.creation(mazeSize);
    std::cout << "in game.init() where entities are done";

    
    
}

bool Game::update(double dt){
    //std::cout << "in update now, delta time = "<< dt << "\n";
    return true;
}

void Game::render(){
    //rene
    //glClearColor(1.0,1.0,1.0,0.0);
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0,100.0,-100.0,100.0,0,0);
    glMatrixMode(GL_MODELVIEW);*/

    /* //misc attempt from tutorial
    float view[4][4];
    glPushMatrix();
    glClearColor(0.05f, 0.05f, 0.2f, 0.0f);
    
    // Setup modelview matrix (flat XY view)
    {
        glVertex3f eye ( 0.f, 0.f, 1.f );
        vec3 center = { 0.f, 0.f, 0.f };
        vec3 up = { 0.f, 1.f, 0.f };
        glm::look_at(view, eye, center, up);
    }*/ 

    
    

    /*//opengl-tutorial projection matrix

    glm::mat4 Proj = glm::perspective(glm::radians(45.0f), (float) window_w / (float)window_h, 0.1f, 100.0f);

    glm::mat4 ViewMatrix = glm::lookAt(
        glm::vec3(4,3,3), //Camera or eye
        glm::vec3(0,0,0), // cent
        glm::vec3(0,1,0) //looks upwards
    );

    glm::mat4 Model = glm::mat4(1.0f); // our model identity matrix at original location

    glm::mat4 MVP = Proj * ViewMatrix * Model; // projection of our ModelView

    GLuint MatrixId = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, &MVP[0][0]);


    // PROJ
    glMatrixMode(GL_PROJECTION);*/


    
    renderer.render();
    
}
Game::~Game(){

};