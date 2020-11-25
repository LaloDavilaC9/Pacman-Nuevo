#pragma once

#include <vector>
#include <iostream>

using namespace std;

struct Nodo {
	int posX, posY, id;
};

struct Edge {
	Nodo inicio, fin;
	float distancia;
};

