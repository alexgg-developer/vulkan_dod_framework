#include <tiny_obj_loader.h>
#include <glm/common.hpp>

#include "ModelLoader.h"
#include "Assert.h"
#include "MaterialMgr.h"


#include <unordered_map>
#include <chrono>
#include <iostream>


const glm::vec3 ModelLoader::DEFAULT_COLOR(1.0f, 0.0f, 0.0f);


ModelLoader::model_t ModelLoader::loadModel(const std::string& modelName, const glm::vec3& defaultColor)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	std::string modelPath = "models/" + modelName;
	std::string materialPath = "materials/";
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelPath.c_str(), materialPath.c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
	std::vector<Vertex> vertices;
	//vertices.reserve(100000);
	std::vector<uint32_t> indices;
	//indices.reserve(2000000);

	//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.color = defaultColor;

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
	//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;


	return std::make_pair(vertices, indices);
}

ModelLoader::texturedModel_t ModelLoader::loadModel(const std::string& modelName)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	std::string modelPath = "models/" + modelName;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelPath.c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<TexturedVertex, uint32_t> uniqueVertices = {};
	std::vector<TexturedVertex> vertices;
	std::vector<uint32_t> indices;

	//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			TexturedVertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
	//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;


	return std::make_pair(vertices, indices);
}

void ModelLoader::loadMeshes(const std::string & modelName, std::function<void(const Mesh<TexturedVertex>&)> loadMeshCallback)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	std::string modelPath = "models/" + modelName;
	std::string materialPath = "materials/";
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelPath.c_str(), materialPath.c_str())) {
		throw std::runtime_error(warn + err);
	}
	
	std::vector<materialExternalId_t> materialIds;
	for (const auto& material : materials) {
		materialIds.push_back(MaterialMgr::Get().addMaterial(material));
	}
	//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (const auto& shape : shapes) {
		std::unordered_map<TexturedVertex, uint32_t> uniqueVertices = {};
		Mesh<TexturedVertex> mesh;

		mesh.name = shape.name;
		mesh.material = materialExternalId_t{ UINTPTR_MAX };
		//auto isPod = std::is_pod<materialExternalId_t>().value;
		if (!shape.mesh.material_ids.empty()) {
			mesh.material = materialIds[shape.mesh.material_ids[0]];
		}
		for (const auto& index : shape.mesh.indices) {
			TexturedVertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(mesh.vertices.size());
				mesh.vertices.push_back(vertex);
			}

			mesh.indexes.push_back(uniqueVertices[vertex]);

		}
		loadMeshCallback(mesh);
	}
}


