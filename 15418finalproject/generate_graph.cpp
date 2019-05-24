
 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CycleTimer.h"
#include <cstdlib>
#include <algorithm>
using namespace std;
 
// g++ -Wall -Wextra -O2 -std=c++11 generate_graph.cpp -lm
// ./a.out 300000 2400000 300k_node_dense
 
int main(int argc, char* argv[])
{
 
    //Graph* graph = read_graph_input(argv[1]);
 
    int n = 100000;
    int m = 1000000;
    string s = "";
    scanf("%d",&n);
    scanf("%d",&m);
    getline(cin, s);
    cout << s <<"\n";
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
            num_edges++;
        adj_matrix[u][v] = 1;
    }
    cout << "hello\n";
   
    vector<int> randweights; // randweights
    for(int i =0; i < num_edges; i++)
    {
        randweights.push_back(i+1);
    }
 
 
    ofstream myfile(s.erase(0,1) + ".txt");
   
    myfile << s << " " << n << " " << num_edges << "\n";
    for(int u = 0; u < n; u++)
    {
        for(int v = u; v < n; v++)
        {
            if(adj_matrix[u][v] == 1)
            {
               
 
       
                int r = rand() % randweights.size();
               
                int weight = randweights[r];
                randweights.erase(randweights.begin() + r);
                myfile << u << " " << v << " " << weight << "\n";
            }
        }
    }
    myfile.close();
 
    return 0;
}