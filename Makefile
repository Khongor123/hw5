all:
	g++ -std=c++17 -Wall -g Graph.cpp GraphTests.cpp -o graph-tests
	g++ -std=c++17 -Wall -g Graph.cpp BetterPriorityQueue.cpp BPQTests.cpp -o bpq-tests
	g++ -std=c++17 -Wall -g Graph.cpp BetterPriorityQueue.cpp Dijkstra.cpp DijkstraTests.cpp -o dijkstra

