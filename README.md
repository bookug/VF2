VF2

TODO:remove blank lines in graph data
why graph num of a query > 1
how does this isomorphism mean?

how to avoid duplicates?
how to filter in linear time?
how to select the order(always one by one, dfs)
is the data graph undirected? I think so, because all edge from smaller id to larger id
if directed, then no circle at all, impossible!

compare with vflib and naughty, the latter is called the fastest, using auto-morphism groups(canonical ordering) to speed up
Another way is to build a graph library considering the common subgraphs of all data graphs, to reduce redundent matching with query graphs

./dig file 0.07 
0.1 for big graph, while 0.7 for small graph
0.05 0.1 0.15 0.2 0.25 0.3
0.4 0.5 0.6 0.7 0.8 0.9

Target: frequent subgraphs, connected, labeled, undirected, single-machine, embedded instead of induced
Require: all graphs can be stored in memory(if not, algorithm may have to change)
Parameter: enum different support(for example, 6.5%), and lower the corresponding time(for example 10min)
Accuracy: compare with gSpan library online or others programs, almost is ok

Some References:
http://www.cs.ucsb.edu/~xyan/software/gSpan.htm
http://www.tuicool.com/articles/FvaEJju
source codes on github

Thought:
1. keeping matching of rightmost path in each data graph for a pattern
2. strict dfs code order in searching space 

TODO:
bugs exist(more results)
induced version
distributed version

Notes:
special graphs to consider    
v 0 2 
v 1 2
v 2 2
v 3 2
v 4 2
v 5 2
e 0 1 2
e 0 2 2
e 1 2 2
e 0 3 2
e 1 4 2
e 2 5 2

one problem is that a false pattern will be output:
2-2-2-2
  | |
  2 2

2-(2)-3-(3)-3-(2)-2
The wrong graph si generated from v3-v0-v1-v2
                                        |
										v4
original rightmost path is v3-v0-v1-v2, now is v3-v0-v1-v4, so we can grow from v0 to v2 again!!!
This is impossible! when growing, it can point to vertex in the rightmost path, but not the internal node!!!


Notice the difference between > and >= when considering minsup, and the float2integer is also a problem!
this may cause result different, but this is not our fault!

