# Kevin-Bacon-Number-C
Finding the first actor and second actor links between the information in the input text file given according to the Breadth First Search algorithm and Finding Kevin Bacon Number.


How it works?

1-)Due to the large number of actors and movies, 2 hash tables(films and actors) were created.(For ease of searching)

2-)Performing Breadth First Search operation in the created Hash table.

3-)How BFS works?

a) A queue (FIFO-First in First Out) data structure is used by BFS.

b) You mark any node in the graph as root and start traversing the data from it.

c) BFS traverses all the nodes in the graph and keeps dropping them as completed.

d) BFS visits an adjacent unvisited node, marks it as done, and inserts it into a queue.

e) Removes the previous vertex from the queue in case no adjacent vertex is found.

f) BFS algorithm iterates until all the vertices in the graph are successfully traversed and marked as completed.

g) There are no loops caused by BFS during the traversing of data from any node.
