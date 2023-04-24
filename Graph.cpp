#include "Graph.h"
#include "Stack.h"
#include "MinHeap.h"
#include <set>
#include <queue>
#include <stack>
using namespace std;
#define BFS_FIRST_PATH

Graph::Graph()  //Initialization
{
    m_pVHead = NULL;
    m_vSize = 0;
    pHead = NULL;
    pNext = NULL;
    pDown = NULL;
    head = 0;
}
Graph::~Graph() //distructor
{
    // TODO: implement
}

/// add vertex with vertexNum at the end of the linked list for the vertics
void Graph::AddVertex(int vertexKey)
{   //Not used
}

/// add edge from the vertex which the number is startVertexKey to the vertex which the number is endVertexKey
void Graph::AddEdge(int startVertexKey, int endVertexKey, int weight)
{   //Not used
}

/*
/// get the vertex which the key is vertexNum
Vertex* Graph::FindVertex(int key)
{
     //Not used
}
*/


/// get the number of the vertics
int Graph::Size() const
{
    return m_vSize;
}

/// memory free for the vertics
void Graph::Clear()
{
   
}

/// print out the graph as matrix form
void Graph::Print(std::ofstream& fout)
{
    Node* pNode = pHead;
    Node* Head = pHead;

    fout << "=========PRINT========" << endl;
    while (Head)
    {
        pNode = pNode->GetNext();
        while (pNode)
        {
            fout << pNode->GetEdge()->GetWeight() << " ";
            pNode = pNode->GetNext();
        }
        Head = Head->GetDown();
        pNode = Head;
       
        fout << endl;
    }
    fout << "=====================" << endl << endl;
}

/// check whether the graph has negative edge or not.
bool Graph::IsNegativeEdge()
{
    bool result = false; //At the beginning, assume that there's no negative weight
    int size = Size();

    /*Find negative weight*/
    Node* pNode = pHead;
    Node* Head = pHead;
    int i=0,j=0;
    while(Head)
    {
	pNode=pNode->GetNext();
        while(pNode)
        {
            int weight = pNode->GetEdge()->GetWeight();
            if (weight < 0)
                result = true; //if vertex have negative weight
	    pNode=pNode->GetNext();
	    j++;
        }
        Head = Head->GetDown();
        pNode = Head;
	i++;
	j=0;
    }
    return result;
}


/// find the path from startVertexKey to endVertexKey with BFS
std::vector<int> Graph::FindPathBfs(int startVertexKey, int endVertexKey)
{

    int size = Size();
    vector < vector <int> > weight(size, vector <int>(size, 0)); //store weight
    vector < vector <int> > vertex(size, vector <int>(size, 0)); //store vertex
 
    /*Move to the value vector of the link list*/
    Node* pNode = pHead;
    Node* Head = pHead;
    for(int i=0;i<size;i++) 
    {
        for(int j=0;j<size;j++)
        {
	    pNode=pNode->GetNext();
            int v = pNode->GetVertex()->GetKey();   //vertex key
            int w = pNode->GetEdge()->GetWeight();  //weight

            if (w <= 0) //If not connected, change the value to infinity
            {
                w = IN_FINITY;
                v = IN_FINITY;
            }
            vertex[i][j] = v;
            weight[i][j] = w;
      	}
	Head = Head->GetDown();
        pNode = Head;
    }
    
    bool* visit = new bool[size];   //Variables to determine if visited the vertex
    vector<int> prev(size,IN_FINITY);   //Save the previous vertex
    vector<bool> check(size, false);    //Variables to check if have been included in a queue
    vector<int> d(size, 0);  //Save the distance

    for (int i = 0; i < size; i++)
        visit[i] = false;   //Initialization (Not visit)

    queue<int> q;   //during BFS, used queue
    q.push(startVertexKey); //start startVertex
    visit[startVertexKey] = true;

    /*BFS*/
    while (!q.empty())
    {
        int x = q.front();  //store queue's first vertex value
        q.pop();
        for (int i = 0; i <size; i++)
        {
            int y = weight[x][i];

            if (!visit[i])  //if not visited, continue
            {
                if (y != IN_FINITY)  
                {
                    int v_num = vertex[x][i];   //store vertex key
                    q.push(v_num);
                    d[i] = weight[x][i];    //update distance
                    visit[v_num] = true;    //visited
                    prev[v_num] = x;        //store previous vertex
                    if (v_num == size - 1)  //if reach the endVertex
                    {
                      //  cout << "Reach the final destination" << endl;
                        //is need break?
                        //break;
                    }

                }
            }
        }
    }
   
    /*Backtrack the path*/
    vector<int> result;
    stack<int> temp;   //Temporary variable to store reverse path
    int i = size - 1;   //endVertex
    int tem = IN_FINITY;
    int distance = d[size - 1];
    temp.push(i);   //Store endVertex
  
    while (i>0)     //Repeat until the startVertex
    {
        temp.push(prev[i]);
        tem = prev[i];
        i = tem;
        distance += d[i];   //Add distance
    }

    //Change the route (startVertex -> endVertex)
    while(!temp.empty())    //Move all paths
    {
        result.push_back(temp.top());
        temp.pop();
    }
    result.push_back(distance); //Save the shortest distance at the end

    delete[] visit; //free memory
    return result;
}



/// find the shortest path from startVertexKey to endVertexKey with Dijkstra using std::set
std::vector<int> Graph::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
    int size = Size();
 
    vector < vector <int> > weight(size, vector <int>(size, 0)); //Store weight
    vector < vector <int> > vertex(size, vector <int>(size, 0)); //Store vertex key
   
    /*Move to the value vector of the link list*/
    Node* pNode = pHead;
    Node* Head = pHead;
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
	    pNode=pNode->GetNext();
            int v = pNode->GetVertex()->GetKey();   //vertex key
            int w = pNode->GetEdge()->GetWeight();  //weight
 
            if (w <= 0) //If not connected, change the value to infinity
            {
                w = IN_FINITY;
                v = IN_FINITY;
            }
            vertex[i][j] = v;
            weight[i][j] = w;
        }
        Head = Head->GetDown();
        pNode = Head;
    }

    set<pair<int, int>> pq; //using set <vertex key, weight>
    int* d = new int[size]; //Save the distance
    int* prev = new int[size];  //Save the previous vertex
    for (int i = 0; i < size; i++)
    {  //Initialization
        d[i] = IN_FINITY;
        prev[i] = -1;
    }
    d[startVertexKey] = 0;  //Save the initial distance
    pq.insert({startVertexKey, 0 });    //start startVertex
    set<pair<int, int>>::iterator iter = pq.begin();    //pq's first data

    /*Dijkstra*/
    while (!pq.empty())
    {
        iter = pq.begin();
        int current = iter->first;  //current vertes key
        //Negativeize weight because the short distance to come first
        int distance = -iter->second;   //weight
        pq.erase(iter); //pop pq's first data

        if (d[current] < distance) continue;    //if it is not shortest path, Skip under code
        for (int i = 0; i < size; i++)
        {
            int next = i;   //Adjacent nodes of the selected node
            int nextDistance = distance + weight[current][i];   //The cost of passing through the selected node to the neighboring node
 
            if (nextDistance < d[next]) //if it's shorter than the original distance
            {
                d[next] = nextDistance; //Update distance
                pq.insert({ next,-nextDistance });
                prev[next] = current;   //Update previous vertex
            }
        }
    }

    /*Backtrack the path*/
    vector<int> result;
    stack<int> temp;   //Temporary variable to store reverse path
    int i = size - 1;   //endVertex
    int tem = IN_FINITY;
    int distance = d[size - 1];
    temp.push(i);   //Store endVertex

    while (prev[i] >= 0) //Repeat until the startVertex
    {
        temp.push(prev[i]);
        tem = prev[i];
        i = tem;
    }

    //Change the route (startVertex -> endVertex)
    while (!temp.empty())    //Move all the path
    {
        result.push_back(temp.top());
        temp.pop();
    }
    result.push_back(distance); //Save the shortest distance at the end
   
    //free memory
    delete[] d;
    delete[] prev;

    return result;
}


/// find the shortest path from startVertexKey to endVertexKey with Bellman-Ford
std::vector<int> Graph::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
    int size = Size();

    vector < vector <int> > weight(size, vector <int>(size, 0)); //Store weight
    vector < vector <int> > vertex(size, vector <int>(size, 0)); //Store vertex
    vector<int> node(size, IN_FINITY);   //variable to save the starting vertex
    vector<int> result; //Save the result value to return

    /*Move to the value vector of the link list*/
    Node* pNode = pHead;
    Node* Head = pHead;
    for(int i=0;i<size;i++)
    {
        node[i] = pNode->GetVertex()->GetKey(); //Save the staring vertex
        for(int j=0;j<size;j++)
        {
            pNode = pNode->GetNext();
            int v = pNode->GetVertex()->GetKey();   //vertex key
            int w = pNode->GetEdge()->GetWeight();  //weight

            if (w == 0) //Not exclude negative weight
            {   //If not connected, change the value to infinity
                w = IN_FINITY;
                v = IN_FINITY;
            }

            vertex[i][j] = v;
            weight[i][j] = w;
        }
        Head = Head->GetDown();
        pNode = Head;
    }

    /*Bellmanford*/
    int* d = new int[size]; //Save the distance
    int* prev = new int[size];  //Save the previous vertex
    for (int i = 0; i < size; i++)
    {   //Initialization (Not visit)
        d[i] = IN_FINITY;
        prev[i] = IN_FINITY;
    }
    d[0] = 0;   //startVertex distance

    for(int n=0;n<size-1;n++)   //Repeat v-1 
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                int From = node[i]; //vertex start(from)
                int To = vertex[i][j];  //vertex end(to)
                int Cost = weight[i][j];    //weight

                if (Cost == IN_FINITY) continue; //If the vertex is not connected
                if (d[To] > d[From] + Cost)
                {   //If the distance is shorter than the previous one
                    d[To] = d[From] + Cost; //update distance
                    prev[To] = From;    //update previous vertex
                }
            }
        }
    }
  
    /*Repeat one more time to determine if a negative cycle occurs*/
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int From = node[i]; //vertex start(from)
            int To = vertex[i][j];  //vertex end(to)
            int Cost = weight[i][j];     //weight

            if (Cost == IN_FINITY) continue; //If the vertex is not connected
            if (d[To] > d[From] + Cost)
            {   //If the distance is shorter than the previous one
                //negative clcyle occurs
                return result;
            }
        }
    }
    
    /*Backtrack the path*/
    stack<int> temp;   //Temporary variable to store reverse path
    int i = size - 1;   //endVertex
    int tem = IN_FINITY; //Temporary variable
    int distance = d[size - 1];
    temp.push(i);   //Store endVertex

    //store path in the temp (endVertex -> startVertex)
    while (prev[i]!=IN_FINITY)
    {
        temp.push(prev[i]);
        tem = prev[i];
        i = tem;    //store previous vertex
    }

    //Change the route (startVertex -> endVertex)
    while (!temp.empty())    //Move all paths
    {
        result.push_back(temp.top());
        temp.pop();
    }
    result.push_back(distance); //Save the shortest distance at the end

    //free memory
    delete[]d;
    delete[]prev;
    return result;
}


std::vector<vector<int> > Graph::FindShortestPathFloyd()
{ 
    int size = Size();
    vector < vector <int> > weight(size, vector <int>(size, 0)); //Store weight
   
    /*Move to the value vector of the link list*/
    Node* pNode = pHead;
    Node* Head = pHead;
    int a=0,b=0;
    while(Head)
    {
        pNode = pNode->GetNext();
        while(pNode)
        {
            weight[a][b] = pNode->GetEdge()->GetWeight();
            if (a != b)
            {
                if (weight[a][b] == 0)   //Not exclude negative weight
                    weight[a][b] = IN_FINITY; //If not connected, change the value to infinity
            }
            pNode = pNode->GetNext();
	    b++;
        }
        Head = Head->GetDown();
        pNode = Head;
	a++;
	b=0;
    }
    
    /*Floyd*/
    for (int k = 0; k < size; k++)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                int temp = weight[i][k] + weight[k][j]; //go through k vertex
                if (weight[i][k] == IN_FINITY && weight[k][j] < 0) {}    //nothing
                else if (weight[i][k] < 0 && weight[k][j] == IN_FINITY)  {}   //nothing
                else if (temp < weight[i][j])    //If the distance is smaller through k vertex
                    weight[i][j] = temp;    //update distance
            }
        }
    }

    /*Check if a negative cycle occurs*/
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            if (i==j) //If the distance to oneself is negative, a negative cycle occurs
            {
                if (weight[i][j] < 0)    //negative cycle occurs
                {
                    weight.clear();
                    return weight;   //return empty result vector
                }
            }

    //Change the disconnected partial INFINITE value back to 0
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (weight[i][j] == IN_FINITY)
                weight[i][j] = 0;

    return weight;
}


bool Graph::AddNode(int vertex, int v_key, int weight)
{//Vertex is its key value connected to the bottom at the front of the linked list

    /*Store from vertex*/
    Node* Head = pHead; //The first value of each linked list
    while (Head)
    {
        if (Head->GetVertex()->GetKey() == vertex)
            break;
        else
             Head = Head->GetDown();
    }

    while (Head->GetNext()) //Find a place to insert new node
    {
        Head = Head->GetNext();
    }
    Node* pNew = new Node;
    Head->SetNext(pNew);    //connect linked list
    pNew->SetNode(v_key, weight);   //insert new node
 
    m_vSize = vertex + 1;   //update vertex size

    return true;
}

bool Graph::AddLine()   //Add it from the linked list to the under
{
    if (!pHead) //graph is empty
    {
        pHead = new Node;   //make new Head
        pHead->SetLine(head);
        head++;
        return true;
    }

    //graph exists
    Node* pNode = pHead;
    while (pNode->GetDown())    //search bottom vertex node
    {
        pNode = pNode->GetDown();
    }

    Node* pNew = new Node;
    pNode->SetDown(pNew);   //connect linked list
    pNew->SetLine(head);    //insert new node
   // delete pNew;  why delete is not??
    head++;

    return true;
}

/*A function to check if the graph is empty*/
bool Graph::Empty()
{
    if (!pHead) //graph is empty
        return true;
    else //graph is not empty
        return false;
}
