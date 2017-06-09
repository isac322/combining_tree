#include <iostream>
#include <thread>
#include "CombiningTree.h"
#include "PanicException.h"
#include "Timer.h"

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
	CombiningTree tree(256);
	array<thread *, 128> threads;
	
	Timer timer;
	
	for (size_t i = 0; i < threads.size(); i++) {
		threads[i] = new thread(run, &tree, i);
	}
	
	for (auto &thread : threads) thread->join();
	
	cout << timer.elapsed() << "seconds" << endl;
}