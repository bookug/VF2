/*=============================================================================
# Filename: Pattern.cpp
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 23:01
# Description: 
=============================================================================*/

#include "Pattern.h"

using namespace std;

PID Pattern::current_pid = 0;

Pattern::Pattern(LABEL vertex)
{
	this->parent = NULL;
	this->pid = -1;
	//this->pid = Pattern::current_pid++;
	this->addVertex(new Vertex(vertex));
	//the only vid is 0
	rmp.push_back(0);
}

Pattern::Pattern(const OneEdge& edge)
{
	this->parent = NULL;
	this->pid = -1;
	//this->pid = Pattern::current_pid++;
	this->addVertex(new Vertex(edge.from));
	this->addVertex(new Vertex(edge.to));
	this->addEdge(new Edge(0, 1, edge.pred));
	//the only eid is 0
	this->addNeighbor(0, Neighbor(1,0));
	this->addNeighbor(1, Neighbor(0,0));
	//set rightmost path
	this->rmp.push_back(0);
	this->rmp.push_back(1);
	//set the code
	Element ele = Element(0, 1, edge.from, edge.pred, edge.to);
	//can not write directly (Element()) because the arg is a Element&
	this->code.addElement(ele);
}

Pattern::Pattern(Pattern* ptn)
{
	//copy the basic information
	this->parent = ptn;
	//this->parent = ptn->pid;
	this->pid = -1;
	this->code = ptn->code;

	//do not copy support
	this->rmp = ptn->rmp;
	this->copy(ptn);
}
	
void 
Pattern::addForwardEdge(const Element& et)
{
	this->addEdge(new Edge(et.from, et.to, et.elb));
	EID eid = this->eSize() - 1;
	this->addVertex(new Vertex(et.lb2));
	this->addNeighbor(et.from, Neighbor(et.to, eid));
	this->addNeighbor(et.to, Neighbor(et.from, eid));
	this->code.addElement(et);
	////update the rightmost path
	//vector<VID> nmp;
	//int i, j, k = rmp.size();
	//for(i = 0; i < k; ++i)
	//{
		//j = this->rmp[i];
		//nmp.push_back(j);
		//if(j == et.from)
		//{
			//break;
		//}
	//}
	////if grow from the right most vertex, then just append a new edge
	//nmp.push_back(et.to);
	//this->rmp = nmp;
}

void 
Pattern::addBackwardEdge(const Element& et)
{
	this->addEdge(new Edge(et.from, et.to, et.elb));
	EID eid = this->eSize() - 1;
	this->addNeighbor(et.from, Neighbor(et.to, eid));
	this->addNeighbor(et.to, Neighbor(et.from, eid));
	this->code.addElement(et);
}

unsigned
Pattern::frequence() const
{
	return this->support.size();
}

GID 
Pattern::lastSupport() const
{
	int size = this->support.size();
	if(size == 0)
		return -1;
	return this->support[size-1]->gid;
}

Mapping* 
Pattern::lastMapping() const
{
	int size = this->support.size();
	if(size == 0)
		return NULL;
	return this->support[size-1];
}
void 
Pattern::addSupport(Mapping* mp)
{
	this->support.push_back(mp);
}

//void 
//Pattern::addElement(Element& ele)
//{
//}

Pattern::~Pattern()
{
	int size = this->support.size();
	for(int i = 0; i < size; ++i)
	{
		delete this->support[i];
	}
}

void 
Mapping::addPath(vector<VID>& path)
{
	this->rmps.push_back(path);
}

void 
Mapping::addEdge(vector<EID>& eset)
{
	this->amps.push_back(eset);
}

