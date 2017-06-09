//
// Created by bhyoo on 17. 6. 9.
//

#include "CombiningTree.h"
#include <stack>
#include <iostream>

CombiningTree::CombiningTree(int width) : leaves(new Node *[(width + 1) / 2]) {
	Node *nodes[width - 1];
	nodes[0] = new Node();
	
	for (size_t i = 1; i < width - 1; ++i)
		nodes[i] = new Node(nodes[(i - 1) / 2]);
	
	for (size_t i = 0; i < (width + 1) / 2; ++i)
		leaves[i] = nodes[width - i - 2];
}

int CombiningTree::getAndIncrement() {
	std::stack<Node *> stack;
	
	Node *my_leaf = leaves[thread_map[std::this_thread::get_id()]];
	
	Node *stop = my_leaf;
	// precombining phase
	while (stop->precombine()) {
		stop = stop->parent;
	}
	
	// combining phase
	int combined = 1;
	for (Node *node = my_leaf; node != stop; node = node->parent) {
		combined = node->combine(combined);
		stack.emplace(node);
	}
	
	// operation phase
	int prior = stop->op(combined);
	
	// distribution phase
	while (!stack.empty()) {
		stack.top()->distribute(prior);
		stack.pop();
	}
	
	return prior;
}
