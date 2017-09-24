VF2

WARN: vf2算法的第五个剪枝条件只适用于induced subgraph matching
查询图里两点没边，在数据图的匹配中对应的两点也不能有边，这样第五个剪枝条件（二步邻居约束）才能成立。
可看《子图匹配三十年》，一个各种子图匹配算法的综述。
./run data.graph query.graph
查询结果默认在ans.txt中

NOTICE:just compared with vflib2, ./run data query output
place all query graphs in memory and read data graph one by one
the maxium node num in data graphs are 213
the maxium node num in query graphs are indicated by the file name

is the data graph undirected? I think so, because all edge from smaller id to larger id
if directed, then no circle at all, impossible!
Viewed as directed and add each edge twice

compare with vflib and naughty, the latter is called the fastest, using auto-morphism groups(canonical ordering) to speed up
vflib2中要求节点个数不能超过65535，而且它的子图同构是指最标准的格式，而非子图包含，即要求边是相互对应的
Another way is to build a graph library considering the common subgraphs of all data graphs, to reduce redundent matching with query graphs


