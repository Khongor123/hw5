#include "Graph.h"
#include "BetterPriorityQueue.h"
#include <cassert>
#include <iostream>

using namespace std;

void EdgeCasesTest() {
    cout << "Testing empty queues, formatting, and rejected updates..." << endl;
    BetterPriorityQueue q;
    BPQNode node;
    node.gnode = 7;
    node.pri = 10;
    node.visited = true;
    assert(q.ToString() == "[]");
    assert(!q.Contains(node));
    assert(!q.Update(node));

    q.push(node);
    assert(q.ToString() == "[(7, pri: 10)]");
    assert(BetterPriorityQueue::BPQNodeToString(node) == "(7, pri: 10)");
    BPQNode dummy;
    dummy.gnode = 7;
    dummy.pri = 10;
    assert(q.Contains(dummy)); // Different visited values do not affect identity.
    assert(!q.Update(dummy)); // Equal priority makes no change.
    dummy.pri = 11;
    assert(!q.Update(dummy));
    assert(q.top().pri == 10);
    dummy.gnode = 99;
    dummy.pri = -1;
    assert(!q.Update(dummy));
    assert(q.ToString() == "[(7, pri: 10)]");

    dummy.gnode = 7;
    dummy.pri = INT_MIN;
    assert(q.Update(dummy)); // Also tests updating the root of a one-item heap.
    assert(q.top().pri == INT_MIN);
    assert(q.top().visited); // Update must not overwrite stored metadata.
    assert(q.size() == 1);
    q.pop();
    assert(!q.Contains(dummy));
    assert(!q.Update(dummy));
    assert(q.ToString() == "[]");
    cout << "PASSED!" << endl;
}

void HeapOrderTest() {
    cout << "Testing heap ordering after an update..." << endl;
    BetterPriorityQueue q;
    for (int key = 1; key <= 6; ++key) {
        BPQNode node;
        node.gnode = key;
        node.pri = key;
        q.push(node);
    }

    BPQNode updated;
    updated.gnode = 6;
    updated.pri = 0;
    assert(q.Update(updated));
    assert(q.top().gnode == 6);
    q.pop();

    // All remaining priorities must come out in increasing order.
    for (int priority = 1; priority <= 5; ++priority) {
        assert(q.top().pri == priority);
        q.pop();
    }
    assert(q.empty());
    cout << "PASSED!" << endl;
}

void ContainsTest() {
	cout << "Testing Contains Helper Method..." << endl;

	Graph g = Graph();
	BetterPriorityQueue q;

	g.AddNode(1);
	g.AddNode(2);
	g.AddNode(3);
	g.AddNode(4);
	g.AddNode(5);
	g.AddNode(6);
	set<nodekey_t> nodes = g.GetNodes();
	for(nodekey_t n : nodes){
		BPQNode cur;
		cur.gnode = n;
		q.push(cur);
	}

	// 1st test
	BPQNode positive;
	positive.gnode = 2;
	assert(q.Contains(positive) == true);

	// 2nd test
	BPQNode negative;
	assert(q.Contains(negative) == false);

	// 3rd test (just to ensure it still works after 2nd test finishes)
	assert(q.Contains(positive) == true);

	cout << "PASSED!" << endl;
}

void UpdateTest() {
	cout << "Testing Update Helper Method..." << endl;

	Graph g = Graph();
	BetterPriorityQueue q;

	g.AddNode(1);
	g.AddNode(2);
	g.AddNode(3);
	g.AddNode(4);
	g.AddNode(5);
	g.AddNode(6);
	set<nodekey_t> nodes = g.GetNodes();
	size_t i = 0;
	for(nodekey_t n : nodes){
		BPQNode cur;
		cur.pri = i;
		cur.gnode = n;
		q.push(cur);
		i++;
	}

	// Test 1: update a node with pri 0 (normal case)
	BPQNode positive;
	positive.gnode = 6;
	positive.pri = 0;
	cout << "q before: " << q.ToString() << endl;
	assert(q.Update(positive) == true);
	cout << "q after: " << q.ToString() << endl;

	// some students might have a slightly different order
	// please ensure the output is a valid priorityqueue as
	// implemented by a binary heap.  Any valid order is fine!
	string soln1 = "[(6, pri: 0), (1, pri: 0), (2, pri: 1), (3, pri: 2), (4, pri: 3), (5, pri: 4)]";
	string soln2 = "[(1, pri: 0), (2, pri: 1), (6, pri: 0), (4, pri: 3), (5, pri: 4), (3, pri: 2)]";
	string soln3 = "[(1, pri: 0), (6, pri: 0), (2, pri: 1), (3, pri: 2), (4, pri: 3), (5, pri: 4)]";

	// Test 2: attempt to update with an empty / undefined input
	BPQNode negative;
	assert(q.Update(negative) == false);
	cout << "q after test 2: " << q.ToString() << endl;
	assert(q.ToString() == soln1 || q.ToString() == soln2 || q.ToString() == soln3);

	// Test 3: update a node with a larger priority
	// It must behave this way (ignoring larger priorities)
	// for compatibility with Dijkstra's algorithm
	BPQNode negative2;
	negative2.gnode = 5;
	negative2.pri = 80;
	assert(q.Update(negative2) == false);
	cout << "q after test 3: " << q.ToString() << endl;
	assert(q.ToString() == soln1 || q.ToString() == soln2 || q.ToString() == soln3);

	cout << "PASSED!" << endl;
}

int main(){

	ContainsTest();
	UpdateTest();
	EdgeCasesTest();
	HeapOrderTest();

	cout << "ALL TESTS PASSED!!" << endl;

	return 0;

}
