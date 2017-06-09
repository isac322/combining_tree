#pragma once

#include "CStatus.h"
#include <mutex>
#include <condition_variable>

class Node {
	typedef std::unique_lock<std::recursive_mutex> guard_lock;
	
private:
	std::recursive_mutex mutex_precombine, mutex_combine, mutex_op, mutex_distribute;
	std::condition_variable_any conditional;
	bool locked;
	CStatus status;
	int first_value, second_value;
	int result;

public:
	Node *parent;
	
	Node() : parent(nullptr), status(CStatus::ROOT), locked(false) {}
	
	Node(Node *parent) : parent(parent), status(CStatus::IDLE), locked(false) {}
	
	~Node() {
		if (parent == nullptr) return;
		delete parent;
	}
	
	bool precombine();
	
	int combine(int combined);
	
	int op(int combined);
	
	void distribute(int prior);
};
