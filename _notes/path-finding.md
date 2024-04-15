## Graph


## Breadth-First Search
this algorithm only considers the further
nodes once the closer nodes are exhausted, it won’t miss
the shortest path to the cheese.


To begin, you enqueue the start node and enter a loop. 
In each iteration, you dequeue a node and enqueue its neighbors. 
You can avoid adding the same node multiple times to the queue by checking the parent map. 
A node’s parent is null only if the node hasn’t been enqueued before or it’s the start node.

BFS doesn’t look at the weight of the edges at all; every edge traversal is equivalent.
Another issue with BFS is that it tests nodes even if they are in the opposite direction of the goal.


Most other pathfinding algorithms used in games have an overall structure like BFS. On every iteration, you pick
one node to inspect next and add its neighbors to a data structure. What changes is that different pathfinding
algorithms **evaluate nodes in different orders**.


## Greedy Best-First Search



## A* Search



## GAME TREES
