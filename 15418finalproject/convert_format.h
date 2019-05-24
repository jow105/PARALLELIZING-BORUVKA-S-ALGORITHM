// convert_format.h
// header file for convert_format.cpp

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

struct linked_list
{
	edge adj;
	struct linked_list *next;
};

typedef struct linked_list LL;

struct Graph
{
	int num_vertices;
	int num_edges;

	LL** adjacency;
};

typedef struct Graph Graph;

Graph* init_graph(int num_vertices, int num_edges);

Graph* graph_insert(Graph* G, int from_vertex, edge e);

Graph* read_graph_input(string file_name);

void print_graph(Graph* G);