/*=============================================================================
# Filename: IO.h
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 22:55
# Description: 
=============================================================================*/

#ifndef _IO_IO_H
#define _IO_IO_H

#include "../util/Util.h"
#include "../graph/Pattern.h"

class IO
{
public:
	IO();
	IO(std::string data, std::string file);
	bool input(std::vector<Graph*>& gs);
	bool output(Pattern& g, int frequence = -1);
	void flush();
	~IO();
private:
	FILE* ifp;
	FILE* ofp;
};

#endif

