#include "MaterialMgr.h"
#include "MemoryPool.h"
#include "IdManager.h"

#include <glm/gtc/type_ptr.hpp>

using namespace dodf;

MaterialMgr::MaterialMgr()
{
}

MaterialMgr::~MaterialMgr()
{
	//cleanup();
}

MaterialMgr & MaterialMgr::Get()
{
	static MaterialMgr materialManager;

	return materialManager;
}

/*typedef struct {
  std::string name;

  real_t ambient[3];
  real_t diffuse[3];
  real_t specular[3];
  real_t transmittance[3];
  real_t emission[3];
  real_t shininess;
  real_t ior;       // index of refraction
  real_t dissolve;  // 1 == opaque; 0 == fully transparent
  // illumination model (see http://www.fileformat.info/format/material/)
  int illum;

  int dummy;  // Suppress padding warning.

  std::string ambient_texname;             // map_Ka
  std::string diffuse_texname;             // map_Kd
  std::string specular_texname;            // map_Ks
  std::string specular_highlight_texname;  // map_Ns
  std::string bump_texname;                // map_bump, map_Bump, bump
  std::string displacement_texname;        // disp
  std::string alpha_texname;               // map_d
  std::string reflection_texname;          // refl

  texture_option_t ambient_texopt;
  texture_option_t diffuse_texopt;
  texture_option_t specular_texopt;
  texture_option_t specular_highlight_texopt;
  texture_option_t bump_texopt;
  texture_option_t displacement_texopt;
  texture_option_t alpha_texopt;
  texture_option_t reflection_texopt;

  // PBR extension
  // http://exocortex.com/blog/extending_wavefront_mtl_to_support_pbr
  real_t roughness;            // [0, 1] default 0
  real_t metallic;             // [0, 1] default 0
  real_t sheen;                // [0, 1] default 0
  real_t clearcoat_thickness;  // [0, 1] default 0
  real_t clearcoat_roughness;  // [0, 1] default 0
  real_t anisotropy;           // aniso. [0, 1] default 0
  real_t anisotropy_rotation;  // anisor. [0, 1] default 0
  real_t pad0;
  std::string roughness_texname;  // map_Pr
  std::string metallic_texname;   // map_Pm
  std::string sheen_texname;      // map_Ps
  std::string emissive_texname;   // map_Ke
  std::string normal_texname;     // norm. For normal mapping.

  texture_option_t roughness_texopt;
  texture_option_t metallic_texopt;
  texture_option_t sheen_texopt;
  texture_option_t emissive_texopt;
  texture_option_t normal_texopt;

  int pad2;

  std::map<std::string, std::string> unknown_parameter;
} material_t;*/

materialExternalId_t MaterialMgr::addMaterial(tinyobj::material_t newMaterial)
{
	strcpy_s(m_name[sizeData], newMaterial.name.length()+1, newMaterial.name.data());
	strcpy_s(m_diffuseTexture[sizeData], newMaterial.diffuse_texname.length() + 1, newMaterial.diffuse_texname.data());
	strcpy_s(m_normalTexture[sizeData], newMaterial.normal_texname.length() + 1, newMaterial.normal_texname.data());
	strcpy_s(m_emissiveTexture[sizeData], newMaterial.emissive_texname.length() + 1, newMaterial.emissive_texname.data());
	strcpy_s(m_specularTexture[sizeData], newMaterial.specular_texname.length() + 1, newMaterial.specular_texname.data());
	
	m_ambient[sizeData] = glm::make_vec3(newMaterial.ambient);
	m_diffuse[sizeData] = glm::make_vec3(newMaterial.diffuse);
	m_specular[sizeData] = glm::make_vec3(newMaterial.specular);
	m_emission[sizeData] = glm::make_vec3(newMaterial.emission);
	m_shininess[sizeData] = newMaterial.shininess;


	auto materialId = IdManager::Get().createId(IdManager::manager_t::MATERIAL, internalId_t{ sizeData++ } );
	return materialExternalId_t{ materialId.id };
}


void MaterialMgr::allocate(size_t size)
{
	//size_t dataBufferByteSize = size * (2 * sizeof(uint32_t) + 2 * sizeof(unsigned long long) + 2 * sizeof(uint64_t));
	//m_buffer.data = MemoryPool::Get(dataBufferByteSize);
	m_name = reinterpret_cast<char(*)[MATERIAL_NAME_SIZE]>(MemoryPool::Get(size * MATERIAL_NAME_SIZE * sizeof(char)));
	memset(m_name, 0, size * MATERIAL_NAME_SIZE * sizeof(char));
	m_diffuseTexture = reinterpret_cast<char(*)[TEXTURE_NAME_SIZE]>(MemoryPool::Get(size * TEXTURE_NAME_SIZE * sizeof(char)));
	memset(m_diffuseTexture, 0, size * MATERIAL_NAME_SIZE * sizeof(char));
	m_normalTexture = reinterpret_cast<char(*)[TEXTURE_NAME_SIZE]>(MemoryPool::Get(size * TEXTURE_NAME_SIZE * sizeof(char)));
	memset(m_normalTexture, 0, size * MATERIAL_NAME_SIZE * sizeof(char));
	m_emissiveTexture = reinterpret_cast<char(*)[TEXTURE_NAME_SIZE]>(MemoryPool::Get(size * TEXTURE_NAME_SIZE * sizeof(char)));
	memset(m_emissiveTexture, 0, size * MATERIAL_NAME_SIZE * sizeof(char));
	m_specularTexture = reinterpret_cast<char(*)[TEXTURE_NAME_SIZE]>(MemoryPool::Get(size * TEXTURE_NAME_SIZE * sizeof(char)));
	memset(m_specularTexture, 0, size * MATERIAL_NAME_SIZE * sizeof(char));
	m_path = reinterpret_cast<char(*)[PATH_SIZE]>(MemoryPool::Get(size * PATH_SIZE * sizeof(char)));
	memset(m_path, 0, size * PATH_SIZE * sizeof(char));
	m_ambient = static_cast<glm::vec3*>(MemoryPool::Get(size * sizeof(glm::vec3)));
	m_diffuse = static_cast<glm::vec3*>(MemoryPool::Get(size * sizeof(glm::vec3)));
	m_specular = static_cast<glm::vec3*>(MemoryPool::Get(size * sizeof(glm::vec3)));
	m_emission = static_cast<glm::vec3*>(MemoryPool::Get(size * sizeof(glm::vec3)));
	m_shininess = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
}
