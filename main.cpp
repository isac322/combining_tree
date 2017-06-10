#include <iostream>
#include <thread>
#include <vector>
#include "CombiningTree.h"
#include "PanicException.h"
#include "Timer.h"

#define SIZE 32768

using namespace std;

mutex mutex_map;

void run(CombiningTree *tree, const size_t id) {
	mutex_map.lock();
	tree->thread_map[this_thread::get_id()] = id;
	mutex_map.unlock();
	
	try {
		tree->getAndIncrement();
	} catch (PanicException e) {
		cerr << e.what() << endl;
	}
	
	mutex_map.lock();
	tree->thread_map.erase(this_thread::get_id());
	mutex_map.unlock();
}

int main() {
	CombiningTree tree(SIZE);
	vector<thread *> threads;
	
	Timer timer;
	
	for (size_t i = 0; i < SIZE >> 1; i++) {
		threads.emplace_back(new thread(run, &tree, i));
	}
	
	for (auto &thread : threads) thread->join();
	
	cout << timer.elapsed() << "seconds" << endl;
}