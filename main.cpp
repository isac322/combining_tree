#include <iostream>
#include <thread>
#include "CombiningTree.h"
#include "PanicException.h"

using namespace std;

void run(CombiningTree *tree, const size_t id) {
	tree->thread_map[this_thread::get_id()] = id;
	
	try {
		cout << tree->getAndIncrement() << endl;
	} catch (PanicException e) {
		cerr << e.what() << endl;
	}
}

int main() {
	CombiningTree tree(256);
	array<thread *, 128> threads;
	
	for (size_t i = 0; i < threads.size(); i++) {
		threads[i] = new thread(run, &tree, i);
	}
	
	for (auto &thread : threads) thread->join();
}