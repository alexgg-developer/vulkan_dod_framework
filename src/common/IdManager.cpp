#include "IdManager.h"
#include "MemoryPool.h"
#include "Assert.h"
#include "IdDeclarations.h"

#include <cstring>
#include <limits>

using namespace dodf;

IdManager::IdManager()
{
}

IdManager::~IdManager()
{
	//cleanup();
}

IdManager & IdManager::Get()
{
	static IdManager IdManager;

	return IdManager;
}

void IdManager::allocate(size_t size)
{
	size_t totalSize = 0;

	idCount = (size_t *)MemoryPool::Get(sizeof(size_t) * manager_t::SIZE);
	maxIds = (size_t *)MemoryPool::Get(sizeof(size_t) * manager_t::SIZE);
	nextIdSpot = (externalId_t *)MemoryPool::Get(sizeof(externalId_t) * manager_t::SIZE);
	offset = (size_t *)MemoryPool::Get(sizeof(size_t) * manager_t::SIZE);

	for (unsigned int i = 0; i < manager_t::SIZE; ++i) {
		idCount[i] = 0;
		nextIdSpot[i].id = totalSize;
		//maxIds[i] = numberOfIdsNeeded[i];
		maxIds[i] = size;
		offset[i] = totalSize;
		//totalSize += numberOfIdsNeeded[i];
		totalSize += size;
	}

	toExternal = (externalId_t *)MemoryPool::Get(totalSize * sizeof(externalId_t));
	toInternal = (internalId_t *)MemoryPool::Get(totalSize * sizeof(internalId_t));

	memset(toExternal, '\xFF', totalSize * sizeof(size_t));
	memset(toInternal, '\xFF', totalSize * sizeof(size_t));
}

externalId_t IdManager::createId(manager_t manager, internalId_t internalId) 
{
	ASSERT(manager >= 0 && manager < manager_t::SIZE);
	//ASSERT(toExternal[offset[manager] + internalId.id].id == UINTPTR_MAX); //don't ask for an already existing externalId
	ASSERT(toExternal[offset[manager] + internalId.id].id == std::numeric_limits<size_t>::max()); //don't ask for an already existing externalId
	size_t result = nextIdSpot[manager].id - offset[manager];

	toExternal[offset[manager] + internalId.id] = nextIdSpot[manager];
	//toInternal[nextIdSpot[manager].id].id = internalId.id;
	toInternal[nextIdSpot[manager].id] = internalId;

	//nextIdSpot[manager].id++;
	idCount[manager]++;

	ASSERT(idCount[manager] <= maxIds[manager]);

	searchEmptySpot(manager);

	return externalId_t{ result };
}

void IdManager::searchEmptySpot(manager_t manager) 
{
	while (toInternal[nextIdSpot[manager].id].id != std::numeric_limits<size_t>::max() && idCount[manager] < maxIds[manager]) {
		nextIdSpot[manager].id++;
		if (nextIdSpot[manager].id > offset[manager] + maxIds[manager] - 1) {
			nextIdSpot[manager].id = offset[manager];
		}
	}
}