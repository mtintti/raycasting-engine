
#include "Game.h"
#include "Renderer.h"
#include <iostream>
#include "gobals.h"
#include <algorithm>
#include <math.h>
#include "glm/ext/matrix_transform.hpp"

glm::vec3 nextPlayerPosition;
glm::vec2 playerTranslations[6];
glm::vec3 playerPosition;
// glm::vec2 sightline;

// const/dest
Game::Game(int window_w, int window_h, int mazeSize, int gameSize)
    : renderer(window_w, window_h), image(w * h)
{
    /*std::cout << "Game initialized with window size..." << std::endl;
    std::cout << "game projection initialized with size " << gameSize << " x " << gameSize << "\n";
    std::cout << "Game initialized with maze size " << mazeSize << " x " << mazeSize << std::endl;*/
    maze.creation(mazeSize);
    // image[w*h];
    if (tilemapDone == true)
    {
        init();
    };
}

void Game::init()
{
    //std::cout << "in game.init() where entities are done";

    //std::cout << "game window done -> next data init ";
    playerinit();
    renderer.init();
};

void Game::playerinit()
{
    int indNum = 0;
    camera = {0, 0, 0};
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            indNum++;
            if (tilemap[i][j] == 2)
            {
                //std::cout << "\n tilemap 2/start " << tilemap[i][j] << " found in " << i << " , " << j;
                //std::cout << "\n this makes our camera/player pos " << i + 1 << " , " << j << " in " << tilemap[i + 1][j];
                camera = {i + 1, j, 0};
                //std::cout << "\n camera vec3 = " << camera.x << "," << camera.y << "," << camera.z;
                //std::cout << "\n index for transformations " << indNum;
                break;
            }
        }
    }

    int playerRow = camera.x;
    int playerCol = camera.y;
    origin = {0.0f, 0.0f, 0.0f};
    deltax = cos(angle) * 5;
    deltay = sin(angle) * 5;

    // new
    playersize = 2.0f / row;
    float offset = playersize * 0.3f;

    // sama logiikka kuin maze translatiossa. -1 ja +1 koska opengl ndc koordinaatti muutos. kerrotaan pelaajan koolla ja laitetaan offset hitbox:iksi
    playerPosition.x = -1.0f + playerCol * playersize + offset;
    playerPosition.y = 1.0f - playerRow * playersize - offset;
    playerPosition.z = 0;

    /*std::cout << "\nPlayer world pos: "
              << playerPosition.x << ", "
              << playerPosition.y << "\n";*/

    lastRow = playerRow;
    lastColumn = playerCol;

    int instIndex = playerRow * column + playerCol;
    colors[instIndex] = {1.0f, 0.0f, 0.0f}; // debug red
    renderer.updateColors(colors);
    renderer.playerInit();
    renderer.sightlineInit();
    buildVerticesQuad();
};
int debug = 0;
int ind = 0;

void Game::colorAddNeighbours(float up, float right, float south, float left, int ourInstIndex)
{
    if (up == 1 || right == 1 || south == 1 || left == 1)
    {
        c = {0.1, 0.5, 0.5};
        colors[ourInstIndex] = c;
        //std::cout << "\n index of color after change " << ourInstIndex << " in: " << colors[ourInstIndex].r << " , " << colors[ourInstIndex].b << " , " << colors[ourInstIndex].g;
    };
    renderer.updateColors(colors);
};
bool renderProjection = true;

void Game::buildVerticesQuad()
{
    // fills the global verticesQuad[2 * w * h] with NDC grid positions
    // matching the image[] pixel layout (row-major, top-left = (-1, 1))
    int i = 0;
    for (int py = 0; py < h; py++)
    {
        for (int px = 0; px < w; px++)
        {
            // map px in [0, w) → NDC x in [-1, 1]
            verticesQuad[2*i]     =  (2.0f * px / (float)(w - 1)) - 1.0f;
            // map py in [0, h) → NDC y in [1, -1]  (flip Y so row 0 = top)
            verticesQuad[2*i + 1] =  1.0f - (2.0f * py / (float)(h - 1));
            i++;
        }
    }
}

bool Game::update(double dt)
{
    glm::vec3 nextPlayerPosition;

    nextPlayerPosition.x = playerPosition.x;
    nextPlayerPosition.y = playerPosition.y;
    nextPlayerPosition.z = playerPosition.z;
    float playerSpeed = 0.05f;
    // katsotaan jos keyboard inputti tulee ja lisätään hieman y tai x liikkumista
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        nextPlayerPosition.y += 0.005f;
        // sightlinedirection += 0.005f;
        origin += playerSpeed * cameraDirection;
    };
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        nextPlayerPosition.x += 0.005f;
        // sightlinedirection += 0.005f;
        origin += playerSpeed * glm::normalize(glm::cross(cameraDirection, cameraUp));
    };
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        nextPlayerPosition.y -= 0.005f;
        // sightlinedirection -= 0.005f;
        origin -= playerSpeed * cameraDirection;
    };
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        nextPlayerPosition.x -= 0.005f;
        // sightlinedirection -= 0.005f;
        // origin -= glm::normalize(glm::cross(cameraDirection, cameraUp));
        origin -= playerSpeed * glm::cross(cameraDirection, cameraUp);
    };

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    // glfwSetMousePos(window_w/2, window_h/2);
    currentPos.x = xpos;
    currentPos.y = ypos;

    if (previousPos.x > 0 && previousPos.y > 0)
    {
        double x = currentPos.x - previousPos.x;
        double y = currentPos.y - previousPos.y;
        /*bool rotateX = x != 0;
        bool rotateY = y != 0;
        if(rotateX){
            sightlinedirection.x = x;
            angle -= 0.1;
            if(angle < 0){
                angle += 2 * pi;
                deltax = cos(angle) * 5;
                deltay = sin(angle) * 5;
            };
        };
        if(rotateY){
            sightlinedirection.y = y;
            angle += 0.1;
            if(angle > 2*pi){
                angle -= 2 * pi;
                deltax = cos(angle) * 5;
                deltay = sin(angle) * 5;
            };
        };*/
        double dx = currentPos.x - previousPos.x;
        playerAngle += dx * 0.002;

        if (playerAngle < 0)
            playerAngle += 2 * pi;
        if (playerAngle > 2 * pi)
            playerAngle -= 2 * pi;
        //std::cout << "\n playerAngle: " << playerAngle;
    };
    previousPos.x = currentPos.x;
    previousPos.y = currentPos.y;
    //projection();

    // std::cout <<"\n mous pos: x" << previousPos.x << " , y " << previousPos.y;

    // opengl lookaatio taas, jos seuraava column tai row plus/miinus muunnos.
    // -1 +1 playerPosition numero muutetaan screen koordinaatiksi. 0.5f puolittaa saadun numeron ja sitten scalataan mazen koolla, 21.
    nextmovedColumn = int((nextPlayerPosition.x + 1.0f) * 0.5f * column);
    nextmovedRow = int((1.0f - (nextPlayerPosition.y + 1.0f) * 0.5f) * row);

    if (nextmovedRow >= 0 && nextmovedRow < row && nextmovedColumn >= 0 && nextmovedColumn < column)
    {
        if (tilemap[nextmovedRow][nextmovedColumn] == 1)
        {

            int ourInstIndex = nextmovedRow * column + nextmovedColumn;
            // otetaan koordinaatit talteen jotta saadaan värit missä ollaan käyty.
            float up = tilemap[nextmovedRow][nextmovedColumn + 1];
            float right = tilemap[nextmovedRow + 1][nextmovedColumn];
            float south = tilemap[nextmovedRow][nextmovedColumn - 1];
            float left = tilemap[nextmovedRow - 1][nextmovedColumn];

            // sightlines ??
            /*
             int cellSize = (window_h - 20) / tilemap[0].size();
             int raysNumber = 60;
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
            // debug test sightline end here

            playerPosition = nextPlayerPosition; // jos ollaan reitillä niin liikutaan sauraavaan tileen
            if (nextmovedRow != lastRow || nextmovedColumn != lastColumn)
            {
                // debug aina kun liikutaan seuraavaan celliin.
                /*std::cout << "\n lastrow: " << lastRow << " -> next: " << nextmovedRow;
                std::cout << "\n lastcolumn: " << lastColumn << " -> next: " << nextmovedColumn;*/
                lastRow = nextmovedRow;
                lastColumn = nextmovedColumn;
                renderProjection = true;
                /*std::cout << "\n tilemap wdsa: w" << tilemap[nextmovedRow][nextmovedColumn + 1] << ", d: " << tilemap[nextmovedRow + 1][nextmovedColumn] << ", s: " << tilemap[nextmovedRow][nextmovedColumn - 1] << ", a: " << tilemap[nextmovedRow - 1][nextmovedColumn];
                std::cout << "\n tilemap row: " << nextmovedRow << " col: " << nextmovedColumn;
                std::cout << "\n tilemap we get??: " << tilemap[nextmovedRow][nextmovedColumn] << " playernextpos.x " << nextPlayerPosition.x << " , y: " << nextPlayerPosition.y;
                std::cout << "\n image size: " << image.size();
                std::cout << "\n image : " << image[500].red << " " << image[500].green << " " << image[500].blue;
                std::cout << "\n neighbours: " << up << " " << left << " " << south << " " << left;*/
                colorAddNeighbours(up, right, south, left, ourInstIndex);
                // projection();
                renderer.GenerateQuadForRay(image);
            };
        };
    };

    render();
    return true;
};
int colorpixeldebug = 0;

void Game::setpixel(int x, int y, float r, float g, float b)
{
    int ourRayColorIndex = y * w + x;

    // std::cout << "\n setPixel val: "<< r << " , " << g << " , " << b;
    image[ourRayColorIndex].red = r;
    image[ourRayColorIndex].green = g;
    image[ourRayColorIndex].blue = b;

    colorpixeldebug++;
};

bool projectionisDone = false;
int hitted = 0;
int missed = 0;
float Game::distance(float ax, float ay, float bx, float by, float ang)
{
    return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
};

void normalizeAngle(float &angle)
{
    float pi = 3.1415926535;
    while (angle < 0)
        angle += 2 * pi;
    while (angle > 2 * pi)
        angle -= 2 * pi;
}

void Game::projection()
{

    // uus sightlines ja oikea projection oikealle puolelle screeniä
    const int image_width  = w; 
    const int image_height = h;

    int   numRays     = image_width;         
    float fov         = 0.785f * 2.0f;        
    float halfFov     = fov * 0.5f;
    float maxRayLen   = 3.0f;
    float stepSize    = 0.005f;               

    
    sightline.clear();
    sightline.reserve(numRays * 2);

    
    for (int col = 0; col < numRays; col++)
    {
        
        float t         = (float)col / (float)(numRays - 1);   // 0..1
        float rayAngle  = playerAngle - halfFov + t * fov;

        
        while (rayAngle < 0)       rayAngle += 2.0f * pi;
        while (rayAngle > 2.0f*pi) rayAngle -= 2.0f * pi;

        glm::vec2 pos   = glm::vec2(playerPosition.x, playerPosition.y);
        glm::vec2 dir   = glm::vec2(std::cos(rayAngle), std::sin(rayAngle));
        glm::vec2 point = pos;
        float traveled  = 0.0f;
        bool  hit       = false;

        while (traveled < maxRayLen)
        {
            point.x += dir.x * stepSize;
            point.y += dir.y * stepSize;
            traveled += stepSize;

            // NDC → tile index  (same formula kuin muualla projektissa)
            int tileX = int((point.x + 1.0f) * 0.5f * column);
            int tileY = int((1.0f - (point.y + 1.0f) * 0.5f) * row);

            if (tileX < 0 || tileX >= column || tileY < 0 || tileY >= row)
                break;

                // seinä löydetty
            if (tilemap[tileY][tileX] == 0)  
            {
                hit = true;
                break;
            }
        }

        sightline.push_back(pos);
        sightline.push_back(point);

        float angleDiff = rayAngle - playerAngle;
        while (angleDiff < -pi) angleDiff += 0.2f * pi; // was 2.0, 0.5
        while (angleDiff >  pi) angleDiff -= 0.2f * pi;
        float perpDist  = hit ? (traveled * std::cos(angleDiff)) : maxRayLen;
        perpDist        = std::max(perpDist, 0.001f);   // guard div/0

        float wallHeight  = (1.0f / perpDist) * (image_height * 0.3f); // was 1.0f, all *
        int   wallTop     = (int)((image_height * 0.3f) - wallHeight * 0.3f);
        int   wallBottom  = (int)((image_height * 0.3f) + wallHeight * 0.3f);
        wallTop           = std::max(wallTop,    0);
        wallBottom        = std::min(wallBottom, image_height - 1);

        // shade wall määritellään pituudella: lähempänä on kirkkaampaa vihreää
        float shade = std::max(0.0f, 0.4f - perpDist / maxRayLen);

        for (int row_px = 0; row_px < image_height; row_px++)
        {
            if (row_px < wallTop)
            {
                // katto, harmaa
                setpixel(col, row_px, 0.15f, 0.15f, 0.15f);
            }
            else if (row_px <= wallBottom)
            {   // seinä, vihreä
                setpixel(col, row_px, 0.0f, shade, 0.0f);
            }
            else
            {
                // lattia — vähän vaaleampi harmaa kuin katto
                setpixel(col, row_px, 0.25f, 0.25f, 0.25f);
            }
        }
    }
};

void Game::render()
{
    projection();
    renderer.GenerateQuadForRay(image);
    renderer.updateSightline(sightline);
    renderer.render(image, sightline);
};
Game::~Game() {};