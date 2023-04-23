#ifndef _Node_H_
#define _Node_H_
#include "Vertex.h"
#include "Edge.h"
#include <vector>
#include <iostream>
using namespace std;

class Node {
private:
	Node* pHead;
	Vertex* v;	//vertex
	Edge* e;	//edge
	Node* pNext;	//next node
	Node* pDown;	//under node
public:
	Node();
	~Node() {
	   delete v;
	delete e;   // 	delete v; delete e;
	       	 }
	void SetNode(int v_key, int weight);	//set Node
	Vertex* GetVertex();	//get vertex
	Edge* GetEdge();	//get edge
	Node* GetNext();	//get next node
	void SetNext(Node* Next);	//set next node
	void SetLine(int v_key);	//set that linked list connected to the bottom

	Node* GetDown();	//get under node
	void SetDown(Node* down);	//set under node
};

#endif
