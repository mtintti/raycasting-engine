
#include <iostream>
#include <random>
#include "Maze.h"
#include <vector>
#include "gobals.h"
#include <glm.hpp> 

Maze::Maze() {};
Maze::~Maze() {};

std::vector<std::vector<int>> tilemap;
glm::vec3 colors[441];
glm::vec3 c;
glm::vec2 translations[441];
bool tilemapDone = false;
int row = 21;
int column = 21;


void Maze::creation(int mazeSize)
{
    std::cout << "in Maze.creation()";
    bool gone = false;
    tilemap.assign(row, std::vector<int>(column, 0));

    std::cout << "in Maze.creation() past vector.. \n";
    const int first = 1; 
    const int rangeTo = 20;

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(first, rangeTo);
    int start = distr(generator);
    tilemap[0][start] = 2;
    std::cout << " maze start: (0, " << start << ")\n";
    int end = distr(generator);
    tilemap[row - 1][end] = 3;
    std::cout << " maze end: (" << row - 1 << ", " << end << ")\n";

    std::cout << " .. and start/end set \n";
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            std::cout << tilemap[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "--start/end set --\n";
    std::cout<< "tilemap is done?? "<<tilemapDone;
    
    std::vector<std::vector<int>> possiblePoints(0, std::vector<int>(2));



    int offx[] = {0, 2, 0, -2}; // liikutaan x
    int offy[] = {-2, 0, 2, 0}; // liikutaan y

    
    int x = 1;
    int y = start;
    tilemap[x][y] = 1;

    std::vector<std::vector<int>> walls;

    // lisätään kaksi askeleen päässä olevat
    for (int k = 0; k < 4; k++)
    {
        int nx = x + offx[k];
        int ny = y + offy[k];
        if (nx > 0 && nx < row -1 && ny > 0 && ny < column -1)

        {
            walls.push_back({nx, ny});
            tilemap[nx][ny] = 4;
        }
    }

    std::srand(std::time(nullptr));

    while (!walls.empty())
    {
        // otetaan randomi seinä
        int r = std::rand() % walls.size();
        int wallx = walls[r][0];
        int wally = walls[r][1];
        walls.erase(walls.begin() + r);

        // katsotaan jos sillä on naapuri cell lähdellä
        std::vector<std::pair<int, int>> naapurit;
        for (int k = 0; k < 4; k++)
        {
            int nx = wallx + offx[k];
            int ny = wally + offy[k];

            if (nx >= 0 && nx < row && ny >= 0 && ny < column)
            {
                if (tilemap[nx][ny] == 1)
                {
                    naapurit.push_back({nx, ny});
                }
            }
        }

        if (!naapurit.empty())
        {
            // otateen randomi naapuri
            auto neighbor = naapurit[std::rand() % naapurit.size()];
            int nx = neighbor.first;
            int ny = neighbor.second;

            // löydetään keskiosa seinästä ja reitistä
            int midX = (wallx + nx) / 2;
            int midY = (wally + ny) / 2;

            // ja tehdään reitti seinästä
            tilemap[wallx][wally] = 1;
            tilemap[midX][midY] = 1;

            for (int k = 0; k < 4; k++)
            {
                int frontierx = wallx + offx[k];
                int frontiery = wally + offy[k];

                if (frontierx > 0 && frontierx < row-1 && frontiery > 0 && frontiery < column-1)
                {
                    

                     if (tilemap[frontierx][frontiery] == 0)
                    {
                    walls.push_back({frontierx, frontiery});
                    tilemap[frontierx][frontiery] = 4;
                    }
                }
            }
        }
    }

    // laitetaan frontier merkit -> 0
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            if (tilemap[i][j] == 4)
                tilemap[i][j] = 0;

    
    tilemap[row - 1][end] = 3;


    std::cout << "\nGenerated Maze:\n";     
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {       
            std::cout << tilemap[i][j] << " ";   
        }
        std::cout << "\n";
        }    
  
    // laitetaan tilemap value numeron mukaan oikeat värit colors indexin numeron paikalle
    // haetaan myöhemmin colors index numerolla translate paikat jotta saadaan värit screeniin 
    for(int i = 0; i < row; i++){
        for(int j = 0; j < column; j++){
            if(tilemap[i][j] == 0){
                c = {0.180, 0.180, 0.180};
            };
            if (tilemap[i][j] == 1) {
                c = {0.0f, 1.0f, 0.0f};
            };
            if (tilemap[i][j] == 2) {
                c = {0.0f, 0.0f, 1.0f}; // pink
                 
            };
            if (tilemap[i][j] == 3){
                c = {1.0f, 1.0f, 0.0f}; // yellow
                
            };
            int ourInstIndex = i * column + j;
            colors[ourInstIndex] = c;
        };
    };

    // tehdään translations vertex pisteet. neilöt on tehty kahdesta kolmiosta, yhdessä neliössä on 6 pistettä.
    // aloitetaan pisteiden koordinaatio ylävasemmassta kohdasta eli -1 ja +1 opengl koordinaatti muutoksessa.
    int indx = 0;
    float offset = 0.05f;
    for(int y = 21; y > -21; y -= 2){
        for(int x = -21; x < +21; x += 2){
            glm::vec2 t;
            t.x = (float)x / 21.0f + offset; // x numero jaetaan tilemapin pituudella ja lisätään offsetti jotta ei tungosta
            t.y = (float)y / 21.0f - offset;
            translations[indx++] = t;
        };
    };
    std::cout <<"\n translations are done!";
    

    tilemapDone = true;
    std::cout<< "\n tilemap is done?? "<<tilemapDone;
}
