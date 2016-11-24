/*=============================================================================
# Filename: Code.h
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-25 16:29
# Description: 
=============================================================================*/

#ifndef _SEARCH_CODE_H
#define _SEARCH_CODE_H

#include "../util/Util.h"

//this represents one edge of a graph according to the order of a generating tree
class Element
{
public:
	VID from;
	VID to;
	LABEL lb1;
	LABEL elb;
	LABEL lb2;
	Element()
	{
		from = to = -1;
		lb1 = elb = lb2 = -1;
	}
	Element(VID _from, VID _to, LABEL _lb1, LABEL _elb, LABEL _lb2):from(_from),to(_to),lb1(_lb1),elb(_elb),lb2(_lb2)
	{
	}
	bool operator<(const Element& ele) const;
};

//this represents a dfs code of a graph
class Code
{
public:
	std::vector<Element> eles;
	Code()
	{
	}
	bool operator<(const Code& cod) const;
	void addElement(const Element& ele);
	int size() const;
};

#endif

