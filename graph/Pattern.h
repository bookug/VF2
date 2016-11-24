/*=============================================================================
# Filename: Pattern.h
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 23:00
# Description: 
=============================================================================*/

#ifndef _GRAPH_PATTERN_H
#define _GRAPH_PATTERN_H

#include "../util/Util.h"
#include "../util/Code.h"
#include "Graph.h"

//just use vector<VID> for rmp and rmps! grow from vid, select the id2 is a question, not always eid.to
//NOTICE:another solution is not to keeping order, but keeping double edges in edges, as well as neighbors

class Mapping
{
public:
	GID gid; // a mapping gid with the pattern
	std::vector< std::vector<VID> > rmps; // all rightmost path mappings
	std::vector< std::vector<EID> > amps; //all edges for a mapping
	//NOTICE:this can be achieved by the two above
	//std::vector< std::vector<VID> > internals; //all internals for a mapping(not rightmost path)
	Mapping(GID _gid):gid(_gid)
	{
	}
	void addPath(std::vector<VID>& path);
	void addEdge(std::vector<EID>& eset);
};

//NOTICE:we had better not put set/map in basic classes, vector and list are ok

//used in frequent pattern search
class Pattern : public Graph
{
public:
	static PID current_pid;
	//should be same as support size
	//int frequence;
	
	//the pid is on output order, also the search order, almost the depth-first-search order except fro vertices
	PID pid; //the id of this pattern
	//this pattern grows from which
	//PID parent; 
	Pattern* parent;
	//valid growth edge from this pattern
	std::set<Element> valid;   
	//record all matchings of this pattern in a graph:just the rightmost path
	std::vector<Mapping*> support; //data graphs containing this subgraph
	//NOTICE:when a backward edge is growed, rightmost path not changes
	//rightmost path only contain forward edge in dfs tree
	std::vector<VID> rmp; //right most path of current pattern
	//NOTICE:it is better to record the eid as path, instead of vid
	//
	//current code for this pattern, rightmost grow only add one element at last
	Code code;
	
	//deal with single vertex
	Pattern(LABEL vertex);
	//deal with single edge
	Pattern(const OneEdge& edge);
	//to genearte a child
	Pattern(Pattern* ptn);
	void addForwardEdge(const Element& et);
	void addBackwardEdge(const Element& et);
	~Pattern();
	unsigned frequence() const;
	GID lastSupport() const;
	Mapping* lastMapping() const;
	void addSupport(Mapping* mp);
	//void addElement(Element& ele);
};

#endif

