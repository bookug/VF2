/*=============================================================================
# Filename: Util.cpp
# Author: Bookug Lobert 
# Mail: 1181955272@qq.com
# Last Modified: 2016-10-24 17:23
# Description: 
=============================================================================*/

#include "Util.h"

using namespace std;

Util::Util()
{
}

Util::~Util()
{
}

long
Util::get_cur_time()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec*1000 + tv.tv_usec/1000);
}

void
Util::swapLabel(LABEL& lb1, LABEL& lb2)
{
	if(lb1 <= lb2) return;
	LABEL lb3 = lb1;
	lb1 = lb2;
	lb2 = lb3;
}

