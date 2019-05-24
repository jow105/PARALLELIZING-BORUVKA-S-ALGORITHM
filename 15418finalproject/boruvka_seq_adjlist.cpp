/* 
 * 15-418 Parallel Computer Architecture and Programming
 * Benjamin Chu - Jordan Widjaja
 * bcchu - jwidjaja
 * BORUVKA'S ALGORITHM - SEQUENTIAL IMPLEMENTATION
 */

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

// Use a union find data structure
struct UnionFind_Set
{
	int parent;
	int rank;
};

typedef struct UnionFind_Set set;


int find_parent(set* sets, int to_find)
{
	if(sets[to_find].parent != to_find)
	{
		sets[to_find].parent = find_parent(sets,sets[to_find].parent);
	}
	return sets[to_find].parent;
}

void union_sets(set* sets, int x, int y)
{
	int init_x = find_parent(sets,x);
	int init_y = find_parent(sets,y);

	if(sets[init_x].rank < sets[init_y].rank)
	{
		sets[init_x].parent = init_y;
	}
	else if(sets[init_x].rank > sets[init_y].rank)
	{
		sets[init_y].parent = init_x;
	}
	else
	{
		sets[init_y].parent = init_x;
		sets[init_x].rank++;
	}
}


void print_arr(int* arr, int n)
{
    cout << "cheap array: \n";
    for(int i = 0; i < n; i ++)
    {
        cout << arr[i] << " ";
    }
    cout << "\n";
}

void print_union(set* ufind, int n)
{
    cout << "union find: \n";
    cout << "";
    for(int i = 0; i < n; i ++)
    {
        cout << "parent: " << ufind[i].parent << ", rank:  "  << ufind[i].rank << " | ";
    }
    cout << "\n";
}


void run_boruvka(Graph* G)
{
    double start;
    cout << "start boruvka's\n";
    start = CycleTimer::currentSeconds();
	int num_vertices = G->num_vertices;
	int num_edges = G->num_edges;
	int num_components = num_vertices;
	int MST_weight = 0;

	set* ufind_sets = (set*)malloc(sizeof(set)*num_vertices);

	int* cheap = (int*)malloc(sizeof(int)*num_vertices);

    edge* edges = (edge*)malloc(sizeof(edge)*num_edges);

	for(int i = 0; i < num_vertices; i++)
	{
		ufind_sets[i].parent = i;
		ufind_sets[i].rank = 0;
		cheap[i] = -1;
	}
    int i = 0;
    int edge_counter = 0;
	while(num_components > 1)
	{

        // cout << "\n\n" << "Iteration " << i << "\n";
        //cout << "num components: " << num_components << "\n";
        i++;
        
        for(int j = 0; j < num_vertices; j++)
        {
            for(int k = 0; k < G->adjacency[j].size(); k++)
            {
                edges[edge_counter].from_vertex = j;
                edges[edge_counter].to_vertex = G->adjacency[j][k].to_vertex;
                edges[edge_counter].weight = G->adjacency[j][k].weight;

                edge_counter++;
            }
        }

		for(int j = 0; j < num_edges; j++)
		{
            // cout << "actual source: " << edges[j].source
                 // << ", actual dest: " << edges[j].dest << " \n";
			int source = find_parent(ufind_sets,edges[j].from_vertex);
			int dest = find_parent(ufind_sets,edges[j].to_vertex);
            //cout << "source: " << source << ", dest: " << dest << " \n";
			if(source == dest) 
            {
                //cout << "continued \n";
                continue;
            }
			else
			{
				if(cheap[source] == -1 || edges[cheap[source]].weight > edges[j].weight)
				{
					cheap[source] = j;
				}
				if(cheap[dest] == -1 || edges[cheap[dest]].weight > edges[j].weight)
				{
					cheap[dest] = j;
				}
			}
            // print_arr(cheap, num_vertices);
             // cout << "\n";
		}
        // cout << "\n";
        
		for(int k = 0; k < num_vertices; k++)
		{
            // print_union(ufind_sets, num_vertices);
			if(cheap[k] != -1)
			{
                // cout << "actual source: " << edges[cheap[k]].source
                 // << ", actual dest: " << edges[cheap[k]].dest << " \n";
				int source = find_parent(ufind_sets,edges[cheap[k]].from_vertex);
				int dest = find_parent(ufind_sets,edges[cheap[k]].to_vertex);
                // cout << "source: " << source << ", dest: " << dest << " \n";
				if(source == dest)
				{
        
                    // cout << "continued \n";
					continue;
				}
				MST_weight += edges[cheap[k]].weight;

				printf("Edge %d->%d included in MST\n",
                    edges[cheap[k]].from_vertex,edges[cheap[k]].to_vertex,edges[cheap[k]].weight);

				union_sets(ufind_sets,source,dest);
				num_components--;
			}
            // cout << "\n";
		}
        // print_union(ufind_sets, num_vertices);   
        // cout << "\n";
                
        for(int n = 0; n < num_vertices; n++)
        {
                cheap[n] = -1;
        }
       
	}
	printf("total weight = %d\n", MST_weight);
    double boruvka_time = CycleTimer::currentSeconds() - start;
    cout << "Time taken to run: " << boruvka_time << "\n";
	return;
}

// Graph* read_graph_input(string file_name)
// {
 
//     string line;

//     ifstream myfile (file_name.c_str());
    
//     if (myfile.is_open())
//     {
//         std::string data;
//         int vertices;
//         int edges;

//         getline (myfile,line);
//         stringstream linestream(line);
//         linestream >> data >> vertices >> edges;
//         cout << "Graph Type: " <<  data << " | num vertices: " << vertices << 
//         " | num edges: " << edges << "\n";
//         Graph* graph = init_graph(vertices, edges);
//         int it = 0;
//         while ( getline (myfile,line) )
//         {
//             std::stringstream linestream(line);
//             int source;
//             int dest;
//             int weight;
//             linestream >> source >> dest >> weight;
//             graph->edges[it].source = source;
//             graph->edges[it].dest = dest;
//             graph->edges[it].weight = weight;
//             cout << "edge: " << source << "," << dest << " | weight: " << weight << "\n";
//             it++;
//         }
//         myfile.close();
//         return graph;
//     }
//     else
//     {
//         exit(1);
//     }


// }

// g++ -Wall -Wextra -O2 -std=c++11 boruvka_seq.cpp -lm
// ./a.out graphs/facebook_combined_weights.txt
// Graph* make_adj(int n, int m)
// {


//     bool* adj_matrix[n]; 
//     for(int i = 0; i < n; i ++)
//     {
//         adj_matrix[i] = (bool*)malloc(sizeof(bool)*n);
//     }
    
//     vector<int> randperm;
//     for(int i =0; i < n; i++)
//     {
//         randperm.push_back(i);
//     }
//     random_shuffle(randperm.begin(),randperm.end());
//     for(int i = 0; i < n-1 ;i++) //make sure its connected
//     {
//         int u = randperm[i];
//         int v = randperm[i+1];
//         if(u < v)
//         {
//             adj_matrix[u][v] = 1;
//         }
//         else
//         {
//             adj_matrix[v][u] = 1;
//         }
//     }

//     int num_edges = n-1;
//     for(int i = 0; i < m; i ++) //generate edges
//     {
//         int u = rand() % n;
//         int v = rand() % (n - u) + u + 1;
//         if(adj_matrix[u][v] == 0)
//             num_edges++;
//         adj_matrix[u][v] = 1;
//     }
//     Graph* graph = init_graph(n, num_edges);
//     vector<int> randweights; // randweights
//     for(int i =0; i < num_edges; i++)
//     {
//         randweights.push_back(i+1);
//     }
//     int edge_it = 0;
//     cout << "out\n";
//     for(int u = 0; u < n; u++)
//     {
//         for(int v = u; v < n; v++)
//         {
//             if(adj_matrix[u][v] == 1)
//             {

//                 graph->edges[edge_it].source = u;
//                 graph->edges[edge_it].dest = v;

//                 int r = rand() % randweights.size();
//                 graph->edges[edge_it].weight = randweights[r];
//                 randweights.erase(randweights.begin() + r);
//                 edge_it++;
//             }
//         }
//     }
    

//     return graph;
// }

Graph* make_adj(int n, int m)
{
 
 
    bool* adj_matrix[n];
    for(int i = 0; i < n; i ++)
    {
        adj_matrix[i] = (bool*)malloc(sizeof(bool)*n);
    }
   
    vector<int> randperm;
    for(int i =0; i < n; i++)
    {
        randperm.push_back(i);
    }
    random_shuffle(randperm.begin(),randperm.end());
    for(int i = 0; i < n-1 ;i++) //make sure its connected
    {
        int u = randperm[i];
        int v = randperm[i+1];
        if(u < v)
        {
            adj_matrix[u][v] = 1;
        }
        else
        {
            adj_matrix[v][u] = 1;
        }
    }
 
    int num_edges = n-1;
    for(int i = 0; i < m; i ++) //generate edges
    {
        int u = rand() % (n - 1);
        int v = rand() % (n - u - 1) + u + 1;
        if(adj_matrix[u][v] == 0)
        {
            num_edges++;
        }
        adj_matrix[u][v] = 1;
    }
    Graph* graph = init_graph(n, num_edges);
    cout << "graph has " << n << " vertices and " << num_edges << " edges\n";
    vector<int> randweights; // randweights
    for(int i =0; i < num_edges; i++)
    {
        randweights.push_back(i+1);
    }
    int edge_it = 0;
    cout << "out\n";
    for(int u = 0; u < n; u++)
    {
        for(int v = u; v < n; v++)
        {
            if(adj_matrix[u][v] == 1)
            {
                edge e;
                e.from_vertex = u;
                e.to_vertex = v; 
                int r = rand() % randweights.size();
                e.weight = randweights[r];
                (graph->adjacency)[u].push_back(e);
                randweights.erase(randweights.begin() + r);
                edge_it++;
            }
        }
    }
   
 
    return graph;
}

// g++ -Wall -Wextra -O2 -std=c++11 boruvka_seq.cpp -lm
// ./a.out 100000 1000000
int main(int argc, char* argv[])
{

    //Graph* graph = read_graph_input(argv[1]);

    // int n = 100000;
    // int m = 1000000;
    // scanf("%d",&n);
    // scanf("%d",&m);
    // Graph* graph = make_adj(n, m);

    // char file_input[256];
    // scanf("%s", file_input);

    // Graph* G = read_graph_input(file_input);

    int num_vertices, num_edges;

    scanf("%d %d", &num_vertices, &num_edges);
    
    Graph* graph = make_adj(num_vertices, num_edges);

    run_boruvka(graph);

	return 0;
}