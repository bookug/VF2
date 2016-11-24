/*=============================================================================
# Filename: Search.h
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 17:44
# Description: 
=============================================================================*/

#ifndef _SEARCH_SEARCH_H
#define _SEARCH_SEARCH_H

#include "../util/Util.h"
#include "../util/Code.h"
#include "../graph/Pattern.h"
#include "../io/IO.h"

class Tree
{
public:
	std::vector<VID> rmp0;  //rightmost path of the tree
	std::vector<VID> rmp1;  //mapping rmp in the pattern
	int vsize, esize;
	Tree()
	{
		vsize = esize = 0;
	}
	Tree(const Tree& tree)
	{
		this->vsize = tree.vsize;
		this->esize = tree.esize;
		this->rmp0 = tree.rmp0;
		this->rmp1 = tree.rmp1;
	}
	Tree(VID vid)
	{
		rmp0.push_back(0);
		rmp1.push_back(vid);
		this->vsize = 1;
		this->esize = 0;
	}
	void addForwardEdge(Element& et);
	void addBackwardEdge(Element& et);
};

class Search
{
private:
	std::vector<Graph*>& gs;
	std::vector<Pattern*>& front;
	IO& io;
	unsigned border;
	int maxPatternVSize;
	int maxPatternESize;
public:
	//Search()
	//{
		//this->gs = std::vector<Graph*>();
		//this->result = std::vector<Pattern*>();
		//this->front = std::vector<Pattern*>();
	//}
	Search(std::vector<Graph*>& _gs, std::vector<Pattern*>& _front, IO& _io, unsigned _border):gs(_gs),front(_front),io(_io)
	{
		this->border = _border;
		this->maxPatternVSize = MAX_PATTERN_SIZE;
		this->maxPatternESize = MAX_PATTERN_SIZE;
	}
	bool run(int MaxPatternVSize = MAX_PATTERN_SIZE, int MaxPatternESize = MAX_PATTERN_SIZE);
	void SubgraphMining(Pattern* ptn);
	bool NotMiniumCode(Pattern* ptn);
	bool dfs(Pattern* ptn, bool* visit, bool* node, Tree& tree);
	void GenerateMappings(Pattern* edgePattern);
	void GraphProjection(Pattern* edgePattern);
	void OneEdgeGrow(Pattern* ptn, std::vector<Pattern*>& childs);
	void OneMappingGrow(Pattern* ptn, std::map<Element, Pattern*>& cm, Graph* gp, std::vector<VID>& gpath, std::vector<EID>& geset, GID gid);
	static bool PatternCompare(Pattern* a, Pattern* b);
};

#endif //_SEARCH_SEARCH_H

