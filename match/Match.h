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

private:
	int qsize, dsize;
	Graph* query;
	Graph* data;
	//structures for query
	int* qcore;
	bool* qin;
	bool* qout;
	int qin_num, qout_num;
	//structures for data
	int* dcore;
	bool* din;
	bool* dout;
	int din_num, dout_num;
	//call with matching num
	void dfs(int num, IO& io);
};

#endif

