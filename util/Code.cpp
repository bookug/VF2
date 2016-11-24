/*=============================================================================
# Filename: Code.cpp
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-25 16:30
# Description: 
=============================================================================*/

#include "Code.h"

using namespace std;

bool 
Element::operator<(const Element& ele) const
{
	//BETTER:keep a backward bool in Element to save comparisions
	bool backward1 = this->from > this->to;
	bool backward2 = ele.from > ele.to;

	if(backward1 && !backward2) 
	{
		return true;
	}
	//else if(!backward1 && backward2)
	//{
		//return false;
	//}
	else if(backward1 && backward2)
	{
		if(this->to < ele.to) return true;
		if(this->to == ele.to && this->elb < ele.elb) return true;
	}
	else //if(!backward1 && !backward2)
	{
		if(ele.from < this->from) return true;
		else if(ele.from == this->from)
		{
			if(this->lb1 < ele.lb1) return true;
			else if(this->lb1 == ele.lb1)
			{
				if(this->elb < ele.elb) return true;
				else if(this->elb == ele.elb && this->lb2 < ele.lb2) return true;
			}
		}
	}

	return false;
}

bool 
Code::operator<(const Code& code) const
{
	int m = this->size();
	int n = code.size();
	int min = (m <= n)?m:n;
	for(int i = 0; i < min; ++i)
	{
		if(this->eles[i] < code.eles[i]) return true;
	}
	if(n > m) return true;
	return false;
}

void 
Code::addElement(const Element& ele)
{
	this->eles.push_back(ele);
}

int 
Code::size() const
{
	return this->eles.size();
}

