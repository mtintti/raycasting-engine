#include <vector>
#ifndef HEADERS_H_DECLARED
#define HEADERS_H_DECLARED
extern std::vector<std::vector<int>> tilemap;

class Maze
{

private:
public:
    Maze();
    virtual ~Maze();
    void creation(int mazeSize);

    int row = 21;
    int column = 21;
    

};
#endif

