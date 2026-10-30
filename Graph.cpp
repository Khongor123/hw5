#include "Graph.h"
#include <sstream>

Graph::Graph(const Graph &other)
{
    // build a temporary owner so partially copied edges are freed if copying fails.
    Graph copy;
    copy.nodes = other.nodes;
    copy.edges.reserve(other.edges.size());
    for (const GraphEdge *edge : other.edges)
    {
        copy.edges.push_back(new GraphEdge(*edge));
    }
    nodes.swap(copy.nodes);
    edges.swap(copy.edges);
}

Graph &Graph::operator=(const Graph &other)
{
    // swapping with a complete copy also handles self-assignment safely.
    Graph copy(other);
    nodes.swap(copy.nodes);
    edges.swap(copy.edges);
    return *this;
}

Graph::~Graph()
{
    for (GraphEdge *edge : edges)
    {
        delete edge;
    }
}

void Graph::AddNode(nodekey_t key)
{
    if (IsPresent(key))
    {
        throw invalid_argument("Node key already exists");
    }
    nodes.push_back(key);
}

const GraphEdge *Graph::AddEdge(nodekey_t fromNodeKey, nodekey_t toNodeKey, unsigned int weight)
{
    if (!IsPresent(fromNodeKey) || !IsPresent(toNodeKey))
    {
        throw invalid_argument("Both edge endpoints must exist");
    }
    for (const GraphEdge *edge : edges)
    {
        // the endpoints identify an edge regardless of its weight.
        if (edge->from == fromNodeKey && edge->to == toNodeKey)
        {
            throw invalid_argument("Directed edge already exists");
        }
    }

    GraphEdge *edge = new GraphEdge{fromNodeKey, toNodeKey, weight};
    try
    {
        edges.push_back(edge);
    }
    catch (...)
    {
        // if the vector cannot grow, it has not taken ownership of the edge.
        delete edge;
        throw;
    }
    return edge;
}

bool Graph::IsPresent(nodekey_t key) const
{
    for (nodekey_t node : nodes)
    {
        if (node == key)
        {
            return true;
        }
    }
    return false;
}

string Graph::NodesToString() const
{
    ostringstream result;
    result << "[";
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        if (i != 0)
        {
            result << ", ";
        }
        result << "(" << nodes[i] << ")";
    }
    result << "]";
    return result.str();
}

string Graph::EdgesToString() const
{
    ostringstream result;
    result << "[";
    for (size_t i = 0; i < edges.size(); ++i)
    {
        if (i != 0)
        {
            result << ", ";
        }
        const GraphEdge *edge = edges[i];
        result << "((" << edge->from << ")->(" << edge->to
               << ") w:" << edge->weight << ")";
    }
    result << "]";
    return result.str();
}

set<const GraphEdge *> Graph::GetOutwardEdgesFrom(nodekey_t nodeKey) const
{
    set<const GraphEdge *> outwardEdges;
    for (const GraphEdge *edge : edges)
    {
        if (edge->from == nodeKey)
        {
            outwardEdges.insert(edge);
        }
    }
    // with no matching edges (including an unknown key), return an empty set.
    return outwardEdges;
}

set<nodekey_t> Graph::GetNodes() const
{
    // return an independent set: callers cannot change the graph through it.
    return set<nodekey_t>(nodes.begin(), nodes.end());
}

size_t Graph::Size() const
{
    return edges.size();
}

size_t Graph::Order() const
{
    return nodes.size();
}
