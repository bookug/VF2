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
	this->qin = new int[qsize];
	this->qout = new int[qsize];

	this->dcore = new int[dsize];
	this->din = new int[dsize];
	this->dout = new int[dsize];
}

Match::~Match()
{
	delete[] this->qcore;
	delete[] this->qin;
	delete[] this->qout;

	delete[] this->dcore;
	delete[] this->din;
	delete[] this->dout;
}

//Two States of core queues: matching [0,size) not-matching -1
//Four States of in/out queues: matching depth   in_queue depth  out_queue depth other -1
//NOTICE:a vertex maybe appear in in_queue and out_queue at the same time
void 
Match::match(IO& io)
{
	if(qsize > dsize)
	{
		return;
	}

	//initialize the structures
	memset(qcore, -1, sizeof(int) * qsize);
	memset(qin, -1, sizeof(int) * qsize);
	memset(qout, -1, sizeof(int) * qsize);
	memset(dcore, -1, sizeof(int) * dsize);
	memset(din, -1, sizeof(int) * dsize);
	memset(dout, -1, sizeof(int) * dsize);

	//call dfs with macthing num(depth)
	dfs(0, io);
}

//NOTICE:here can be different equal functions, maybe simlarity
bool 
Match::equal(LABEL lb1, LABEL lb2)
{
	return lb1 == lb2;
}

bool 
Match::checkCore(vector<Neighbor>& qlist, vector<Neighbor>& dlist)
{
	//data  another array record the mapping label? record the query is better? 2 directions
	int qnum = 0, dnum = 0; 
	int i, j, k, size1, size2;
	LABEL* temp = new LABEL[qsize];
	//NOTICE+WARN:sizeof(temp) is only 8 bytes
	//memset(temp, -1, sizeof(temp));
	memset(temp, -1, sizeof(LABEL) * qsize);

	size1 = qlist.size();
	for(i = 0; i < size1; ++i)
	{
		j = qlist[i].vid;
		if(qcore[j] >= 0)
		{
			qnum++;
			temp[j] = qlist[i].elb;
		}
	}

	size2 = dlist.size();
	for(i = 0; i < size2; ++i)
	{
		j = dlist[i].vid;
		if(dcore[j] >= 0)
		{
			//check the edge label
			if(dlist[i].elb != temp[dcore[j]])
			{
				delete[] temp;
				return false;
			}
			dnum++;
		}
	}

	delete[] temp;
	if(qnum != dnum)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool 
Match::checkOther(vector<Neighbor>& qlist, vector<Neighbor>& dlist)
{
	int qin_num = 0, qout_num = 0, qres_num = 0; 
	int din_num = 0, dout_num = 0, dres_num = 0;
	int i, j, k, size1, size2;

	size1 = qlist.size();
	for(i = 0; i < size1; ++i)
	{
		j = qlist[i].vid;
		if(qin[j] >= 0 && qcore[j] < 0)
		{
			qin_num++;
		}
		if(qout[j] >= 0 && qcore[j] < 0)
		{
			qout_num++;
		}
		if(qin[j] < 0 && qout[j] < 0)
		{
			qres_num++;
		}
	}

	//BETTER?:check if satisfy the pruning limit each time
	size2 = dlist.size();
	for(i = 0; i < size2; ++i)
	{
		j = dlist[i].vid;
		if(din[j] >= 0 && dcore[j] < 0)
		{
			din_num++;
		}
		if(dout[j] >= 0 && dcore[j] < 0)
		{
			dout_num++;
		}
		if(din[j] < 0 && dout[j] < 0)
		{
			dres_num++;
		}
	}

	if(din_num < qin_num || dout_num < qout_num || dres_num < qres_num)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool 
Match::prune()
{
#ifdef DEBUG
	if(qnid < 0 || qnid >= qsize || dnid < 0 || dnid >= dsize)
	{
		cerr<<"ERROR in prune"<<endl;
		return true;
	}
	cerr<<"qnid: "<<qnid<<"\tdnid: "<<dnid<<endl;
	for(int i = 0; i < qsize; ++i)
	{
		cerr<<qcore[i]<<" ";
	}
	cerr<<endl;
	for(int i = 0; i < dsize; ++i)
	{
		cerr<<dcore[i]<<" ";
	}
	cerr<<endl;
#endif

	vector<Neighbor>& qpred = this->query->vertices[qnid].in;
	vector<Neighbor>& qsucc = this->query->vertices[qnid].out;
	vector<Neighbor>& dpred = this->data->vertices[dnid].in;
	vector<Neighbor>& dsucc = this->data->vertices[dnid].out;

	//vertex label: semantic pruning
	if(!equal(this->query->vertices[qnid].label, this->data->vertices[dnid].label))
	{
#ifdef DEBUG
	cerr<<"prune: the vertex label not macthed"<<endl;
#endif
		return true;
	}

	//NOTICE:If not consider labels, core restrictions can be done by computing the num of qnid and dnid matching 
	//with corresponding already-matching sets, just compare the num if equal is ok
	//NOTICE:If considering labels, just build a new array for query which keeps the labels for comparison
	//This array is temporal and small
	if(!checkCore(qpred, dpred))
	{
#ifdef DEBUG
	cerr<<"prune: the pred core not macthed"<<endl;
#endif
		return true;
	}
	if(!checkCore(qsucc, dsucc))
	{
#ifdef DEBUG
	cerr<<"prune: the succ core not macthed"<<endl;
#endif
		return true;
	}

	//compute and compare 6 nums both for pred list
	if(!checkOther(qpred, dpred))
	{
#ifdef DEBUG
	cerr<<"prune: the pred other not macthed"<<endl;
#endif
		return true;
	}
	//compute and compare 6 nums both for succ list
	if(!checkOther(qsucc, dsucc))
	{
#ifdef DEBUG
	cerr<<"prune: the succ other not macthed"<<endl;
#endif
		return true;
	}	
	
	return false;
}

void 
Match::modify(int depth, vector<Neighbor>& list, int* queue)
{
	int size = list.size(), i, j;
	for(i = 0; i < size; ++i)
	{
		j = list[i].vid;
		if(queue[j] < 0)
		{
			queue[j] = depth;
		}
	}
}

void 
Match::update(int depth)
{
	qcore[qnid] = dnid;
	dcore[dnid] = qnid;
	if(qin[qnid] < 0)
		qin[qnid] = depth;
	if(qout[qnid] < 0)
		qout[qnid] = depth;
	if(din[dnid] < 0)
		din[dnid] = depth;
	if(dout[dnid] < 0)
		dout[dnid] = depth;
	
	//add the new in vertices according to qnid
	this->modify(depth, this->query->vertices[qnid].in, qin);
	//add the new out vertices according to qnid
	this->modify(depth, this->query->vertices[qnid].out, qout);
	//add the new in vertices according to dnid
	this->modify(depth, this->data->vertices[dnid].in, din);
	//add the new out vertices according to dnid
	this->modify(depth, this->data->vertices[dnid].out, dout);
}

void 
Match::restore(int depth)
{
	//if(qnid < 0 || qnid >= qsize || dnid < 0 || dnid >= dsize)
	//{
		//cerr<<"ERROR in restore"<<endl;
		//return;
	//}
	qcore[qnid] = -1;
	dcore[dnid] = -1;
	//update the query queues
	for(int i = 0; i < qsize; ++i)
	{
		if(qin[i] == depth)
		{
			qin[i] = -1;
		}
		if(qout[i] == depth)
		{
			qout[i] = -1;
		}
	}
	//update the data queues
	for(int i = 0; i < dsize; ++i)
	{
		if(din[i] == depth)
		{
			din[i] = -1;
		}
		if(dout[i] == depth)
		{
			dout[i] = -1;
		}
	}
}

void
Match::dfs(int num, IO& io)
{
	if(num == this->qsize)
	{
#ifdef DEBUG
		cerr<<"find a mapping here"<<endl;
#endif
		io.output(this->qcore, this->qsize);
		return;
	}
	//NOTICE:to avoid duplicates, only consider the smallest qid in candidates
	//int depth = num;

	//prepare the candidates from all cases
	int i, j;
	this->qnid = -1;
	vector<int> cans;

	//consider the out queue
	for(i = 0; i < qsize; ++i)
	{
		if(qout[i] < 0 || qcore[i] >= 0)
		{
			continue;
		}
		this->qnid = i;
		for(j = 0; j < dsize; ++j)
		{
			if(dout[j] < 0 || dcore[j] >= 0)
			{
				continue;
			}
			cans.push_back(j);
		}
		break;
	}
	//consider the in queue
	if(qnid == -1)
	{
		for(i = 0; i < qsize; ++i)
		{
			if(qin[i] < 0 || qcore[i] >= 0)
			{
				continue;
			}
			this->qnid = i;
			for(j = 0; j < dsize; ++j)
			{
				if(din[j] < 0 || dcore[j] >= 0)
				{
					continue;
				}
				cans.push_back(j);
			}
			break;
		}
	}
	//consider the other queue
	if(qnid == -1)
	{
		for(i = 0; i < qsize; ++i)
		{
			if(qin[i] >= 0 || qout[i] >= 0)
			{
				continue;
			}
			this->qnid = i;
			for(j = 0; j < dsize; ++j)
			{
				if(din[j] >= 0 || dout[j] >= 0)
				{
					continue;
				}
				cans.push_back(j);
			}
			break;
		}
	}

	int size = cans.size(), qnid2, dnid2;
	for(i = 0; i < size; ++i)
	{
		this->dnid = cans[i];
		if(prune())
		{
#ifdef DEBUG
			cerr<<"prune dnid: "<<dnid<<endl;
#endif
			continue;
		}
		else
		{
#ifdef DEBUG
			cerr<<"not prune dnid: "<<dnid<<endl;
#endif
		}
		update(num);
		//BETTER?:place qnid and dnid in function parameters?
		qnid2 = this->qnid; dnid2 = this->dnid;
		dfs(num+1, io);
		this->qnid = qnid2; this->dnid = dnid2;
		restore(num);
	}
}

