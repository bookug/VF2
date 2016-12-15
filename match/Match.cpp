/*=============================================================================
# Filename: Match.cpp
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-12-15 01:38
# Description: 
=============================================================================*/

#include "Match.h"

using namespace std;

Match::Match(Graph* _query, Graph* _data)
{
	this->query = _query;
	this->data = _data;
	this->qsize = _query->vSize();
	this->dsize = _data->vSize();

	this->qcore = new int[qsize];
	this->qin = new bool[qsize];
	this->qout = new bool[qsize];

	this->dcore = new int[dsize];
	this->din = new bool[dsize];
	this->dout = new bool[dsize];
}

//Two States of core queues: matching [0,size) not-matching -1
//Four States of in/out queues: matching [0,size)   in_queue -1  out_queue -1 other -2
//NOTICE:a vertex maybe appear in in_queue and out_queue at the same time
void 
Match::match(IO& io)
{
	//initialize the structures
	int i, j, k;
	for(i = 0; i < qsize; ++i)
	{
		qcore[i] = -1;
		qin[i] = false;
		qout[i] = false;
		this->qin_num = this->qout_num = 0;
	}
	for(i = 0; i < dsize; ++i)
	{
		dcore[i] = -1;
		din[i] = false;
		dout[i] = false;
		this->din_num = this->dout_num = 0;
	}

	//call dfs with macthing num
	dfs(0, io);
}

void
Match::dfs(int num, IO& io)
{
	if(num == this->qsize)
	{
		io.output(this->qcore, this->qsize);
		return;
	}
	//NOTICE:to avoid duplicates, only consider the smallest qid in candidates

	int i, j, k;
	int qnid = -1;
	vector<int> cans;

	//consider the out queue
	if(qout_num > 0 && dout_num > 0)
	{
		for(i = 0; i < qsize; ++i)
		{
			if(!qout[i] || qcore[i] >= 0)
			{
				continue;
			}
			qnid = i;
			for(j = 0; j < dsize; ++j)
			{
				if(!dout[j] || dcore[j] >= 0)
				{
					continue;
				}
				cans.push_back(j);
			}
			break;
		}
	}
	//consider the in queue
	else if(qin_num > 0 && din_num > 0)
	{
		for(i = 0; i < qsize; ++i)
		{
			if(!qin[i] || qcore[i] >= 0)
			{
				continue;
			}
			qnid = i;
			for(j = 0; j < dsize; ++j)
			{
				if(!din[j] || dcore[j] >= 0)
				{
					continue;
				}
				cans.push_back(j);
			}
			break;
		}
	}
	//consider the other queue
	else
	{
		for(i = 0; i < qsize; ++i)
		{
			if(qin[i] || qout[i])
			{
				continue;
			}
			qnid = i;
			for(j = 0; j < dsize; ++j)
			{
				if(din[j] || dout[j])
				{
					continue;
				}
				cans.push_back(j);
			}
			break;
		}
	}

	//NOTICE:if in vertex, in_queue is true and core is -1
	//TODO:update the four queue num
	int size = cans.size()	;
	for(i = 0; i < size; ++i)
	{
		int dnid = cans[i];
		//TODO:prune - structure and semantic
		qcore[qnid] = dnid;
		dcore[dnid] = qnid;
		//TODO:update structures
		dfs(num+1, io);
	}
	//TODO:restore data structure??
}

