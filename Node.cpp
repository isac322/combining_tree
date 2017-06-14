#include "Node.h"

bool Node::precombine() __transaction_atomic {
	while (locked) __transaction_cancel;
	
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
	}
}

int Node::combine(int combined) __transaction_atomic {
	while (locked) __transaction_cancel;
	
	locked = true;
	first_value = combined;
	
	switch (status) {
		case CStatus::FIRST:
			return first_value;
		
		case CStatus::SECOND:
			return first_value + second_value;
	}
}

int Node::op(int combined) __transaction_atomic {
	switch (status) {
		case CStatus::ROOT: {
			tmp = result;
			result += combined;
			return tmp;
		}
		
		case CStatus::SECOND:
			__transaction_atomic{
				second_value = combined;
				
				locked = false;
			}
			
			while (status != CStatus::RESULT) __transaction_cancel;
			locked = false;
			
			status = CStatus::IDLE;
			return result;
	}
}

void Node::distribute(int prior) __transaction_atomic {
	switch (status) {
		case CStatus::FIRST:
			status = CStatus::IDLE;
			locked = false;
			break;

		case CStatus::SECOND:
			result = prior + first_value;
			status = CStatus::RESULT;
			break;
	}
}
