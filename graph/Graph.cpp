/*=============================================================================
# Filename: Graph.cpp
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 23:01
# Description: 
=============================================================================*/

#include "Graph.h"

using namespace std;

void 
Graph::copy(const Graph* graph)
{
	int vsize = graph->vertices.size();
	int esize = graph->edges.size();
	int i, j;
	Vertex* vp = NULL;
	Edge* ep = NULL;
	for(i = 0; i < vsize; ++i)
	{
		vp = new Vertex();
		*vp = *(graph->vertices[i]);
		this->vertices.push_back(vp);
	}
	for(j = 0; j < esize; ++j)
	{
		ep = new Edge();
		*ep = *(graph->edges[j]);
		this->edges.push_back(ep);
	}
}

void
Graph::rebuild()
{
	//rebuild according to edge status
	int j, k;
	int vsize = this->vSize();
	int esize = this->eSize();
	for(j = 0; j < vsize; ++j)
	{
		Vertex* vtp = this->vertices[j];
		bool valid = false;
		vector<Neighbor>& neigh = vtp->neighbors;
		int nsize = neigh.size();
		for(k = 0; k < nsize; ++k)
		{
			if(this->edges[neigh[k].eid] == NULL)
			{
				neigh[k].eid = -1;
			}
			else if(!valid)
			{
				valid = true;
			}
		}
		if(!valid)
		{
			delete this->vertices[j];
			this->vertices[j] = NULL;
		}
	}

	VID* vmap = new VID[vsize];
	EID* emap = new EID[esize];
	vector<Vertex*> vset;
	vector<Edge*> eset;

	for(j = 0; j < vsize; ++j)
	{
		Vertex* vtp = this->vertices[j];
		if(vtp != NULL)
		{
			vset.push_back(vtp);
			vmap[j] = vset.size() - 1;
		}
	}
	for(j = 0; j < esize; ++j)
	{
		Edge* etp = this->edges[j];
		if(etp != NULL)
		{
			eset.push_back(etp);
			emap[j] = eset.size() - 1;
		}
	}

	int vsize2 = vset.size();
	int esize2 = eset.size();
	for(j = 0; j < esize2; ++j)
	{
		eset[j]->from = vmap[eset[j]->from];
		eset[j]->to = vmap[eset[j]->to];
	}
	for(j = 0; j < vsize2; ++j)
	{
		vector<Neighbor>& neigh = vset[j]->neighbors;
		vector<Neighbor> tnb;
		int nsize = neigh.size();
		for(k = 0; k < nsize; ++k)
		{
			if(neigh[k].eid != -1)
			{
				tnb.push_back(Neighbor(vmap[neigh[k].vid], emap[neigh[k].eid]));
			}
		}
		neigh = tnb;
	}

	this->vertices = vset;
	this->edges = eset;
	delete[] vmap;
	delete[] emap;
}

VID 
Graph::getEdgeNeighbor(EID eid, VID vid)
{
	Edge* ep = this->edges[eid];
	if(ep->from == vid)
	{
		return ep->to;
	}
	else   //to == vid
	{
		return ep->from;
	}
}

Vertex* 
Graph::getVertex1ByEdge(EID eid)
{
	return this->vertices[this->edges[eid]->from];
}

Vertex* 
Graph::getVertex2ByEdge(EID eid)
{
	return this->vertices[this->edges[eid]->to];
}

VID 
Graph::getVertex1IDByEdge(EID eid)
{
	return this->edges[eid]->from;
}

VID 
Graph::getVertex2IDByEdge(EID eid)
{
	return this->edges[eid]->to;
}

LABEL 
Graph::getVertex1LabelByEdge(EID eid)
{
	return this->vertices[this->edges[eid]->from]->label;
}

LABEL 
Graph::getVertex2LabelByEdge(EID eid)
{
	return this->vertices[this->edges[eid]->to]->label;
}

LABEL 
Graph::getLabelByEdge(EID eid)
{
	return this->edges[eid]->label;
}

LABEL 
Graph::getLabelByVertex(VID vid)
{
	return this->vertices[vid]->label;
}

void 
Graph::addVertex(Vertex* vertex)
{
	this->vertices.push_back(vertex);
}

void 
Graph::addEdge(Edge* edge)
{
	this->edges.push_back(edge);
}

void 
Graph::addNeighbor(VID vid, Neighbor neigh)
{
	this->vertices[vid]->neighbors.push_back(neigh);
}

vector<Neighbor>& 
Graph::getNeighborByVertex(VID vid)
{
	return this->vertices[vid]->neighbors;
}

Graph::~Graph()
{
	int vsize = this->vertices.size();
	for(int i = 0; i < vsize; ++i)
	{
		delete this->vertices[i];
	}
	int esize = this->edges.size();
	for(int i = 0; i < esize; ++i)
	{
		delete this->edges[i];
	}
}

