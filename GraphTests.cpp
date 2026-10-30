#include "Graph.h"
#include <iostream>
#include <cassert>

using namespace std;

void TestAddNode(){
	cout << "Testing AddNode..." << endl;
	Graph g = Graph();

	g.AddNode(15);
	assert(g.NodesToString() == "[(15)]");

	g.AddNode(12);
	assert(g.NodesToString() == "[(15), (12)]");

	try
	{
		g.AddNode(12);
		assert(false);
	}
	catch( const invalid_argument& e ) { }

	cout << "PASSED!" << endl << endl;
}

void TestAddEdge(){
	cout << "Testing AddEdge..." << endl;
	Graph g = Graph();

	g.AddNode(15);
	g.AddNode(12);
	g.AddNode(9);
	assert(g.NodesToString() == "[(15), (12), (9)]");

	const GraphEdge *ge1 = g.AddEdge(15, 9, 2);
	assert(g.NodesToString() == "[(15), (12), (9)]");
	assert(g.EdgesToString() == "[((15)->(9) w:2)]");
	assert(ge1->from == 15);
	assert(ge1->to == 9);
	assert(ge1->weight == 2);

	const GraphEdge *ge2 = g.AddEdge(9, 12); // this makes the weight 0, since default value for weight is 0
	assert(g.NodesToString() == "[(15), (12), (9)]");
	assert(g.EdgesToString() == "[((15)->(9) w:2), ((9)->(12) w:0)]");
	assert(ge2->from == 9);
	assert(ge2->to == 12);
	assert(ge2->weight == 0);

	try
	{
		const GraphEdge *ge3 = g.AddEdge(20, 12);
		cout << "ge3: (" + to_string(ge3->from) + "->" + to_string(ge3->to) + "  w: " + to_string(ge3->weight) + ")" << endl;
		assert(false);
	} catch (const invalid_argument& e) {}

	try
	{
		const GraphEdge *ge3 = g.AddEdge(15, 9, 7);
		// edge already exists!
		cout << "ge3: (" + to_string(ge3->from) + "->" + to_string(ge3->to) + "  w: " + to_string(ge3->weight) + ")" << endl;
		assert(false);
	} catch (const invalid_argument& e) {}

	// A missing destination must be rejected, just like a missing source.
	try {
		g.AddEdge(15, 20);
		assert(false);
	} catch (const invalid_argument& e) {}
	assert(g.Size() == 2);

	// Opposite directions are distinct edges; loops are also valid.
	g.AddEdge(9, 15, 4);
	g.AddEdge(12, 12, 1);
	assert(g.Size() == 4);
	assert(g.Order() == 3);

	cout << "PASSED!" << endl << endl;
}

void TestIsPresentSizeAndOrder() {
	cout << "Testing IsPresent, Size, and Order..." << endl;

	Graph g2 = Graph();
	assert(g2.Size() == 0);
	assert(g2.Order() == 0);
	assert(g2.IsPresent(17) == false);
	g2.AddNode(); // Default node key is zero.
	g2.AddNode(17);
	assert(g2.IsPresent(0));
	assert(g2.IsPresent(17));
	assert(!g2.IsPresent(18));
	assert(g2.Order() == 2);
	g2.AddEdge(0, 17);
	assert(g2.Size() == 1);
	assert(g2.Order() == 2);

	cout << "PASSED!" << endl << endl;
}

void TestGetOutwardEdgesFrom(){
	cout << "Testing GetOutwardEdgesFrom..." << endl;
	Graph g = Graph();
	g.AddNode(17);
	g.AddNode(18);
	g.AddNode(22);
	g.AddNode(34);

	const GraphEdge* ge1 = g.AddEdge(17, 18, 3);
	g.AddEdge(22, 18); // weight will be 0 of course
	const GraphEdge* ge3 = g.AddEdge(17, 34, 8);
	g.AddEdge(22, 17, 6); // note, 22 is not a neighbor of 17, but 17 is a neighbor of 22

	set<const GraphEdge*> soln;
	soln.insert(ge3);
	soln.insert(ge1);

	set<const GraphEdge*> result = g.GetOutwardEdgesFrom(17);
	assert(result.size() == soln.size());
	assert(result == soln);
	assert(g.GetOutwardEdgesFrom(18).empty()); // Incoming edges do not count.
	assert(g.GetOutwardEdgesFrom(999).empty());
	const GraphEdge* loop = g.AddEdge(34, 34, 2);
	assert(g.GetOutwardEdgesFrom(34) == set<const GraphEdge*>{loop});

	cout << "PASSED!" << endl << endl;
}

void TestGetNodes(){
	cout << "Testing GetNodes..." << endl;

	Graph g = Graph();
	g.AddNode(51);
	g.AddNode(24);
	g.AddNode(73);

	set<nodekey_t> nodes = g.GetNodes();
	set<nodekey_t> soln = {24, 73, 51};
	assert(nodes.size() == soln.size());
	assert(nodes == soln);
	nodes.clear();
	assert(g.GetNodes() == soln); // changing the returned set leaves g intact.

	g = Graph();
	nodes = g.GetNodes();
	soln.clear();
	assert(nodes.size() == soln.size());
	assert(nodes == soln);

	cout << "PASSED!" << endl << endl;
}

void TestEmptyGraphAndCopies() {
	cout << "Testing empty graphs, copying, and assignment..." << endl;
	Graph empty;
	assert(empty.NodesToString() == "[]");
	assert(empty.EdgesToString() == "[]");
	assert(empty.GetNodes().empty());
	assert(empty.GetOutwardEdgesFrom(1).empty());
	try {
		empty.AddEdge(1, 2);
		assert(false);
	} catch (const invalid_argument& e) {}
	assert(empty.Size() == 0);

	Graph copy;
	{
		Graph original;
		original.AddNode(1);
		original.AddNode(2);
		const GraphEdge* edge = original.AddEdge(1, 2, 15);
		Graph constructed(original);
		assert(constructed.NodesToString() == original.NodesToString());
		assert(constructed.EdgesToString() == original.EdgesToString());
		assert(*constructed.GetOutwardEdgesFrom(1).begin() != edge);

		copy = original;
		assert(*copy.GetOutwardEdgesFrom(1).begin() != edge);
		const Graph& sameGraph = copy;
		copy = sameGraph; // Self-assignment must preserve contents.
		assert(copy.EdgesToString() == original.EdgesToString());

	}
	// the original is destroyed, but the copied edge is still alive.
	assert((*copy.GetOutwardEdgesFrom(1).begin())->weight == 15);
	copy = Graph(); // replacing a nonempty graph has to release its old edges.
	assert(copy.Order() == 0 && copy.Size() == 0);
	cout << "PASSED!" << endl << endl;
}

void TestDestructor(){
	cout << "Testing Destructor..." << endl;
	Graph *g = new Graph();

	g->AddNode(3);
	g->AddNode(5);
	g->AddEdge(3, 5, 15);
	delete g;

	Graph *g2 = new Graph();
	g2->AddNode(12);
	g2->AddNode(11);
	delete g2;

	Graph *g3 = new Graph();
	delete g3;

	Graph g4 = Graph(); // deconstructed automatically when this funciton ends

	cout << "NO ERRORS.  Use valgrind to check!" << endl << endl;
}

int main(){

	TestAddNode();
	TestAddEdge();
	TestIsPresentSizeAndOrder();
	TestGetOutwardEdgesFrom();
	TestGetNodes();
	TestDestructor();
	TestEmptyGraphAndCopies();

	cout << "ALL TESTS PASSED!" << endl;
	return 0;
}
