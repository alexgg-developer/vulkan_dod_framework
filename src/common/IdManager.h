#pragma once

#include "IdDeclarations.h"

class IdManager
{
public:
	enum manager_t {
		MATERIAL,
		SIZE
	};

	static IdManager& Get();

	void allocate(size_t size);
	externalId_t createId(manager_t manager, internalId_t internalId);

private:
	IdManager();
	~IdManager();
	IdManager(const IdManager &) = delete;
	IdManager& operator = (const IdManager &) = delete;

	size_t * idCount; //how many used ids are there
	size_t * maxIds;  //maxIds as in positions
	size_t * offset;
	externalId_t * toExternal; //actual internal id
	internalId_t * toInternal; //external id
	externalId_t* nextIdSpot;

	void searchEmptySpot(manager_t manager);
};
