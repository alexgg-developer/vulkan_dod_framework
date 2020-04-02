#pragma once

struct internalId_t {
	size_t id;

	bool operator==(const internalId_t &other) const {
		return id == other.id;
	}
};

struct externalId_t {
	size_t id;

	bool operator==(const externalId_t &other) const {
		return id == other.id;
	}
};


struct materialExternalId_t {
	size_t id;

	bool operator==(const materialExternalId_t &other) const {
		return id == other.id;
	}
};

struct materialInternalId_t {
	size_t id;

	bool operator==(const materialInternalId_t &other) const {
		return id == other.id;
	}
};