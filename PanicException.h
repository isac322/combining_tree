#pragma once

#include <exception>
#include <string>

class PanicException : public std::exception {
private:
	std::string message;
public:
	PanicException(std::string msg) : message(msg) {}
	
	const char *what() const throw() override {
		return message.c_str();
	}
};
