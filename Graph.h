#ifndef GRAPH_H
#define GRAPH_H

#include "Vertex.h"
#include "Edge.h"
#include "Node.h"
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
#define IN_FINITY 999999
#define visited true
#define unvisited false

class Graph
{
private:
    // the head pointer for the linked list of the vertics
    Vertex* m_pVHead;
    // the number of the vertics
    int m_vSize;
    Node* pHead;    //graph's head
    Node* pNext;    //next pointer
    Node* pDown;    //down pointer
    int head;   //head vertex

public:
	/// constructor
	Graph();
	/// destructor
	~Graph();


    /// add vertex with vertexNum at the end of the linked list for the vertics
    void AddVertex(int vertexKey);  //Not used

    /// add edge from the vertex which the number is startVertexKey to the vertex which the number is endVertexKey
    void AddEdge(int startVertexKey, int endVertexKey, int weight); //Not used

    /// get the vertex which the key is vertexNum
   // Vertex* FindVertex(int key);    //Not used

    /// get the number of the vertics
    int Size() const;

    /// memory free for the vertics
    void Clear();

    /// print out the graph as matrix form
    void Print(std::ofstream& fout);
    //원래 std::ofstream& fout 이거 함수 인자로 들어가 있어야함!!!!!

    /// check whether the graph has negative edge or not.
    bool IsNegativeEdge();

    /// find the path from startVertexKey to endVertexKey with BFS
	std::vector<int> FindPathBfs(int startVertexKey, int endVertexKey);

    /// find the shortest path from startVertexKey to endVertexKey with Dijkstra using std::set
    std::vector<int> FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey);

    /// find the shortest path from startVertexKey to endVertexKey with Bellman-Ford
    std::vector<int> FindShortestPathBellmanFord(int startVertexKey, int endVertexKey);
    
    std::vector<vector<int> > FindShortestPathFloyd();

    //insert node
    bool AddNode(int vertex, int v_key, int weight);

    //insert from vertex
    bool AddLine();

    bool Empty();   //A function to check if the graph is empty
};

#endif
