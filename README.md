# WaterPipes
After several years and considerable economic investments, finally the state of Grapha-Nui has its dam which can produce electricity and provide drinking water to the citizens.
It is necessary, however, to finish the water network so that all cities receive water.
To this end, a computer scientist is summoned and given the map of the city with the current pipelines, he is supposed to determine the minimum number of pipelines to be built.

## Input data
It is given an input file containing in the first row two integers, 1 <= N <= 1000 and 
0 <= P <= 1000, separated by a space that represent, respectively, the number of cities and water pipes of the graph. With “0” we denote the watershed.
The following P rows, contain two integers, separated by a space, which represent an existing water pipe from the city C1 to C2.

## Output data
Determine the minimum number and which water pipes build so as to bring the water in all the cities.  
Assumptions:   
1 ≤ N ≤ 1000   # of cities   
0 ≤ P ≤ 10000 	 # of existing water pipes  


## Example of graph  
As we can see, the input graph is directed and unweighted, furthermore the source node (in red) has no outgoing edges.  
<img src="https://github.com/Salvatore-tech/WaterPipes/blob/master/resources/input_graph.png" width="500" height="500">  

## Algorithm
    1. Initialize all the nodes of the graph {N} as not reachable
    2.  Run a DFS from the source node {S} and mark the visited nodes as reachable {R }
    3.  Compute the vector of not reachable nodes {V}: {N} - {R} (subtracting all the reachable nodes from the nodes of the graph)
    4.  For each not reachable node {NRN} in {V}
         •  Run a DFS(NRN) and the neighbours that are not reachable from source as well {NRNEIGH}
         • Subtract the neighbours of the current node: {NRN} - {NRNEIGH}
   	    end loop
    5. For each element {E} in {NRN}
        • Create an edge from source {S} to {E} 
  	   end loop   
       
Running the algorithm on the graph presented above, we're going to build 14 edges from the source node to these nodes:  2 4 7 8 9 10 16 17 18 19 20 21 22 23 in order to bring water in every city with the minimum number of pipes.
