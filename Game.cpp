
#include "Game.h"
#include "Renderer.h"
#include <iostream>
#include "gobals.h"
#include <algorithm>
#include <cmath>



glm::vec3 nextPlayerPosition;
glm::vec2 playerTranslations[6];
glm::vec3 playerPosition;
//glm::vec2 sightline;


//const/dest
Game::Game(int window_w, int window_h, int mazeSize, int gameSize) 
:renderer(window_w, window_h), image(w*h)
{
    std::cout << "Game initialized with window size..."  << std::endl;
    std::cout << "game projection initialized with size " << gameSize<< " x " <<gameSize << "\n";
    std::cout << "Game initialized with maze size " << mazeSize << " x " << mazeSize << std::endl;
    maze.creation(mazeSize);
    //image[w*h];
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
    float playersize = 2.0f / row ;
    float offset = playersize * 0.3f; 

    // sama logiikka kuin maze translatiossa. -1 ja +1 koska opengl ndc koordinaatti muutos. kerrotaan pelaajan koolla ja laitetaan offset hitbox:iksi
    playerPosition.x = -1.0f + playerCol * playersize + offset;
    playerPosition.y = 1.0f -playerRow * playersize -offset;
    playerPosition.z = 0;


    std::cout << "\nPlayer world pos: "
              << playerPosition.x << ", "
              << playerPosition.y << "\n";

    lastRow = playerRow;
    lastColumn = playerCol;

    int instIndex = playerRow * column + playerCol;
    colors[instIndex] = {1.0f, 0.0f, 0.0f}; // debug red
    renderer.updateColors(colors);
    renderer.playerInit();
    renderer.sightlineInit();

};
int debug = 0;
int ind = 0;


void Game::colorAddNeighbours(float up, float right, float south, float left, int ourInstIndex){
    if(up ==1 || right == 1 || south ==1 || left == 1){
        c = {0.1, 0.5, 0.5};
        colors[ourInstIndex] = c;
        std::cout <<"\n index of color after change "<< ourInstIndex<< " in: " << colors[ourInstIndex].r << " , "<< colors[ourInstIndex].b<< " , "<< colors[ourInstIndex].g;
    };
    renderer.updateColors(colors);
};
bool renderProjection = true;


bool Game::update(double dt){
    glm::vec3 nextPlayerPosition;
    
    nextPlayerPosition.x = playerPosition.x;
    nextPlayerPosition.y = playerPosition.y;
    nextPlayerPosition.z = playerPosition.z;

    // katsotaan jos keyboard inputti tulee ja lisätään hieman y tai x liikkumista
    if(glfwGetKey(window, GLFW_KEY_W)== GLFW_PRESS){
        nextPlayerPosition.y += 0.005f;
        //sightlinedirection += 0.005f;
    };
    if(glfwGetKey(window, GLFW_KEY_D)== GLFW_PRESS){
        nextPlayerPosition.x += 0.005f;
        //sightlinedirection += 0.005f;
    };
    if(glfwGetKey(window, GLFW_KEY_S)== GLFW_PRESS){
        nextPlayerPosition.y -= 0.005f;
        //sightlinedirection -= 0.005f;
    };
    if(glfwGetKey(window, GLFW_KEY_A)== GLFW_PRESS){
        nextPlayerPosition.x -= 0.005f;
        //sightlinedirection -= 0.005f;
    };

    double xpos, ypos;
    glfwGetCursorPos( window, &xpos, &ypos);
    //glfwSetMousePos(window_w/2, window_h/2);
    currentPos.x = xpos;
    currentPos.y = ypos;



    if(previousPos.x > 0 && previousPos.y > 0){
        double x = currentPos.x - previousPos.x;
        double y = currentPos.y - previousPos.y;
        bool rotateX = x != 0;
        bool rotateY = y != 0;
        if(rotateX){
            sightlinedirection.x = x;
        };
        if(rotateY){
            sightlinedirection.y = y;
        };

    };
    previousPos.x = currentPos.x;
    previousPos.y = currentPos.y;
    projection();
    //std::cout <<"\n mous pos: x" << previousPos.x << " , y " << previousPos.y;


    // opengl lookaatio taas, jos seuraava column tai row plus/miinus muunnos.
    // -1 +1 playerPosition numero muutetaan screen koordinaatiksi. 0.5f puolittaa saadun numeron ja sitten scalataan mazen koolla, 21.
    nextmovedColumn = int((nextPlayerPosition.x + 1.0f) * 0.5f * column);
    nextmovedRow = int((1.0f- (nextPlayerPosition.y + 1.0f) *0.5f) *row);
    

    if(nextmovedRow >= 0 && nextmovedRow < row && nextmovedColumn >= 0 && nextmovedColumn < column){
        if(tilemap[nextmovedRow][nextmovedColumn] == 1){
            
            
            int ourInstIndex = nextmovedRow * column + nextmovedColumn;
            // otetaan koordinaatit talteen jotta saadaan värit missä ollaan käyty.
            float up =  tilemap[nextmovedRow][nextmovedColumn+1];
            float right = tilemap[nextmovedRow+1][nextmovedColumn];
            float south = tilemap[nextmovedRow][nextmovedColumn-1];
            float left =  tilemap[nextmovedRow-1][nextmovedColumn];

           /*
            int cellSize = (window_h - 20) / tilemap[0].size();
            int raysNumber = 10;
            float spreadedOut = 0.785f; // pi 3.14159 / 4 = spreadedOut
            float maxRayLength = 1.5f;   // world units
            float stepSize     = 0.02f;  // ray marching precision
            
            
            sightline.reserve(raysNumber);
            
            for(int i = 0; i < raysNumber; i++){
             if(sightline.size() > raysNumber *3){
                sightline.clear();
            };
            float offset = ((i / (float)(raysNumber - 1)) -0.5f) * spreadedOut * 2.0f; // 
            float raydirection = sightlinedirection.x + sightlinedirection.y + offset;

            glm::vec2 startPoint = playerPosition;
            glm::vec2 raysPoint = startPoint;
            
            //std::cout << "\n sight x tile: " << lastRow;
            //std::cout << "\n sight y tile: " << lastColumn;
            //std::cout << "\n last row x and last col y: " << tilemap[nextmovedRow][nextmovedColumn];
            
            float traveled = 0.0f;

            while (traveled < maxRayLength) {
                raysPoint.x += std::cos(raydirection) * stepSize;
                raysPoint.y += std::sin(raydirection) * stepSize;
                traveled += stepSize;

                // worldistä → tile muutto
                int tileX = int((raysPoint.x + 1.0f) * 0.5f * column);
                int tileY = int((1.0f - (raysPoint.y + 1.0f) * 0.5f) * row);
                


                if (tileX < 0 || tileX >= column || tileY < 0 || tileY >= row)
                    break;

                if (tilemap[tileY][tileX] == 0)
                    break;
            }


            sightline.push_back(startPoint);
            sightline.push_back(raysPoint);
            //std::cout << "\n size of sightline: " << sightline.size();
        };*/
            
           
            playerPosition = nextPlayerPosition; // jos ollaan reitillä niin liikutaan sauraavaan tileen
            if(nextmovedRow != lastRow || nextmovedColumn != lastColumn){
                //debug aina kun liikutaan seuraavaan celliin.
                std::cout << "\n lastrow: " << lastRow << " -> next: " << nextmovedRow;
                std::cout << "\n lastcolumn: " << lastColumn << " -> next: " << nextmovedColumn;
                lastRow = nextmovedRow;
                lastColumn = nextmovedColumn;
                renderProjection = true;
                std::cout << "\n tilemap wdsa: w" << tilemap[nextmovedRow][nextmovedColumn+1] << ", d: " << tilemap[nextmovedRow+1][nextmovedColumn]<< ", s: " << tilemap[nextmovedRow][nextmovedColumn-1] <<", a: "<< tilemap[nextmovedRow-1][nextmovedColumn];
                std::cout << "\n tilemap row: "<< nextmovedRow << " col: "<< nextmovedColumn;
                std::cout << "\n tilemap we get??: " << tilemap[nextmovedRow][nextmovedColumn] << " playernextpos.x "<< nextPlayerPosition.x << " , y: "<< nextPlayerPosition.y;
                std::cout << "\n image size: " << image.size();
                std::cout << "\n image : " << image[500].red << " " << image[500].green << " " << image[500].blue;
                std::cout << "\n neighbours: " << up << " " << left << " " << south << " " << left;
                colorAddNeighbours(up,right, south,left, ourInstIndex);
                //projection();
                renderer.GenerateQuadForRay(image);
            }; 
            
        };

    };
    
    render();
    return true;
};
int colorpixeldebug = 0;


void Game::setpixel(int x, int y, float r, float g, float b){
    int ourRayColorIndex = y * w + x;

    //std::cout << "\n setPixel val: "<< r << " , " << g << " , " << b;
    image[ourRayColorIndex].red = r;
    image[ourRayColorIndex].green = g;
    image[ourRayColorIndex].blue = b;

    
    colorpixeldebug++;
    if(colorpixeldebug == 30 ){
        
        //std::cout << "\n color rgb val: " <<image[ourRayColorIndex].red << " , " << image[ourRayColorIndex].green << " , " << image[ourRayColorIndex].blue;
        colorpixeldebug = 0;
        //std::cout <<"\n  float discr: " << discriminaatti;
    };

};

bool projectionisDone = false;
int hitted = 0;
int missed = 0;

void Game::projection(){
   
    // if want to use pixel use the for two loops below
    // viewports horz and vert vectors
    int image_width = 800;
    int image_height = 800;
    int projectiondebug = 0;

    int cellSize = (window_h - 20) / tilemap[0].size();
    int raysNumber = 10;
    float spreadedOut = 0.785f; // pi 3.14159 / 4 = spreadedOut
    float maxRayLength = 1.5f;   // world units
    float stepSize     = 0.02f;  // ray marching precision
            
            
    sightline.reserve(raysNumber);
    if(sightline.size() > raysNumber *3){
        sightline.clear();
    };
            
    for(int i = 0; i <= raysNumber; i++){
            
    
    float offset = ((i / (float)(raysNumber - 1)) -0.5f) * spreadedOut * 2.0f; // 
    float raydirection = sightlinedirection.x + sightlinedirection.y + offset;

    glm::vec2 startPoint = playerPosition;
    glm::vec2 raysPoint = startPoint;
            
    //std::cout << "\n sight x tile: " << lastRow;
    //std::cout << "\n sight y tile: " << lastColumn;
    //std::cout << "\n last row x and last col y: " << tilemap[nextmovedRow][nextmovedColumn];
            
    float traveled = 0.0f;

    while (traveled < maxRayLength) {
    raysPoint.x += std::cos(raydirection) * stepSize;
    raysPoint.y += std::sin(raydirection) * stepSize;
    traveled += stepSize;
               
    // worldistä → tile muutto
    int tileX = int((raysPoint.x + 1.0f) * 0.5f * column);
    int tileY = int((1.0f - (raysPoint.y + 1.0f) * 0.5f) * row);
                


        if (tileX < 0 || tileX >= column || tileY < 0 || tileY >= row)
            break;

        if (tilemap[tileY][tileX] == 0)
            break;
        }
        //std::cout << "\n ray num: " << i;
        //std::cout << "\n traveled steps: " << traveled;
        //std::cout << "\n start x and y " << startPoint.x << " " << startPoint.y;
        //std::cout << "\n ray x and y " << raysPoint.x << " " << raysPoint.y;
        sightline.push_back(startPoint);
        sightline.push_back(raysPoint);
        //std::cout << "\n size of sightline: " << sightline.size();
    };

    double xyfact = 1.0 / (image_height / 2.0); //used in ndc screen coords
    int i = 0;
    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            
            //nX = (2.0 * (x + 0.5) / image_width) -1.0;
            nY = (y * xyfact) -1.0; 
            nX = (x * xyfact) -1.0; 
            //nY = 1.0 - (2.0 * (y + 0.5) / image_height);
            verticesQuad[2* i] = nX; // saving our vertex position for invidual points
            verticesQuad[2*i +1] = nY;
            if(i < w*h){
                i++;
            };
            

            glm::vec2 coord = {nX, nY};    
            
        };
    };
    //std::cout << "\n hit: " << hitted;
    //std::cout << "\n missed: " << missed;
   
    projectionisDone = true;

};

void Game::render(){
        
    
    if(renderProjection == true){
       
        projection();
        std::cout << "\n projection true call";
        std::cout << "\n start init renderer quad call";
        renderer.GenerateQuadForRay(image);
        
        renderProjection = false;
    };
    //projection();
    renderer.updateSightline(sightline);
    renderer.render(image, sightline);
    
};
Game::~Game(){};