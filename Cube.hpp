#pragma once
#include "Utils.h"

enum class ROTenum { // rotation enum

};

class Cube
{
public: 
	Cube();
	~Cube();
	void create(Vertex v[8]);
    void transform(ROTenum r, float angle);
    void draw();
    void destroy();
private:
    const GLuint INDICES[36] =
    {
      0,2,1,  0,3,2,
      4,3,0,  4,7,3,
      4,1,5,  4,0,1,
      3,6,2,  3,7,6,
      1,6,5,  1,2,6,
      7,5,6,  7,4,5
    };
};

