#pragma once

#include <stdint.h>

const unsigned ENTITY_INDEX_BITS = 22;
const unsigned ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;

const unsigned ENTITY_GENERATION_BITS = 8;
const unsigned ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;

//WIP: https://bitsquid.blogspot.com/2014/09/building-data-oriented-entity-system.html
class Entity
{
private:
	uint32_t id;
public: 
	Entity(uint32_t idx, uint8_t generationID): id(idx | generationID << ENTITY_INDEX_BITS) {}
	uint32_t index() const { return id & ENTITY_INDEX_MASK; }
	uint32_t generation() const { return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }
};
