#include <vector>
#include <glm.hpp> 
#ifndef HEADERS_H_DECLARED
#define HEADERS_H_DECLARED
extern std::vector<std::vector<int>> tilemap;
extern glm::vec3 colors[441];
extern glm::vec3 c;
extern glm::vec2 translations[441];
extern glm::vec2 t;

extern int row;
extern int column;

class Maze
{

private:
public:
    Maze();
    virtual ~Maze();
    void creation(int mazeSize);

    void normalizing();

    
    

};
#endif

