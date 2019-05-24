/*
 * 15-418 Parallel Computer Architecture and Programming
 * Benjamin Chu - Jordan Widjaja
 * bcchu - jwidjaja
 * BORUVKA'S ALGORITHM - parallel critical_zone IMPLEMENTATION
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CycleTimer.h"
#include <cstdlib>
#include <algorithm>
#include <omp.h>
#include "dset.h"
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

Graph* init_graph(int num_vertices, int num_edges)
{
    Graph* G = (Graph*)malloc(sizeof(Graph));
    G->num_vertices = num_vertices;
    G->num_edges = num_edges;
    G->edges = (Edge*)malloc(sizeof(edge)*num_edges);
    return G;
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
 
    // set* ufind_sets = (set*)calloc(sizeof(set),num_vertices);

    DisjointSets sets = DisjointSets(num_vertices);
 
    int* cheap = (int*)calloc(sizeof(int),num_vertices);
 
    for(int i = 0; i < num_vertices; i++)
    {
        // ufind_sets[i].parent = i;
        // ufind_sets[i].rank = 0;
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
            int source = sets.find(edges[j].source);
            int dest = sets.find(edges[j].dest);
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
        int* sum = (int*)calloc(sizeof(int),num_vertices);
        int* comp = (int*)calloc(sizeof(int),num_vertices);
        #pragma omp parallel for schedule(dynamic,1000)
        for(int k = 0; k < num_vertices; k++)
        {
            // print_union(ufind_sets, num_vertices);
           
                if(cheap[k] != -1)
                {
                   
                    // cout << "actual source: " << edges[cheap[k]].source
                     // << ", actual dest: " << edges[cheap[k]].dest << " \n";
                    #pragma omp critical
                    {
                        int source = sets.find(edges[cheap[k]].source);
                        int dest = sets.find(edges[cheap[k]].dest);
                   
                    // cout << "source: " << source << ", dest: " << dest << " \n";
                        if(source != dest)
                        {
                           
                            printf("Edge %d->%d included in MST\n",
                            edges[cheap[k]].source,edges[cheap[k]].dest,edges[cheap[k]].weight);
                            sets.unite(source,dest);
                            sum[k] = edges[cheap[k]].weight;
                            comp[k] = 1;
                        }
                    }
                }
 
 
                   
                         
               
               
        }
       
            // cout << "\n";
       
        second_loop.push_back(CycleTimer::currentSeconds() - start_second);
        // print_union(ufind_sets, num_vertices);  
        // cout << "\n";
 
               
        for(int n = 0; n < num_vertices; n++)
        {
            cheap[n] = -1;
            MST_weight+=sum[n];
            num_components-= comp[n];
        }
        free(sum);
        free(comp);
       
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
        adj_matrix[i] = (bool*)calloc(sizeof(bool),n);
    }
    cout << "initial\n";
    for(int i = 0; i < n; i++)
    {
        cout << adj_matrix[0][i] << "\n";
    }
     cout << "initial\n";
    vector<int> randperm;
    for(int i =0; i < n; i++)
    {
        randperm.push_back(i);
    }
    random_shuffle(randperm.begin(),randperm.end());
    int num_edges = 0;
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
        num_edges++;
    }
 
    cout << "m: " << m << "\n";  
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
    int count = 0;
    for(int u = 0; u < n; u++)
    {
 
        for(int v = u; v < n; v++)
        {
            if(adj_matrix[u][v] == 1)
            {
                count++;
            }
        }
    }
    cout << count << "\n";
    for(int u = 0; u < n; u++)
    {
 
        for(int v = u; v < n; v++)
        {
            if(adj_matrix[u][v] == 1)
            {
                // cout << randweights.size() << "\n";
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
 
 
 
 
// g++ -fopenmp -Wall -Wextra -O2 -std=c++11 boruvka_par_openmp.cpp -lm
 
// ./a.out 100000 1000000
int main(int argc, char* argv[])
{
    int n = 0;
    int m = 0;
    int threads = 1;
 
 
    scanf("%d", &n);
    scanf("%d", &m);
    scanf("%d", &threads);
    // Graph* graph = read_graph_input(argv[1]);
 
    Graph* graph = make_adj(n,m);
 
 
    omp_set_num_threads(threads);
 
   
 
    run_boruvka(graph);
 
    return 0;
}