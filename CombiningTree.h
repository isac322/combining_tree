#pragma once

#include "Node.h"
#include <unordered_map>
#include <thread>

using namespace std;

class CombiningTree {
protected:
	Node **leaves;

public:
	unordered_map<thread::id, size_t> thread_map;
	
	CombiningTree(int width);
	
	~CombiningTree() {
		delete[] leaves;
	}
	
	int getAndIncrement();
};
