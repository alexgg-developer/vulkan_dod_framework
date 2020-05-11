#include "Entity.h"


bool operator== (const Entity &e1, const Entity &e2)
{
	return (e1.id == e2.id);
}

bool operator<(const Entity &e1, const Entity &e2)
{
	return (e1.id < e2.id);
}

