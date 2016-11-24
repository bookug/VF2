/*=============================================================================
# Filename: dig.cpp
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 14:39
# Description: 
=============================================================================*/

#include "../util/Util.h"
#include "../graph/Pattern.h"
#include "../search/Search.h"
#include "../io/IO.h"

using namespace std;

//NOTICE:a pattern occurs in a graph, then support++(not the matching num in a graph), support/N >= minsup
vector<Graph*> gs;
vector<Pattern*> front;
map<LABEL, int> vertices;
map<OneEdge, int> edges;
int gnum;
unsigned border;

void
RemoveAllInfrequent()
{
	int i, j, k, t;
	OneEdge ed;

	for(i = 0; i < gnum; ++i)
	{
		int esize = gs[i]->eSize();
		for(j = 0; j < esize; ++j)
		{
			ed.from = gs[i]->getVertex1LabelByEdge(j);
			ed.to = gs[i]->getVertex2LabelByEdge(j);
			ed.pred = gs[i]->getLabelByEdge(j);
			if(edges.find(ed) == edges.end()) // not frequent
			{
				delete gs[i]->edges[j];
				gs[i]->edges[j] = NULL;
			}
		}
		gs[i]->rebuild();
	}
}

int
main(int argc, const char * argv[])
{
	string output = "ans.txt";
	if(argc > 4 || argc < 3)
	{
		cerr<<"invalid arguments!"<<endl;
		return -1;
	}
	string data = argv[1];
	double minsup = atof(argv[2]);
	if(argc == 4)
	{
		output = argv[3];
	}

	cerr<<"args all got!"<<endl;
	long t1 = Util::get_cur_time();

	IO io = IO(data, output);
	//read input file and keep the graph in memory
	io.input(gs);
	
	cerr<<"input ok!"<<endl;
	long t2 = Util::get_cur_time();

	gnum = gs.size();
	//frequence of a pattern must > border
	border = (double)gnum * minsup;
	//the size of a pattern can not be greater than this
	int maxPatternVSize = 0;
	int maxPatternESize = 0;
	//BETTER:find maxium label?

	//record the frequence of single vertex or edge
	int i, j, k, t;
	LABEL lb;
	OneEdge et;
	map<LABEL, int>::iterator mvit;
	map<OneEdge, int>::iterator meit;
	set<LABEL>::iterator vit;
	set<OneEdge>::iterator eit;

	//count vertices and edges
	for(i = 0; i < gnum; ++i)
	{
		j = gs[i]->vSize();
		if(j > maxPatternVSize)
		{
			maxPatternVSize = j;
		}
		//add to vertices to count frequence
		set<LABEL> uniqv;
		for(k = 0; k < j; ++k)
		{
			lb = gs[i]->getLabelByVertex(k);
			uniqv.insert(lb);
		}
		for(vit = uniqv.begin(); vit != uniqv.end(); ++vit)
		{
			if(vertices.find(*vit) != vertices.end())
			{
				vertices[*vit]++;
			}
			else
			{
				vertices[*vit] = 1;
			}
		}

		j = gs[i]->eSize();
		if(j > maxPatternESize)
		{
			maxPatternESize = j;
		}
		//add to edges to count frequence
		set<OneEdge> uniqe;
		for(k = 0; k < j; ++k)
		{
			et.pred = gs[i]->getLabelByEdge(k);
			et.from = gs[i]->getVertex1LabelByEdge(k);
			et.to = gs[i]->getVertex2LabelByEdge(k);
			//to ensure from<=to? no need
			//Util::swapLabel(et.from, et.to);
			uniqe.insert(et);
		}
		for(eit = uniqe.begin(); eit != uniqe.end(); ++eit)
		{
			if(edges.find(*eit) != edges.end())
			{
				edges[*eit]++;
			}
			else
			{
				edges[*eit] = 1;
			}
		}
	}

	cerr<<"gnum: "<<gnum<<endl;
	cerr<<"border: "<<border<<endl;
	cerr<<"vsize: "<<maxPatternVSize<<endl;
	cerr<<"esize: "<<maxPatternESize<<endl;

	cerr<<"vertices and edges counted!"<<endl;
	long t3 = Util::get_cur_time();
	//QUERY:if need to sort vertices and edges on frequence first?

	//add frequent one vertex to output/result
	//NOTICE:we need to keep the pattern id valid
	//map<LABEL, Pattern*> vp;
	//for(mvit = vertices.begin(); mvit != vertices.end(); ++mvit)
	//{
		//if(mvit->second > border)
		//{
			//Pattern* ptn = new Pattern(mvit->first);
			////NOTICE:to compare with others, not output these
			////io.output(*ptn, mvit->second);
			//delete ptn;
			////result.push_back(ptn);
			////ptn->pid = result.size() - 1;
			////vp[mvit->first] = ptn;
		//}
	//}

	vertices.clear();
	cerr<<"single vertex pattern dealed!"<<endl;
	//Query:if we can start directly from a point, instead of an edge?which is better?
	//NOTICE:use DFS instead of BFS for graph searching(not efficient for pruning and memory cost)
	
	//set one edge patterns to init
	map<OneEdge, Pattern*> ep;
	map<OneEdge, Pattern*>::iterator epit;
	for(meit = edges.begin(); meit != edges.end();)
	{
		if(meit->second >= border)
		{
			Pattern* ptn = new Pattern(meit->first);
			front.push_back(ptn);
			ep[meit->first] = ptn;
			meit++;
		}
		else
		{
			//Notice that map erase returns void
			//this way it++ move forward before remove, then return the original iterator to remove
			edges.erase(meit++);
		}
	}

	cerr<<"single edge pattern built!"<<endl;
	long t4 = Util::get_cur_time();

	//remove all infrequent edges and vertices
	RemoveAllInfrequent();
	//sort front on DFS lexicographic order, to ensure code-dfs
	sort(front.begin(), front.end(), Search::PatternCompare);

	edges.clear();
	cerr<<"remove and sort!"<<endl;
	long t5 = Util::get_cur_time();

	//search the frequent pattern recursively
	Search srh(gs, front, io, border);
	srh.run(maxPatternVSize, maxPatternESize);
	
	cerr<<"search ended!"<<endl;
	long t6 = Util::get_cur_time();

	//output the time for contrast
	cerr<<"part 1 used: "<<(t2-t1)<<"ms"<<endl;
	cerr<<"part 2 used: "<<(t3-t2)<<"ms"<<endl;
	cerr<<"part 3 used: "<<(t4-t3)<<"ms"<<endl;
	cerr<<"part 4 used: "<<(t5-t4)<<"ms"<<endl;
	cerr<<"part 5 used: "<<(t6-t5)<<"ms"<<endl;

	//release all and flush cached writes
	front.clear();
	gnum = gs.size();
	for(i = 0; i < gnum; ++i)
	{
		delete gs[i];
	}
	gs.clear();
	io.flush();
	return 0;
}

