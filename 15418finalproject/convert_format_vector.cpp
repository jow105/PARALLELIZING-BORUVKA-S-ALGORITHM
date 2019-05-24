#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CycleTimer.h"
#include <cstdlib>
#include <algorithm>
#include "convert_format_vector.h"
using namespace std;

Graph* init_graph(int num_vertices, int num_edges)
{
	Graph* G = new Graph;

	G->num_vertices = num_vertices;
	G->num_edges = num_edges;
	G->adjacency = new Vector[num_vertices];

	return G;
}

Graph* graph_insert(Graph* G, int from_vertex, edge e)
{
	int to_vertex = e.to_vertex;
	int weight = e.weight;

	edge add_edge;
	add_edge.from_vertex = from_vertex;
	add_edge.to_vertex = to_vertex;
	add_edge.weight = weight;

	(G->adjacency)[from_vertex].push_back(add_edge);

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
	for(int i = 0; i < G->num_vertices; i++)
	{
		printf("adjacencies for vertex %d\n", i);
		for(int j = 0; j < G->adjacency[i].size(); j++)
		{
			printf("(%d,%d)\n", G->adjacency[i][j].to_vertex, G->adjacency[i][j].weight);
		}
		printf("-----------------------------------------------------------\n");
	}
}

// int main()
// {
// 	char file_input[256];
// 	scanf("%s", file_input);

// 	Graph* G = read_graph_input(file_input);

// 	print_graph(G);

// 	return 0;
// }