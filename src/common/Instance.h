#pragma once

#include <stdint.h>

//this a simple, lightweight wrap to a unsigned int that we use as an id. Every id is managed through the managed and is local to the instance data of those managers.
struct Instance
{
private:
	Instance(size_t i):i(i) { }
public:
	const static size_t NO_VALUE = -1;
	size_t i;
	
	Instance():i(NO_VALUE) { }
		
	static Instance create(size_t i) { Instance inst = { i }; return inst; }

	inline bool isValid() const { return i != NO_VALUE; }
};
