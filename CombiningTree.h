#pragma once

#include "Node.h"
#include <unordered_map>
#include <thread>

class CombiningTree {
protected:
	Node **leaves;

public:
	std::unordered_map<std::thread::id, size_t> thread_map;
	
	CombiningTree(int width);
	
	~CombiningTree() {
		delete[] leaves;
	}
	
	int getAndIncrement();
};
