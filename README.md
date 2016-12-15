VF2

NOTICE:just compared with vflib2, ./run data query output
place all query graphs in memory and read data graph one by one
the maxium node num in data graphs are 213
the maxium node num in query graphs are indicated by the file name

is the data graph undirected? I think so, because all edge from smaller id to larger id
if directed, then no circle at all, impossible!
Viewed as directed and add each edge twice

compare with vflib and naughty, the latter is called the fastest, using auto-morphism groups(canonical ordering) to speed up
Another way is to build a graph library considering the common subgraphs of all data graphs, to reduce redundent matching with query graphs

