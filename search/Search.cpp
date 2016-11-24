/*=============================================================================
# Filename: Search.cpp
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 17:44
# Description: 
=============================================================================*/

#include "Search.h"

using namespace std;

bool 
Search::PatternCompare(Pattern* a, Pattern* b)
{
	//NOTICE:we only need to compare the last element in the pattern code
	unsigned as = a->code.eles.size();
	unsigned bs = b->code.eles.size();
	if(as != bs) return as <bs;
	else if(as == 0) return true;
	return a->code.eles[as-1] < b->code.eles[bs-1];
	//Notice that back() do not report error when empty
	//return a->code.eles.back() < b->code.eles.back();
}

void 
Tree::addForwardEdge(Element& et)
{
	this->vsize++;
	this->esize++;
	//update the own rmp
	vector<VID> nmp;
	int i, j, k = this->rmp0.size();
	for(i = 0; i <= k; ++i)
	{
		j = this->rmp0[i];
		nmp.push_back(j);
		if(j == et.from)
		{
			break;
		}
	}
	//if grow from the right most vertex, then just append a vertex
	nmp.push_back(et.to);
	this->rmp0 = nmp;
}
	
void 
Tree::addBackwardEdge(Element& et)
{
	this->esize++;
	//the rmp not change
}

void 
Search::GenerateMappings(Pattern* edgePattern)
{
	Mapping* mp = NULL;
	int i, j, k;
	int gnum = this->gs.size();
	OneEdge et, pet;
	pet.from = edgePattern->getVertex1LabelByEdge(0);
	pet.to = edgePattern->getVertex2LabelByEdge(0);
	pet.pred = edgePattern->getLabelByEdge(0);

	//find all rightmost path mappings in each graph
	for(i = 0; i < gnum; ++i)
	{
		j = gs[i]->eSize();
		for(k = 0; k < j; ++k)
		{
			et.pred = gs[i]->getLabelByEdge(k);
			et.from = gs[i]->getVertex1LabelByEdge(k);
			et.to = gs[i]->getVertex2LabelByEdge(k);
			//Util::swapLabel(et.from, et.to);
			if(et != pet)
			{
				continue;
			}
			//cerr<<"mapping "<<i<<" "<<k<<" "<<et.from<<" "<<et.to<<" "<<et.pred<<endl;
			//cerr<<gs[i]->getVertex1IDByEdge(k)<<" "<<gs[i]->getVertex2IDByEdge(k)<<endl;
			if(edgePattern->lastSupport() != i)
			{
				mp = new Mapping(i);
				edgePattern->addSupport(mp);
			}
			mp = edgePattern->lastMapping();
			vector<VID> path;
			//NOTICE: one edge restriction -- the first label <= the second label
			path.push_back(gs[i]->getVertex1IDByEdge(k));
			path.push_back(gs[i]->getVertex2IDByEdge(k));
			mp->addPath(path);
			vector<EID> eset;
			eset.push_back(k);
			mp->addEdge(eset);
			if(et.from == et.to) //this is a special case, we must do 2-direction mappings
			{
				path.clear();
				path.push_back(gs[i]->getVertex2IDByEdge(k));
				path.push_back(gs[i]->getVertex1IDByEdge(k));
				mp->addPath(path);
				mp->addEdge(eset);
			}
		}
	}
}

//NOTICE:if graphs are removed, the support gid maybe not all right(only right for the current)
//BETTER:keep original gid in graph itself if details are needed(gid in support is also the current pos)
//the current output gid is teh GID after projection!!!
void 
Search::GraphProjection(Pattern* edgePattern)
{
	vector<Mapping*>& support = edgePattern->support;
	int msize = support.size(), i, j;
	for(i = 0; i < msize; ++i)
	{
		int gid = support[i]->gid;
		Graph* graph = this->gs[gid];
		vector< vector<EID> >& amps = support[i]->amps;
		int asize = amps.size();
		for(j = 0; j < asize; ++j)
		{
			int eid = amps[j][0];
			//cerr<<gid<<" "<<eid<<" "<<graph->edges[eid]->from<<" "<<graph->edges[eid]->to<<" "<<graph->edges[eid]->label<<endl;
			delete graph->edges[eid];
			graph->edges[eid] = NULL;
		}	
		graph->rebuild();
	}
	//remove invalid graphs
	int gsize = this->gs.size();
	vector<Graph*> newGS;
	for(i = 0; i < gsize; ++i)
	{
		if(this->gs[i]->eSize() > 0)
		{
			newGS.push_back(this->gs[i]);
		}
		else
		{
			//cerr<<"the "<<i<<" graph empty!"<<endl;
		}
	}
	this->gs = newGS;
}

bool 
Search::run(int MaxPatternVSize, int MaxPatternESize)
{
	this->maxPatternVSize = maxPatternVSize;
	this->maxPatternESize = maxPatternESize;
	
	int size = this->front.size(), i;
	for(i = 0; i < size; ++i)
	{
		//cerr<<"the "<<i<<" run"<<endl;
		//cerr<<"graph num: "<<this->gs.size()<<endl;
		GenerateMappings(this->front[i]);
		//cout<<front[i]->getVertex1LabelByEdge(0)<<" "<<front[i]->getVertex2LabelByEdge(0)<<endl;
		SubgraphMining(this->front[i]);
		//sub this edge from all its mappings in all data graphs when finished this turn
		//first set all mapping edges to -1, then remove all isolate vertices
		//Finally, remove all invalid graphs(edge count = 0)
		//WARN:removing this may cause other fronters to be invalid!!!
		//better to compute the mappings of next fronter just before used!!!
		GraphProjection(this->front[i]);

		delete this->front[i];
		//if the total num of gs < border, quit now    
		if(this->gs.size() < this->border)
		{
			break;
		}
	
	//no need to update the maxium edge count and vertex count(limit the pattern size)
	//because pattern size is limited by growing
	}

	return true;
}

void 
Search::SubgraphMining(Pattern* ptn)
{
	int esize = ptn->eSize();
	//single edge has been discussed before
	if(esize > 1 && NotMiniumCode(ptn))
	{
		delete ptn;
		return;
	}
	//then this is the minium code
	
	//this->result.push_back(ptn);
	//ptn->pid = this->result.size() - 1;
	io.output(*ptn);
	//PID pid = ptn->pid;

	vector<Pattern*> childs;
	//generate all possible children with one edge growth
	OneEdgeGrow(ptn, childs);

	int csize = childs.size();
	for(int i = 0; i < csize; ++i)
	{
		if(childs[i]->frequence() < this->border)
		{
			delete childs[i];
			childs[i] = NULL;
			continue;
		}
		else
		{
			ptn->valid.insert(childs[i]->code.eles.back());
		}
	}

	//if children is also frequent, then mining recursively(depth first search)
	for(int i = 0; i < csize; ++i)
	{
		if(childs[i] == NULL) continue;
		childs[i]->parent = ptn;
		SubgraphMining(childs[i]);
	}

	//NOTICE:single edge is used in run() to do graph projection
	if(esize > 1)
	{
		delete ptn;
	}
}


//One Method is to sort on all labels, and choose from theh minium to the current one
//decide if the code can form a dfs tree(but this is costly), and the whole search space is n!
//while the dfs trees num can be much smaller
//
//Another Method:compute the exact minium code is too costly, we just try to verify there is a smaller dfs code for this kind of graph 
//The pruning strategy maybe prune many, but the dfs computing is really costly, especially for the checking of graph with minium code
//NOTICE:we decide to limit the tree num or search depth, when over then we think this is the minium and continue(no loss but maybe duplicates)
//Meanwhile, we try dfs greedily, always select a smaller label to go on
//
//Best Method:do dfs code search, compare the current minium element and decide if continue
bool
Search::NotMiniumCode(Pattern* ptn)
{
	//DEBUG:just for debugging
	//return false;
	unsigned esize = ptn->eSize();
	bool* visit = new bool[esize]; //the index is eid
	memset(visit, 0, sizeof(bool)*esize);
	//int count = 0;  //total dfs tree searched
	//int depth = 0;  //current searching depth
	int vsize = ptn->vSize();
	bool* node = new bool[vsize]; //if a node of this pattern is visited
	memset(node, 0, sizeof(bool)*vsize);
	Tree tp;

	for(int i = 0; i < vsize; ++i)
	{
		tp = Tree(i);
		node[i] = true;
		bool ret = dfs(ptn, visit, node, tp);
		node[i] = false;
		if(ret)
		{
			delete[] visit;
			delete[] node;
			return true;
		}
	}

	delete[] visit;
	delete[] node;
	//NOTICE:this means ptn is the minium pattern code, and we may use it to decide whether this is the first time to visit this minium code
	return false;
}

bool 
Search::dfs(Pattern* ptn, bool* visit, bool* node, Tree& tree)
{
	if(tree.esize >= ptn->eSize())
	{
		//NOTICE:already impossible to be smaller
		//This judge is needed here to avoid pet invalid read
		return false;
	}

	int i, j, k;
	vector<VID>& tpath = tree.rmp0;
	vector<VID>& ppath = tree.rmp1;
	int vlen = ppath.size();
	Element et;
	Element pet = ptn->code.eles[tree.esize];
	Tree tp;
	//NOTICE:to save space, may use Tree* and delte after generating all children
	//However, this requires care when directly return true
	vector<Tree> childs; 
	vector<EID> cmap;
	vector<VID> vmap;

	//BETTER:using an array directly
	map<VID, int> pos; //map vid with the position in ppath
	map<VID, int>::iterator posit;
	for(i = 0; i < vlen; ++i)
	{
		int vid = ppath[i];
		pos[vid] = i;
	}

	//each vertex in rightmost path to grow
	for(k = vlen-1; k >= 0; --k)
	{
		int pvid = ppath[k];
		int tvid = tpath[k];
		vector<Neighbor>& neigh = ptn->getNeighborByVertex(pvid);
		int nsize = neigh.size();
		bool last = (k == vlen-1); //if the rightmost veretx

		bool extend = false;
		//each neighbor to consider
		for(j = 0; j < nsize; ++j)
		{
			int eid = neigh[j].eid;
			if(visit[eid])
			{
				continue;
			}
			VID id2 = ptn->getEdgeNeighbor(eid, pvid);
			LABEL lb1 = ptn->getLabelByVertex(pvid);
			LABEL lb2 = ptn->getLabelByVertex(id2);
			VID nid = tree.vsize;
			bool backward = false;
			posit = pos.find(id2);
			//do not consider this in earlier vertices, backward just at last
			if(posit != pos.end())
			{
				if(!last) continue;
				//this is a backward edge
				backward = true;
				nid = tpath[posit->second];
			}
			else if(node[id2]) //an internal node(not rightmost path)
			{
				continue;	
			}

			extend = true;
			LABEL elb = ptn->getLabelByEdge(eid);
			et = Element(tvid, nid, lb1, elb, lb2);
			//NOTICE:we need to ensure that in current stage, no one is smaller, then we can go on searching
			if(et < pet)
			{
				//a smaller code found
				return true;
			}
			else if(pet < et)
			{
				//then this dfs search path can be given up
			}
			else
			{
				//==, add this to childs
				tp = Tree(tree);
				if(backward)
				{
					tp.addBackwardEdge(et);
				}
				else
				{
					tp.addForwardEdge(et);
					vector<VID> nmp;
					for(i = 0; i <= k; ++i)
					{
						nmp.push_back(tree.rmp1[i]);
					}
					nmp.push_back(id2);
					tp.rmp1 = nmp;
				}
				childs.push_back(tp);
				cmap.push_back(eid);
				vmap.push_back(id2);
			}
		}

		//later element can only be greater!
		if(extend)
		{
			break;
		}
	}
	
	//do dfs search on the current same codes
	//QUERY?:maybe BFS is better to find a smaller one earlier? but costly!!!
	int csize = childs.size();
	for(i = 0; i < csize; ++i)
	{
		visit[cmap[i]] = true;
		node[vmap[i]] = true;
		bool ret = dfs(ptn, visit, node, childs[i]);
		if(ret)
		{
			return true;
		}
		visit[cmap[i]] = false;
		node[vmap[i]] = false;
	}

	return false;
}

void
Search::OneEdgeGrow(Pattern* ptn, vector<Pattern*>& childs)
{
//NOTICE:if not require to output the frequent pattern, just the num or rightmost path, we can just
//discard vertices and edges in pattern. In fact, we can rebuild whole pattern in output by rmp and mapping edge set

	//build a map to record the same pattern to count
	//maybe in a graph there are several same codes for given rmps
	map<Element, Pattern*> cm;
	map<Element, Pattern*>::iterator cmit;
	Element et;
	int i, j, gnum = ptn->support.size();

	//each data graph
	for(i = 0; i < gnum; ++i)
	{
		Mapping* mp = ptn->support[i];
		Graph* gp = this->gs[mp->gid];
		vector< vector<VID> >& rmps = mp->rmps;
		vector< vector<EID> >& amps = mp->amps;
		int pnum = rmps.size();

		//each mapping case
		for(j = 0; j < pnum; ++j)
		{
			//NOTICE:only grow from one edge, from one of the rightmost path
			//backward edge can only grows from the last vertex(the other vertex is in rmp)
			//all vertices before can only grow forward edges(the other veretx is not in rmp)
			this->OneMappingGrow(ptn, cm, gp, rmps[j], amps[j], mp->gid);
		}
	}

	//cerr<<"child map size: "<<cm.size()<<endl;
	//push all generated children to childs[]
	for(cmit = cm.begin(); cmit != cm.end(); ++cmit)
	{
		//NOTICE:map from begin() to end() is already ordered, no need to sort again
		childs.push_back(cmit->second);
	}
	//NOTICE:ensure that we must do depth-first-search on dfs code tree
	//the growing order must be stricitly, to ensure the minium is found early
	//To ensure this, just ensure the pattern code order
	//The basic code is the same, to compare the new code is to compare the last element
	//sort(childs.begin(), childs.end(), Search::PatternCompare);
}

//STRATEGY:use the frequence antimonotone
//father->child1 (x z) 
//      ->child2 (x y) -> child21(x y z) (if xz is valid code, require that xz must be frequent)
void
Search::OneMappingGrow(Pattern* ptn, map<Element, Pattern*>& cm, Graph* gp, vector<VID>& gpath, vector<EID>& geset, GID gid)
{
	//NOTICE:the edge added can not be smaller than this, otherwise the minium code of this child will be smaller
	//Element limit_ele = ptn->code.eles[0];
	//OneEdge limit = OneEdge(limit_ele.lb1, limit_ele.lb2, limit_ele.elb);
	//OneEdge tmp;
	//OneEdge tmp2;

	vector<VID>& ppath = ptn->rmp;
	int gesize = gp->eSize();
	int gvsize = gp->vSize();
	int elen = ppath.size();
	int vsize = ptn->vSize();
	Element et;
	map<Element, Pattern*>::iterator cmit;
	int i, k, q;
	bool* visit = new bool[gesize];
	memset(visit, 0, sizeof(bool)*gesize);
	//if a node has been visited, om to be in rightmost path
	bool* node = new bool[gvsize];
	memset(node, 0, sizeof(bool)*gvsize);
	int esize = geset.size();
	for(k = 0; k < esize; ++k)
	{
		visit[geset[k]] = true;
		node[gp->getVertex1IDByEdge(geset[k])] = true;
		node[gp->getVertex2IDByEdge(geset[k])] = true;
	}

	//QUERY?:change pos to int[]
	map<VID, int> pos; //map vid with the position in gpath
	map<VID, int>::iterator posit;
	for(i = 0; i < elen; ++i)
	{
		pos[gpath[i]] = i;
	}

	//each vertex in rightmost path to grow
	for(k = elen-1; k >= 0; --k)
	//for(k = 0; k < elen; ++k)
	{
		int gvid = gpath[k];
		int pvid = ppath[k];
		vector<Neighbor>& neigh = gp->getNeighborByVertex(gvid);
		int nsize = neigh.size();
		//cerr<<"neighbor size: "<<nsize<<endl;
		bool last = (k == elen-1);

		//each neighbor to consider
		for(q = 0; q < nsize; ++q)
		{
			if(visit[neigh[q].eid])
			{
				continue;
			}
			else
			{
				//cerr<<"not visited!"<<endl;
			}
			VID id2 = gp->getEdgeNeighbor(neigh[q].eid, gvid);
			LABEL lb1 = gp->getLabelByVertex(gvid);
			LABEL lb2 = gp->getLabelByVertex(id2);
			//NOTICE:the dfs order and graph projection ensures that this is unnecessary
			//tmp.from = gp->getVertex1LabelByEdge(neigh[q].eid);
			//tmp.to = gp->getVertex2LabelByEdge(neigh[q].eid);
			//tmp.pred = gp->getLabelByEdge(neigh[q].eid);
			//if(tmp < limit)
			//{
				//continue;
			//}

			VID nid = vsize;
			bool backward = false;
			//if(gpath.size()==3&&ptn->eSize()==5)
			//{
				//cerr<<"found the path"<<endl;
				//cerr<<gpath[0]<<" "<<gpath[1]<<" "<<gpath[2]<<endl;
				//for(posit = pos.begin(); posit != pos.end(); ++posit)
					//cerr<<posit->first<<" ";
				//cerr<<endl<<gid<<" "<<id2<<endl;
			//}
			posit = pos.find(id2);
			if(posit != pos.end())
			{
				if(!last) continue;
				//this is a backward edge
				backward = true;
				nid = ppath[posit->second];
			}
			else if(node[id2])  //an internal node
			{
				continue;
			}

			//NOTICE:this is also a strategy, but itself maybe too costly!
			//not smaller than any connecting ones, maybe we can keep the maxium edge for each right path vertex
			//if(backward)
			//{
				//vector<Neighbor>& neigh2 = gp->getNeighborByVertex(id2);
				//int nsize2 = neigh2.size();
				//bool valid = true;
				//for(int tt = 0; tt < nsize2; ++tt)
				//{
					//if(!visit[neigh2[tt].eid])
					//{
						//continue;
					//}
					//tmp2.from = gp->getVertex1LabelByEdge(neigh2[tt].eid);
					//tmp2.to = gp->getVertex2LabelByEdge(neigh2[tt].eid);
					//tmp2.pred = gp->getLabelByEdge(neigh2[tt].eid);
					//if(tmp < tmp2)
					//{
						//valid = false;
						//break;
					//}
				//}
				//if(!valid)
				//{
					//continue;
				//}
			//}

			//cerr<<"a mapping will be built!"<<endl;
			LABEL elb = gp->getLabelByEdge(neigh[q].eid);
			//pvid < vsize, this must be a forward edge
			et = Element(pvid, nid, lb1, elb, lb2);
			//NOTICE:frequence antimonotone pruning
			if(ptn->eSize() > 1)
			{
				Element tele = et;
				bool validElement = false;
				Element lastElement = ptn->code.eles.back();
				if(lastElement.from > lastElement.to)  //backward egde
				{
					validElement = true;
				}
				else //forward edge
				{
					if(tele.from != lastElement.to)  //some foward edge and all backwards invalid
					{
						validElement = true;
					}
				}
				if(validElement)
				{
					if(tele.from < tele.to)
					{
						tele.to--; //go back to check
					}
					if(ptn->parent->valid.find(tele) == ptn->parent->valid.end())
					{
						continue;
					}
				}
			}


			cmit = cm.find(et);
			Pattern* nptn = NULL;
			//found a child pattern
			if(cmit == cm.end())
			{
				//cerr<<"a new child pattern"<<endl;
				nptn = new Pattern(ptn);
				cm[et] = nptn;
				if(backward)
				{
					nptn->addBackwardEdge(et);
				}
				else
				{
					nptn->addForwardEdge(et);
					vector<VID> nrmp;
					for(int idx = 0; idx <= k; ++idx)
					{
						nrmp.push_back(ppath[idx]);
					}
					nrmp.push_back(nid);
					nptn->rmp = nrmp;
				}
			}
			else
			{
				//cerr<<"a existing child pattern"<<endl;
				nptn = cmit->second;
			}

			Mapping* nmp = NULL;
			//add this mapping
			if(nptn->lastSupport() != gid)
			{
				nmp = new Mapping(gid);
				nptn->addSupport(nmp);
			}
			nmp = nptn->lastMapping();
			//add current mapping, backward edge not change the rightmost path
			vector<VID> npath;
			if(backward)
			{
				npath = gpath;
			}
			else
			{
				for(int idx = 0; idx <= k; ++idx)
				{
					npath.push_back(gpath[idx]);
				}
				npath.push_back(id2);
			}
			vector<EID> neset = geset;
			neset.push_back(neigh[q].eid);
			nmp->addPath(npath);
			nmp->addEdge(neset);
		}
	}

	delete[] visit;
	delete[] node;
}

