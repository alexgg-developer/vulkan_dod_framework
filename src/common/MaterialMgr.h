#pragma once

#ifndef TINY_OBJ_LOADER_H_
#include <tiny_obj_loader.h>
#endif
#include <glm/common.hpp>

#include "IdDeclarations.h"

class MaterialMgr
{
public:
	static const size_t MATERIAL_NAME_SIZE = 128;
	static const size_t TEXTURE_NAME_SIZE = 256;
	static const size_t PATH_SIZE = 512;

	static MaterialMgr& Get();

	void allocate(size_t size);
	materialExternalId_t addMaterial(tinyobj::material_t newMaterial);
	~MaterialMgr();
private:
	char(*m_name)[MATERIAL_NAME_SIZE];
	char(*m_diffuseTexture)[TEXTURE_NAME_SIZE];
	char(*m_normalTexture)[TEXTURE_NAME_SIZE];
	char(*m_emissiveTexture)[TEXTURE_NAME_SIZE];
	char(*m_specularTexture)[TEXTURE_NAME_SIZE];
	char(*m_path)[PATH_SIZE];
	glm::vec3 *m_ambient;
	glm::vec3 *m_diffuse;
	glm::vec3 *m_specular;
	glm::vec3 *m_emission;
	float * m_shininess;

	size_t sizeData = 0;

	MaterialMgr();
	MaterialMgr(const MaterialMgr &) = delete;
	MaterialMgr& operator = (const MaterialMgr &) = delete;
};
