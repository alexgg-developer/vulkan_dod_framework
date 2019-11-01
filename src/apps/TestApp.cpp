#include "TestApp.h"

#include <vector>
#include <assert.h>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

using namespace dodf;

void TestApp::run()
{
	//testMemory();
	size_t initSize = 33554432;
	MemoryPool::Initialize(initSize);
	movementComponentManager.allocate(100);
	testMovementComponent();
	mainLoop();
}

void TestApp::update(float dt)
{
	updateTestMovementComponent(dt);
}

void TestApp::draw()
{
}

void TestApp::updateTestMovementComponent(float dt)
{
	/*for (const auto & entity : entities) {
		movementComponentManager.addPosition(movementComponentManager.lookup(entity), glm::vec3(1.0f, 0.0f, 0.0f));
		auto position = movementComponentManager.getPosition(movementComponentManager.lookup(entity));
		std::cout << "new position:" << position.x << std::endl;
	}*/
	movementSystem.simulate(movementComponentManager.getVelocities(), movementComponentManager.getPositions(), movementComponentManager.getSize(), dt);
	for (const auto & entity : entities) {
		auto position = movementComponentManager.getPosition(movementComponentManager.lookup(entity));
		std::cout << "new position:" << glm::to_string(position) << std::endl;
	}
}

void TestApp::testMovementComponent()
{
	entities.emplace_back(entityManager.create());
	movementComponentManager.add(entities[entities.size()-1]);
	movementComponentManager.setPosition(movementComponentManager.lookup(entities[entities.size() - 1]), glm::vec3(1.0f, 1.0f, 1.0f));
	movementComponentManager.setVelocity(movementComponentManager.lookup(entities[entities.size() - 1]), glm::vec3(0.1f, 0.25f, -0.1f));
}

void TestApp::testMemory()
{
	vector<int*> serie(1000);
	auto totalSerieSize = 0;
	for (auto i = 1; i <= serie.size(); ++i) {
		for (auto j = 1; j <= i; ++j) {
			++totalSerieSize;
		}
	}
	++totalSerieSize;
	MemoryPool::Initialize(totalSerieSize * sizeof(int) * 10);

	for (auto i = 1; i <= serie.size(); ++i) {
		//serie.push_back(reinterpret_cast<int*>(MemoryPool::Get(i * sizeof(int))));
		auto currentSize = MemoryPool::Debug::GetCurrentSize();
		auto currentAddress = MemoryPool::Debug::GetCurrentAddress();
		auto sizeAsked = i * sizeof(int);
		auto newArray = reinterpret_cast<int*>(MemoryPool::Get(sizeAsked));
		auto afterGetSize = MemoryPool::Debug::GetCurrentSize();
		auto afterGetAddress = MemoryPool::Debug::GetCurrentAddress();
		/*cout << "currentSize:: " << currentSize << endl;
		cout << "sizeAsked:: " << sizeAsked << endl;
		cout << "currentAddress:: " << currentAddress << endl;
		//cout << "afterGetSize:: " << afterGetSize << endl;
		//cout << "afterGetAddress:: " << afterGetAddress << endl;
		cout << "diffSize:: " << afterGetSize - currentSize << endl;
		cout << "diffAddress:: " << afterGetAddress - currentAddress << endl;*/
		cout << endl;
		serie[i-1] = newArray;
		auto last = serie[i-1];
		for (auto j = 1; j <= i; ++j) {
			*last = j;
			--totalSerieSize;
			//cout << "currentAddress:: " << reinterpret_cast<uintptr_t>(last) << endl;
			++last;
		}
		cout << endl;
	}
	cout << "run" << endl;

	for (auto i = 1; i <= serie.size(); ++i) {
		auto current = serie[i - 1];
		cout << "current array " << i << endl;
		for (auto j = 1; j <= i; ++j) {
			assert(*current == j);
			cout << "::" << *current << "::" ;
			++current;
		}
		cout << endl;
	}

	MemoryPool::Destroy();
}
