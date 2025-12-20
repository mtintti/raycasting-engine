
#include "Game.h"
#include <iostream>
#include "gobals.h"



glm::vec2 playerPosition;

//const/dest
Game::Game(int window_w, int window_h, int mazeSize, int gameSize) 
:renderer(window_w, window_h){

    std::cout << "Game initialized with window size..."  << std::endl;
    std::cout << "game projection initialized with size " << gameSize<< " x " <<gameSize << "\n";
    std::cout << "Game initialized with maze size " << mazeSize << " x " << mazeSize << std::endl;
    maze.creation(mazeSize);
    if(tilemapDone==true){
        init();
    };
    
}

void Game::init() {
    std::cout << "in game.init() where entities are done";
    
    std::cout << "game window done -> next data init ";
    playerinit();
    renderer.init();
    

};

void Game::playerinit(){
    int indNum = 0;
    camera = {0,0,0};
    for(int i = 0; i < row; i++){
        for(int j = 0; j < column; j++){
            indNum++;
            if(tilemap[i][j] == 2){
                std::cout <<"\n tilemap 2/start "<< tilemap[i][j]<<" found in " << i << " , " << j;
                std::cout <<"\n this makes our camera/player pos " << i+1<< " , " <<j <<" in " <<tilemap[i+1][j];
                camera = {i+1, j, 0};
                std::cout << "\n camera vec3 = "<< camera.x<<"," << camera.y <<","<< camera.z;
                std::cout << "\n index for transformations " << indNum;
                break;
            }
        }
    }

    int playerRow = camera.x;
    int playerCol = camera.y;

    //new 
    float playersize = 2.0f / row;
    float offset = playersize * 0.3f; 

    // sama logiikka kuin maze translatiossa. -1 ja +1 koska opengl ndc koordinaatti muutos. kerrotaan pelaajan koolla ja laitetaan offset hitbox:iksi
    playerPosition.x = -1.0f + playerCol * playersize + offset;
    playerPosition.y = 1.0f -playerRow * playersize -offset;

    


    std::cout << "\nPlayer world pos: "
              << playerPosition.x << ", "
              << playerPosition.y << "\n";

    lastRow = playerRow;
    lastColumn = playerCol;

    int instIndex = playerRow * column + playerCol;
    colors[instIndex] = {1.0f, 0.0f, 0.0f}; // debug red
    renderer.updateColors(colors);
    renderer.playerInit();


};
int debug = 0;
int ind = 0;


void Game::colorAddNeighbours(float up, float right, float south, float left, int ourInstIndex){
    if(up ==0 || right == 0 || south ==0 || left == 0){
        c = {0.100, 0.100, 0.100};
        colors[ourInstIndex] = c;
        std::cout <<"\n index of color after change "<< ourInstIndex<< " in: " << colors[ourInstIndex].r << " , "<< colors[ourInstIndex].b<< " , "<< colors[ourInstIndex].g;
    };
    renderer.updateColors(colors);
};

bool Game::update(double dt){
    glm::vec2 nextPlayerPosition;
    nextPlayerPosition.x = playerPosition.x;
    nextPlayerPosition.y = playerPosition.y;

    // katsotaan jos keyboard inputti tulee ja lisätään hieman y tai x liikkumista
    if(glfwGetKey(window, GLFW_KEY_W)== GLFW_PRESS){
        nextPlayerPosition.y += 0.005f;
    };
    if(glfwGetKey(window, GLFW_KEY_D)== GLFW_PRESS){
        nextPlayerPosition.x += 0.005f;
    };
    if(glfwGetKey(window, GLFW_KEY_S)== GLFW_PRESS){
        nextPlayerPosition.y -= 0.005f;
    };
    if(glfwGetKey(window, GLFW_KEY_A)== GLFW_PRESS){
        nextPlayerPosition.x -= 0.005f;
    };

    // opengl lookaatio taas, jos seuraava column tai row plus/miinus muunnos.
    // -1 +1 playerPosition numero muutetaan screen koordinaatiksi. 0.5f puolittaa saadun numeron ja sitten scalataan mazen koolla, 21.
    int nextmovedColumn = int((nextPlayerPosition.x + 1.0f) * 0.5f * column);
    int nextmovedRow = int((1.0f- (nextPlayerPosition.y + 1.0f) *0.5f) *row);
    

    if(nextmovedRow >= 0 && nextmovedRow < row && nextmovedColumn >= 0 && nextmovedColumn < column){
        if(tilemap[nextmovedRow][nextmovedColumn] == 1){
            
            
            int ourInstIndex = nextmovedRow * column + nextmovedColumn;
            // otetaan koordinaatit talteen jotta saadaan värit missä ollaan käyty.
            float up =  tilemap[nextmovedRow][nextmovedColumn+1];
            float right = tilemap[nextmovedRow+1][nextmovedColumn];
            float south = tilemap[nextmovedRow][nextmovedColumn-1];
            float left =  tilemap[nextmovedRow-1][nextmovedColumn];
           
            playerPosition = nextPlayerPosition; // jos ollaan reitillä niin liikutaan sauraavaan tileen
            if(nextmovedRow != lastRow || nextmovedColumn != lastColumn){
                //debug aina kun liikutaan seuraavaan celliin.
                std::cout << "\n lastrow: " << lastRow << " -> next: " << nextmovedRow;
                std::cout << "\n lastcolumn: " << lastColumn << " -> next: " << nextmovedColumn;
                lastRow = nextmovedRow;
                lastColumn = nextmovedColumn;
                std::cout << "\n tilemap wdsa: w" << tilemap[nextmovedRow][nextmovedColumn+1] << ", d: " << tilemap[nextmovedRow+1][nextmovedColumn]<< ", s: " << tilemap[nextmovedRow][nextmovedColumn-1] <<", a: "<< tilemap[nextmovedRow-1][nextmovedColumn];
                std::cout << "\n tilemap row: "<< nextmovedRow << " col: "<< nextmovedColumn;
                std::cout << "\n tilemap we get??: " << tilemap[nextmovedRow][nextmovedColumn] << " playernextpos.x "<< nextPlayerPosition.x << " , y: "<< nextPlayerPosition.y;
                colorAddNeighbours(up,right, south,left, ourInstIndex);
            }; 
    };

    };
    render();
    return true;
};

void Game::render(){
        
    renderer.render();
    
};
Game::~Game(){};