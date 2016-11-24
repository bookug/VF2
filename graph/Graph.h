/*=============================================================================
# Filename: Graph.h
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 23:00
# Description: 
=============================================================================*/

#ifndef _GRAPH_GRAPH_H
#define _GRAPH_GRAPH_H

#include "../util/Util.h"

class Edge
{
public:
	VID from;
	VID to;
	//EID id;
	LABEL label;
	Edge()
	{
		from = to = -1;
		label = -1;
	}
	Edge(VID _from, VID _to, LABEL _lb):from(_from), to(_to), label(_lb)
	{
	}
};

class OneEdge
{
public:
	LABEL from;
	LABEL to;
	LABEL pred;
	OneEdge()
	{
		from = to = pred = -1;
	}
	//NOTICE:we should keep from<=to to ensure correctness
	OneEdge(LABEL _from, LABEL _to, LABEL _pred)
	{
		//Util::swapLabel(_from, _to);
		from = _from;
		to = _to;
		pred = _pred;
	}
	bool operator<(const OneEdge& ins) const
	{
		if(this->from != ins.from) return this->from < ins.from;
		if(this->pred != ins.pred) return this->pred < ins.pred;
		if(this->to != ins.to) return this->to < ins.to;
		return false;
	}
	bool operator==(const OneEdge& ins) const
	{
		return this->from == ins.from && this->pred == ins.pred && this->to == ins.to;
	}

	bool operator!=(const OneEdge& ins) const
	{
		return this->from != ins.from || this->pred != ins.pred || this->to != ins.to;
	}
};

class Neighbor
{
public:
	VID vid;
	EID eid;
	Neighbor()
	{
		vid = eid = -1;
	}
	Neighbor(int _vid, int _eid)
	{
		vid = _vid;
		eid = _eid;
	}
};

class Vertex
{
public:
	//VID id;
	LABEL label;
	//NOTICE:VID and EID is just used in this single graph
	std::vector<Neighbor> neighbors;
	Vertex()
	{
		label = -1;
	}
	Vertex(LABEL lb):label(lb)
	{
	}
};

class Graph
{
public:
	//GID id;
	//gid is just the vector index
	std::vector<Vertex*> vertices;
	std::vector<Edge*> edges;
	Graph()
	{
	}
	~Graph();
	int vSize() const
	{
		return vertices.size();
	}
	int eSize() const
	{
		return edges.size();
	}
	void copy(const Graph* graph);
	Vertex* getVertex1ByEdge(EID eid);
	Vertex* getVertex2ByEdge(EID eid);
	VID getVertex1IDByEdge(EID eid);
	VID getVertex2IDByEdge(EID eid);
	LABEL getVertex1LabelByEdge(EID eid);
	LABEL getVertex2LabelByEdge(EID eid);
	LABEL getLabelByEdge(EID eid);
	LABEL getLabelByVertex(VID vid);
	void addVertex(Vertex* vertex);
	void addEdge(Edge* edge);
	void addNeighbor(VID vid, Neighbor neigh);
	std::vector<Neighbor>& getNeighborByVertex(VID vid);
	VID getEdgeNeighbor(EID eid, VID vid);
	void rebuild();
};

#endif

