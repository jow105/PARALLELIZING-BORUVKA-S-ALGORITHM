#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CycleTimer.h"
#include <cstdlib>
#include <algorithm>
using namespace std;

struct edge
{
	int from_vertex;
	int to_vertex;
	int weight;
};

typedef struct edge edge;

typedef vector<edge> Vector;

struct Graph
{
	int num_vertices;
	int num_edges;

	Vector* adjacency;
};

typedef struct Graph Graph;

Graph* init_graph(int num_vertices, int num_edges);

Graph* graph_insert(Graph* G, int from_vertex, edge e);

Graph* read_graph_input(string file_name);

void print_graph(Graph* G);