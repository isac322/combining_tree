#pragma once

#include "CStatus.h"
#include <mutex>
#include <condition_variable>

class Node {
	typedef std::mutex mutex_t;
	typedef std::unique_lock<mutex_t> guard_lock;

private:
	mutex_t mutex;
	std::condition_variable conditional;
	bool locked = false;
	CStatus status;
	int first_value, second_value;
	int result;

public:
	Node *parent;
	
	Node() : parent(nullptr), status(CStatus::ROOT) {}
	
	Node(Node *parent) : parent(parent), status(CStatus::IDLE) {}
	
	~Node() {
		if (parent == nullptr) return;
		delete parent;
	}
	
	bool precombine();
	
	int combine(int combined);
	
	int op(int combined);
	
	void distribute(int prior);
};
