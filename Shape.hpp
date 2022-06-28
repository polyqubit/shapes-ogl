#pragma once
#include "Utils.h"

struct Cube {
    const Vertex VERTICES[8] =
    {
      { { -.75f, -.75f,  .75f, 1 }, { 1, 0.5, 0.5,    1 } },
      { { -.75f,  .75f,  .75f, 1 }, { 0.5, 1, 0.5,    1 } },
      { {  .75f,  .75f,  .75f, 1 }, { 0.5, 0.5, 1,    1 } },
      { {  .75f, -.75f,  .75f, 1 }, { 1, 1, 0.5,      1 } },
      { { -.75f, -.75f, -.75f, 1 }, { 1, 0.5, 0.5,    1 } },
      { { -.75f,  .75f, -.75f, 1 }, { 0.5, 1, 0.5,    1 } },
      { {  .75f,  .75f, -.75f, 1 }, { 0.5, 0.5, 1,    1 } },
      { {  .75f, -.75f, -.75f, 1 }, { 1, 1, 0.5,      1 } }
    };
    const GLuint INDICES[36] =
    {
      0,2,1,  0,3,2,
      4,3,0,  4,7,3,
      4,1,5,  4,0,1,
      3,6,2,  3,7,6,
      1,6,5,  1,2,6,
      7,5,6,  7,4,5
    };
} cubeStruct;

struct TriPrism {
	const Vertex VERTICES[6] =
	{	
		{ { 0, 1, -.75,          1 }, { 1, 0.5, 0.5,    1 } },
		{ { .866f, -.5f, -.75,   1 }, { 0.5, 1, 0.5,    1 } },
		{ { -.866f, -.5f, -.75,  1 }, { 0.5, 0.5, 1,    1 } },
		{ { 0, 1, .75,           1 }, { 1, 0.5, 0.5,    1 } },
		{ { .866f, -.5f, .75,    1 }, { 0.5, 1, 0.5,    1 } },
		{ { -.866f, -.5f, .75 ,  1 }, { 0.5, 0.5, 1,    1 } }
	};

	const GLuint INDICES[24] =
	{
	  0,1,2,
	  0,3,4,
	  0,4,1,
	  0,5,3,
	  0,2,5,
	  1,5,2,
	  1,4,5,
	  3,5,4
	};
} tpStruct;