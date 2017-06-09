#pragma once

#include "Node.h"
#include <unordered_map>
#include <thread>

class CombiningTree {
protected:
	Node **leaves;
	int width;

public:
	std::unordered_map<std::thread::id, size_t> thread_map;
	
	CombiningTree(int width);
	
	~CombiningTree() {
		for (int i = 0; i < (width + 1) / 2; i++) delete leaves[i];
		delete leaves;
	}
	
	int getAndIncrement();
};
