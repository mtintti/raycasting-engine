
#include <iostream>
#include <random>
#include "Maze.h"
#include <vector>
#include "gobals.h"

Maze::Maze() {};
Maze::~Maze() {};

std::vector<std::vector<int>> tilemap;

void Maze::creation(int mazeSize)
{
    std::cout << "in Maze.creation()";
    //int row = 21; // orig
    //int column = 21; // origi
    row = 21;
    column = 21;
    bool gone = false;
    //std::vector<std::vector<int>> tilemap(row, std::vector<int>(column, 0));
    //tilemap = std::vector<std::vector<int>>(row, std::vector<int>(column, 0)); 
    tilemap.assign(row, std::vector<int>(column, 0));


    std::cout << "in Maze.creation() past vector.. \n";
    /*for(int firstrow = 0; firstrow <= row; firstrow++){
        std::cout << firstrow << " ";
    }*/
    const int first = 1;    // past
    const int rangeTo = 20; // past
    // const int first = 1;
    // const int rangeTo = 19;
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

            // tilemap[i][j]
            // tilemap.push_back[i][j];
            std::cout << tilemap[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "--start/end set --\n";
    // prims maze
    // int N = 1, S= 2, E = 3, W = 8;
    std::vector<std::vector<int>> possiblePoints(0, std::vector<int>(2));

    /*for(int j = start; j < column + 1; j++){
        std::cout << "we are in (0, " << j<< " )" << " start was : (0, " << start << " )";
    }*/
    struct Neighbors
    {
        int nRow, nColumn;
        int sRow, sColumn;
        int wRow, wColumn;
        int eRow, eColumn;
    };
    // mine int offx[] = {0, 1, 0, -1};
    // mine int offy[] = {-1, 0, 1, 0};

    // int x = 1;
    // int y = start;
    // int nx, ny = 0;
    int prevx, prevy = 0;
    int count = 0;
    // possiblePoints.push_back({x, y});

    // Offsets for moving in four directions
    int offx[] = {0, 2, 0, -2};
    int offy[] = {-2, 0, 2, 0};

    // Start inside the maze (just below start point)
    int x = 1;
    int y = start;
    tilemap[x][y] = 1;

    std::vector<std::vector<int>> walls;

    // Add surrounding walls (2-step away)
    for (int k = 0; k < 4; k++)
    {
        int nx = x + offx[k];
        int ny = y + offy[k];
        if (nx > 0 && nx < row -1 && ny > 0 && ny < column -1)
        // if (nx >= 1 && nx <= row - 2 && ny >= 1 && ny <= column - 2)

        {
            walls.push_back({nx, ny});
            tilemap[nx][ny] = 4; // mark as frontier
        }
    }

    std::srand(std::time(nullptr));

    while (!walls.empty())
    {
        // Pick a random wall
        int r = std::rand() % walls.size();
        int wallx = walls[r][0];
        int wally = walls[r][1];
        walls.erase(walls.begin() + r);

        // Check if this wall has a path cell two steps away
        std::vector<std::pair<int, int>> naapurit;
        for (int k = 0; k < 4; k++)
        {
            int nx = wallx + offx[k];
            int ny = wally + offy[k];
            // if (nx > 0 && nx < row -1 && ny > 0 && ny < column -1)

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
            // Pick random neighbor that is a passage
            auto neighbor = naapurit[std::rand() % naapurit.size()];
            int nx = neighbor.first;
            int ny = neighbor.second;

            // Find cell between wall and passage
            int midX = (wallx + nx) / 2;
            int midY = (wally + ny) / 2;

            // Make passage through wall
            tilemap[wallx][wally] = 1;
            tilemap[midX][midY] = 1;

            for (int k = 0; k < 4; k++)
            {
                int frontierx = wallx + offx[k];
                int frontiery = wally + offy[k];
                // if (frontierx > 0 && frontierx < row -1 && frontiery > 0 && frontiery < column -1)

                if (frontierx > 0 && frontierx < row-1 && frontiery > 0 && frontiery < column-1)
                {
                    // for (int frontierx = 2; frontierx < 3; frontierx++) {
                    // for (int frontiery = 0; frontiery < column; frontiery++){
                    //   std::cout << "\n in mat: "<< frontierx << ", " << frontiery;
                    //}

                     if (tilemap[frontierx][frontiery] == 0)
                    {
                    walls.push_back({frontierx, frontiery});
                    tilemap[frontierx][frontiery] = 4;
                    }
                }
            }
        }
    }

    // Reset frontier marks (4) to walls (0)
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            if (tilemap[i][j] == 4)
                tilemap[i][j] = 0;

    // Place end point again (in case overwritten)
    tilemap[row - 1][end] = 3;

    // Print maze to terminal
    std::cout << "\nGenerated Maze:\n";
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            std::cout << tilemap[i][j] << " ";
        }
        std::cout << "\n";
    }
}


/*while (!possiblePoints.empty())
{
    // pick a random frontier cell
    int r = rand() % possiblePoints.size();
    nx = possiblePoints[r][0];
    ny = possiblePoints[r][1];

    tilemap[nx][ny] = 5; // mark as part of the path

    // explore neighbors of this cell
    for (int k = 0; k < 4; k++)
    {
        int nextx = nx + offx[k];
        int nexty = ny + offy[k];

        if (nextx >= 1 && nextx < row - 1 && nexty >= 1 && nexty < column - 1)
        {
            if (tilemap[nextx][nexty] == 0)
            {                              // unexplored
                tilemap[nextx][nexty] = 4; // mark as frontier
                possiblePoints.push_back({nextx, nexty});
            }
        }
    }
    // remove used cell from frontier
    possiblePoints.erase(possiblePoints.begin() + r);
}
for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){

            // tilemap[i][j]
            // tilemap.push_back[i][j];
            std::cout << tilemap[i][j] << " ";
        }
        std::cout << "\n";
    }*/

// emd for possible points not zero line, okey..

/*for (int i = 0; i < row; i++) // / *
{
    for (int j = start; j < column; j++)
    {
        std::cout << "In mat = (" << i << " , " << j << " ) \n";
        std::cout << "start is: " << 0 << " , " << start;
        // int N = row -1;
        // possiblePoints.insert(row - 1, row +1, column -1, column +1);
        for (int n = 0; n < 4; n++) {
            int nx = i + offx[n];
            int ny = j + offy[n];
            //tilemap[nx][ny] = 4;
            //tilemap[i][j] = 5;
            if(nx >= 0 && nx < row && ny >= 0 && ny < column){
            std::cout << "\n neighbors = (" << nx << " , " << ny << " )";
            tilemap[nx][ny] = 5;

            //tilemap[i][j] = 5;
            };
        };

        if (count == 2)
        {
            break;
        };
        count++;
    };
}; */
// / * new try - ⌄⌄

// std::vector<int, int> frontier;
/*for(int x = -1; x <=1; x++){
    for(int y = -1; y <= 1; y++){
        if(x== 0 && y == 0 || x != 0 && y != 0){
            continue;
        } try {
            if(tilemap[i + x])
        }
    }
}*/

//    for (int i = 0; i < row; i++)
//    {
//        for (int j = 0; j < column; j++)
//        {

//        }
//    }
