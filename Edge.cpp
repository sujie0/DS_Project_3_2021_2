#include "Edge.h"

Edge::Edge()	//Initialization
{
	m_key = 0;
	m_weight = 0;
	m_pNext = NULL;
}

/// copy constructor
Edge::Edge(int key, int weight)
{
	m_key = key;
	m_weight = weight;
	m_pNext = NULL;
}

//set the Next connection to new connection
void Edge::SetNext(Edge* pNext)
{
	m_pNext = pNext;
}

/// get the key of this edge
int Edge::GetKey() const
{
	return m_key;
}

/// get the weight of this edge
int Edge::GetWeight() const
{
	return m_weight;
}

/// get the next pointer of this edge
Edge* Edge::GetNext() const
{
	return m_pNext;
}

//set the weight to new value
void Edge::SetWeight(int weight)
{
	m_weight = weight;
}