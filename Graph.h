#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <set>
#include <climits>
#include <string>
#include <stdexcept>

using namespace std;


// nodes are essentially just their key value
// These are special commands which define
//     "nodekey_t" to mean "unsigned long long int"
//     "INVALID_NODE_KEY" to mean "ULLONG_MAX" (where ULLONG_MAX = 18446744073709551615)
#define nodekey_t unsigned long long int
#define INVALID_NODE_KEY ULLONG_MAX


// when coding, be careful to ensure that every instance of
// a GraphEdge is created on the heap
struct GraphEdge {
    nodekey_t from;
    nodekey_t to;
    unsigned int weight;
};


class Graph{

    public:
        Graph() = default;
        // copies own separate edges so destroying either graph is safe.
        Graph(const Graph& other);
        Graph& operator=(const Graph& other);
        ~Graph();


        // both of these should throw an invalid_argument exception if invalid inputs are given
        // such as a duplicate node key or a node key that doesn't exist in the graph
        void AddNode(nodekey_t key = 0);
        const GraphEdge* AddEdge(nodekey_t fromNodeKey, nodekey_t toNodeKey, unsigned int weight = 0);

        // this method returns true if the given key is in use in the graph
        bool IsPresent(nodekey_t key) const;

        // These are only used for testing/debugging
        string NodesToString() const; // all nodes
        string EdgesToString() const; // all edges

        // These next two functions both return a set since the order shouldn't matter
        // (theoretically, a graph has no order)
        // get pointers to the edges that go out from the given node
        set<const GraphEdge*> GetOutwardEdgesFrom(nodekey_t nodeKey) const;
        set<nodekey_t> GetNodes() const; // gets all the nodes in the graph

        size_t Size() const; // num edges
        size_t Order() const; // num nodes


    private:
        // Vectors preserve the insertion order required by the string methods.
        vector<nodekey_t> nodes;
        // This graph owns these heap-allocated edges; callers only borrow them.
        vector<GraphEdge*> edges;

};

#endif
