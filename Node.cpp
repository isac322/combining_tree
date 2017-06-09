#include "Node.h"
#include "PanicException.h"
#include <iostream>

bool Node::precombine() {
	guard_lock lock(mutex);
	
	while (locked) conditional.wait(lock);
	
	switch (status) {
		case CStatus::IDLE:
			status = CStatus::FIRST;
			return true;
		
		case CStatus::FIRST:
			locked = true;
			status = CStatus::SECOND;
			return false;
		
		case CStatus::ROOT:
			return false;
		
		default:
			throw PanicException(
					"unexpected Node state " +
					std::to_string(static_cast<std::underlying_type<CStatus>::type>(status)) +
					" in precombine"
			);
	}
}

int Node::combine(int combined) {
	guard_lock lock(mutex);
	
	while (locked) conditional.wait(lock);
	
	locked = true;
	first_value = combined;
	
	switch (status) {
		case CStatus::FIRST:
			return first_value;
		
		case CStatus::SECOND:
			return first_value + second_value;
		
		default:
			throw PanicException(
					"unexpected Node state " +
					std::to_string(static_cast<std::underlying_type<CStatus>::type>(status)) +
					" in combine"
			);
	}
}

int Node::op(int combined) {
	guard_lock lock(mutex);
	
	switch (status) {
		case CStatus::ROOT: {
			int prior = result;
			result += combined;
			return prior;
		}
		
		case CStatus::SECOND:
			second_value = combined;
			
			locked = false;
			conditional.notify_all();
			
			while (status != CStatus::RESULT) conditional.wait(lock);
			locked = false;
			conditional.notify_all();
			
			status = CStatus::IDLE;
			return result;
		
		default:
			throw PanicException(
					"unexpected Node state " +
					std::to_string(static_cast<std::underlying_type<CStatus>::type>(status)) +
					" in op"
			);
	}
}

void Node::distribute(int prior) {
	guard_lock lock(mutex);
	
	switch (status) {
		case CStatus::FIRST:
			status = CStatus::IDLE;
			locked = false;
			break;
		
		
		case CStatus::SECOND:
			result = prior + first_value;
			status = CStatus::RESULT;
			break;
		
		default:
			throw PanicException(
					"unexpected Node state " +
					std::to_string(static_cast<std::underlying_type<CStatus>::type>(status)) +
					" in distribute"
			);
	}
	
	conditional.notify_all();
}
