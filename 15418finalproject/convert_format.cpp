// convert_format.cpp
// helper file to convert text format into adjacency list

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CycleTimer.h"
#include <cstdlib>
#include <algorithm>
#include "convert_format.h"
using namespace std;

Graph* init_graph(int num_vertices, int num_edges)
{
	Graph* G = (Graph*)malloc(sizeof(Graph));

	G->num_vertices = num_vertices;
	G->num_edges = num_edges;

	G->adjacency = (LL**)malloc(sizeof(LL*)*num_vertices);

	for(int i = 0; i < num_vertices; i++)
	{
		(G->adjacency)[i] = NULL;
	}

	return G;
}

Graph* graph_insert(Graph* G, int from_vertex, edge e)
{
	printf("inserting to vertex %d\n", from_vertex);
	LL* iter = (G->adjacency)[from_vertex];

	int to_vertex = e.to_vertex;
	int weight = e.weight;
	
	if(iter == NULL)
	{
		LL* new_node = (LL*)malloc(sizeof(LL));
		(new_node->adj).to_vertex = to_vertex;
		(new_node->adj).from_vertex = from_vertex;
		(new_node->adj).weight = weight;
		new_node->next = NULL;
		iter = new_node;
	}
	else
	{
		while(iter != NULL)
		{
			iter = iter->next;
		}
		LL* new_node = (LL*)malloc(sizeof(LL));
		(new_node->adj).to_vertex = to_vertex;
		(new_node->adj).from_vertex = from_vertex;
		(new_node->adj).weight = weight;
		new_node->next = NULL;
		iter = new_node;
	}

	return G;
}

Graph* read_graph_input(string file_name)
{
	string line;

	ifstream myfile (file_name.c_str());

	if(myfile.is_open())
	{
		std::string data;
		int num_vertices;
		int num_edges;
		edge e;

		getline(myfile,line);
		stringstream linestream(line);
		linestream >> data >> num_vertices >> num_edges;

		// print details

		cout << "Graph Type: " <<  data << " | num vertices: " << num_vertices << 
		" | num edges: " << num_edges << "\n";

		Graph* G = init_graph(num_vertices, num_edges);
		while(getline(myfile, line))
		{
			std::stringstream linestream(line);
			int from_vertex;
			int to_vertex;
			int weight;
			linestream >> from_vertex >> to_vertex >> weight;
			e.to_vertex = to_vertex;
			e.weight = weight;
			graph_insert(G, from_vertex, e);
		}
		myfile.close();
		return G;
	}
	else
	{
		exit(1);
	}
}

void print_graph(Graph* G)
{
	int num_vertices = G->num_vertices;

	for(int i = 0; i < num_vertices; i++)
	{
		LL* iter = (G->adjacency)[i];

		printf("vertex number = %d\n", i);

		while(iter != NULL)
		{
			printf("(%d,%d) ", iter->adj.to_vertex, iter->adj.weight);
			iter = iter->next;
		}

		printf("--------------------------------------\n");

	}
}

int main(int argc, char* argv)
{
	char file_input[256];
	scanf("%s", file_input);

	Graph* G = read_graph_input(file_input);

	print_graph(G);

	return 0;
}