/*=============================================================================
# Filename: Match.h
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 22:55
# Description: find all subgraph-graph mappings between query graph and data graph
=============================================================================*/

#ifndef _MATCH_MATCH_H
#define _MATCH_MATCH_H

#include "../util/Util.h"
#include "../graph/Graph.h"
#include "../io/IO.h"

class Match
{
public:
	Match(Graph* _query, Graph* _data);
	void match(IO& io);
	~Match();

private:
	int qnid, dnid;
	int qsize, dsize;
	Graph* query;
	Graph* data;
	//structures for query
	int* qcore;
	int* qin;
	int* qout;
	//structures for data
	int* dcore;
	int* din;
	int* dout;
	//call with matching num
	void dfs(int num, IO& io);
	//decide if to prune this branch
	bool prune();
	bool equal(LABEL lb1, LABEL lb2);
	bool checkCore(std::vector<Neighbor>& qlist, std::vector<Neighbor>& dlist);
	bool checkOther(std::vector<Neighbor>& qlist, std::vector<Neighbor>& dlist);
	//update the 6 queues
	void update(int depth);
	void modify(int depth, std::vector<Neighbor>& list, int* queue);
	//restore the data structures after each dfs iteration
	void restore(int depth);
};

#endif

