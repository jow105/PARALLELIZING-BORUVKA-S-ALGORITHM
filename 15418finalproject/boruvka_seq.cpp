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
using namespace std;
 
struct edge
{
    int source;
    int dest;
    int weight;
};
 
typedef struct edge Edge;
 
struct graph
{
    int num_vertices;
    int num_edges;
    Edge* edges;
};
 
typedef struct graph Graph;
 
// Use a union find data structure
struct UnionFind_Set
{
    int parent;
    int rank;
};
 
typedef struct UnionFind_Set set;
 
Graph* init_graph(int num_vertices, int num_edges)
{
    Graph* G = (Graph*)malloc(sizeof(Graph));
    G->num_vertices = num_vertices;
    G->num_edges = num_edges;
    G->edges = (Edge*)malloc(sizeof(edge)*num_edges);
    return G;
}
 
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
    vector<double> first_loop;
    vector<double> second_loop;
    start = CycleTimer::currentSeconds();
    int num_vertices = G->num_vertices;
    int num_edges = G->num_edges;
    Edge* edges = G->edges;
 
    int num_components = num_vertices;
    long long MST_weight = 0;
 
    set* ufind_sets = (set*)malloc(sizeof(set)*num_vertices);
 
    int* cheap = (int*)malloc(sizeof(int)*num_vertices);
 
    for(int i = 0; i < num_vertices; i++)
    {
        ufind_sets[i].parent = i;
        ufind_sets[i].rank = 0;
        cheap[i] = -1;
    }
    int i= 0;
    while(num_components > 1)
    {
   
        //cout << "num components: " << num_components << "\n";
   
        double start_first = CycleTimer::currentSeconds();
        for(int j = 0; j < num_edges; j++)
        {
            // cout << "actual source: " << edges[j].source
                 // << ", actual dest: " << edges[j].dest << " \n";
            int source = find_parent(ufind_sets,edges[j].source);
            int dest = find_parent(ufind_sets,edges[j].dest);
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
        first_loop.push_back(CycleTimer::currentSeconds() - start_first);
       
        double start_second = CycleTimer::currentSeconds();
        for(int k = 0; k < num_vertices; k++)
        {
            // print_union(ufind_sets, num_vertices);
            if(cheap[k] != -1)
            {
                // cout << "actual source: " << edges[cheap[k]].source
                 // << ", actual dest: " << edges[cheap[k]].dest << " \n";
                int source = find_parent(ufind_sets,edges[cheap[k]].source);
                int dest = find_parent(ufind_sets,edges[cheap[k]].dest);
                // cout << "source: " << source << ", dest: " << dest << " \n";
                if(source == dest)
                {
       
                    // cout << "continued \n";
                    continue;
                }
                MST_weight += (long long)edges[cheap[k]].weight;
 
                printf("Edge %d->%d included in MST\n",
                    edges[cheap[k]].source,edges[cheap[k]].dest,edges[cheap[k]].weight);
 
                union_sets(ufind_sets,source,dest);
                num_components--;
            }
            // cout << "\n";
        }
        second_loop.push_back(CycleTimer::currentSeconds() - start_second);
        // print_union(ufind_sets, num_vertices);  
        // cout << "\n";
               
        for(int n = 0; n < num_vertices; n++)
        {
            cheap[n] = -1;
        }
       
    }
    printf("total weight = %llu\n", MST_weight);
    double boruvka_time = CycleTimer::currentSeconds() - start;
    cout << "Total Time taken to run: " << boruvka_time << "\n";
    double total_first, total_second;
    total_first =0;
    total_second=0;
    for(size_t i = 0; i < first_loop.size();i++)
    {
        total_first += first_loop[i];
        total_second += second_loop[i];
    }
    cout << "first loop time: " << total_first << "\n";
    cout << "second loop time: " << total_second << "\n";
    return;
}
 
Graph* read_graph_input(string file_name)
{
 
    string line;
 
    ifstream myfile (file_name.c_str());
   
    if (myfile.is_open())
    {
        std::string data;
        int vertices;
        int edges;
 
        getline (myfile,line);
        stringstream linestream(line);
        linestream >> data >> vertices >> edges;
        cout << "Graph Type: " <<  data << " | num vertices: " << vertices <<
        " | num edges: " << edges << "\n";
        Graph* graph = init_graph(vertices, edges);
        int it = 0;
        while ( getline (myfile,line) )
        {
            std::stringstream linestream(line);
            int source;
            int dest;
            int weight;
            linestream >> source >> dest >> weight;
            graph->edges[it].source = source;
            graph->edges[it].dest = dest;
            graph->edges[it].weight = weight;
            cout << "edge: " << source << "," << dest << " | weight: " << weight << "\n";
            it++;
        }
        myfile.close();
        return graph;
    }
    else
    {
 
        exit(1);
    }
 
 
}
 
// g++ -Wall -Wextra -O2 -std=c++11 boruvka_seq.cpp -lm
// ./a.out graphs/300k_node_sparse.txt
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
 
                graph->edges[edge_it].source = u;
                graph->edges[edge_it].dest = v;
 
                int r = rand() % randweights.size();
                graph->edges[edge_it].weight = randweights[r];
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

    // int n = 0;
    // int m = 0;

    // scanf("%d", &n);
    // scanf("%d", &m);

    
    
    // Graph* graph = make_adj(n,m);
    Graph* graph = read_graph_input(argv[1]);

    



   
 

    run_boruvka(graph);
 
    return 0;
}