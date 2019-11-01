#pragma once

#include <stdint.h>

//this a simple, lightweight wrap to a unsigned int that we use as an id. Every id is managed through the managed and is local to the instance data of those managers.
struct Instance
{
private:
	Instance(size_t i):i(i) { }
public:
	Instance():i(NO_VALUE) { }
	size_t i;
	const static size_t NO_VALUE = -1;
	static Instance create(size_t i) { Instance inst = { i }; return inst; }
	inline bool isValid() const { return i != NO_VALUE; }
};
