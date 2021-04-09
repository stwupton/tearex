#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tinygltf/tiny_gltf.h>

#include "model_info.hpp"
#include "util/debug.h"

class ModelLoader {
protected:
	uint8_t head = 0;

public:
	// TODO: Will we be loading more that 256 models?
	ModelInfo loaded[256];

protected:
	void applyNodeTransformations(glm::mat4 &transform, const tinygltf::Node &node) const {
		if (node.matrix.size() > 0) {
			assert(node.matrix.size() == 16);
			glm::mat4 matrix = glm::make_mat4(node.matrix.data());
			transform *= matrix;
		} else {
			if (node.translation.size() > 0) {
				assert(node.translation.size() == 3);
				glm::vec3 translation = glm::make_vec3(node.translation.data());
				transform = glm::translate(transform, translation);
			}

			if (node.rotation.size() > 0) {
				assert(node.rotation.size() == 4);
				glm::quat rotation = glm::make_quat(node.rotation.data());
				transform *= glm::mat4_cast(rotation);
			}

			if (node.scale.size() > 0) {
				assert(node.scale.size() == 3);
				glm::vec3 scale = glm::make_vec3(node.scale.data());
				transform = glm::scale(transform, scale);
			}
		}
	}

	void createAttributeBuffers(
		ModelInfo &modelInfo, 
		const tinygltf::Model &gltfData, 
		const std::map<std::string, int> &attributes
	) const {
		glGenBuffers(
			sizeof(modelInfo.attributeBufferIds) / sizeof(GLuint), 
			modelInfo.attributeBufferIds
		);

		int8_t bufferIdIndex = 0;
		for (const std::pair<const std::string, int> &attribute : attributes) {
			const GLuint bufferId = modelInfo.attributeBufferIds[bufferIdIndex];

			const tinygltf::Accessor &accessor = gltfData.accessors[attribute.second];
			const tinygltf::BufferView &bufferView = gltfData.bufferViews[accessor.bufferView];
			const tinygltf::Buffer &buffer = gltfData.buffers[bufferView.buffer];

			glBindBuffer(bufferView.target, bufferId);
			glBufferData(
				bufferView.target,
				bufferView.byteLength,
				&buffer.data.at(0) + bufferView.byteOffset,
				GL_STATIC_DRAW
			);

			// TODO: Come up with a way to make sure that all attributes are in the 
			// same location.
			uint8_t typeSize = this->resolveTinyGltfTypeSize(accessor.type);
			glVertexAttribPointer(
				bufferIdIndex, 
				typeSize, 
				accessor.componentType, 
				GL_FALSE, 
				sizeof(float) * typeSize, 
				0
			);
			glEnableVertexAttribArray(bufferIdIndex);

			bufferIdIndex++;
		}
	}

	void createIndexBuffer(
		ModelInfo &modelInfo, 
		const tinygltf::Model &gltfData, 
		const int &accessorIndex
	) const {
		const tinygltf::Accessor &accessor = gltfData.accessors[accessorIndex];
		const tinygltf::BufferView &bufferView = gltfData.bufferViews[accessor.bufferView];
		const tinygltf::Buffer &buffer = gltfData.buffers[bufferView.buffer];

		modelInfo.indexType = accessor.componentType;
		modelInfo.indexLength = accessor.count;

		glGenBuffers(1, &modelInfo.indexBufferId);
		glBindBuffer(bufferView.target, modelInfo.indexBufferId);
		glBufferData(
			bufferView.target,
			bufferView.byteLength,
			&buffer.data.at(0) + bufferView.byteOffset,
			GL_STATIC_DRAW
		);
	}

	void createMaterial(
		ModelInfo &modelInfo, 
		const tinygltf::Model &gltfData, 
		const int &materialIndex
	) const {
		const tinygltf::Material &material = gltfData.materials[materialIndex];
		const tinygltf::PbrMetallicRoughness &pbr = material.pbrMetallicRoughness;

		// TODO: Handle other textures
		if (pbr.baseColorTexture.index == -1) {
			return; 
		}

		const tinygltf::Texture &texture = gltfData.textures[pbr.baseColorTexture.index];
		const tinygltf::Image &image = gltfData.images[texture.source];

		glGenTextures(1, &modelInfo.textureId);
		glBindTexture(GL_TEXTURE_2D, modelInfo.textureId);
		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			GL_RGBA, 
			image.width, 
			image.height, 
			0, 
			GL_RGBA, 
			image.pixel_type, 
			image.image.data()
		);

		// Texture settings
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLfloat borderColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	tinygltf::Model loadModel(const std::string &fileName) const {
		tinygltf::TinyGLTF loader;
		tinygltf::Model model;
		std::string error;
		std::string warning;

		const std::string filePath = std::string(ASSET_PATH) + "models/" + fileName;
		const bool success = loader.LoadASCIIFromFile(
			&model, 
			&error, 
			&warning,
			filePath.c_str()
		);

		if (!warning.empty()) {
			LOG(warning.c_str())
		}

		if (!success) {
			if (!error.empty()) {
				LOG(error.c_str())
			}
			throw std::runtime_error("Could not loadFile model: " + fileName);
		}

		// TODO: make sure we are not reallocating all the buffer data by return the
		// gltfData object here.
		return model;
	}

	// TODO: We currently stop when we reach the first node that contains a mesh,
	// do we need to support multiple meshes?
	bool parseNode(
		ModelInfo &modelInfo,
		const tinygltf::Model &gltfData,
		const std::vector<int> &indices,
		glm::mat4 transform
	) const {
		for (const int &i : indices) {
			const tinygltf::Node &node = gltfData.nodes[i];
			
			glm::mat4 currentTransform = transform; 
			this->applyNodeTransformations(currentTransform, node);

			if (node.mesh > -1) {
				this->setModelInfo(modelInfo, gltfData, node.mesh, currentTransform);
				modelInfo.transform = currentTransform;
				return true;
			}

			if (node.children.size() > 0) {
				const bool parsed = this->parseNode(
					modelInfo, 
					gltfData, 
					node.children, 
					currentTransform
				);
				if (parsed) {
					return true;
				} else {
					continue;
				}
			}
		}

		return false;
	}

	int resolveTinyGltfTypeSize(const int &type) const {
		switch (type) {
			case TINYGLTF_TYPE_SCALAR:
				return 1;
			case TINYGLTF_TYPE_VEC2:
			case TINYGLTF_TYPE_VEC3:
			case TINYGLTF_TYPE_VEC4:
				return type;
			case TINYGLTF_TYPE_MAT2:
			case TINYGLTF_TYPE_MAT3:
			case TINYGLTF_TYPE_MAT4:
				return pow(type - 32, 2);
			default:
				return -1;
		}
	}

	void setModelInfo(
		ModelInfo &modelInfo, 
		const tinygltf::Model &gltfData, 
		const int &meshIndex, 
		const glm::mat4 &transform
	) const {
		const tinygltf::Mesh &mesh = gltfData.meshes[meshIndex];
		// TODO: Check if we need to iterate over primitives!
		const tinygltf::Primitive &primitive = mesh.primitives[0];

		glGenVertexArrays(1, &modelInfo.vertexArrayId);
		glBindVertexArray(modelInfo.vertexArrayId);

		// TODO: Handle interleaved vertex attributes
		this->createIndexBuffer(modelInfo, gltfData, primitive.indices);
		this->createAttributeBuffers(modelInfo, gltfData, primitive.attributes);
		this->createMaterial(modelInfo, gltfData, primitive.material);
	}

public:
	uint8_t load(const std::string &fileName) {
		ModelInfo &modelInfo = this->loaded[this->head];
		tinygltf::Model gltfData = loadModel(fileName);

		// TODO: Should we handle mutliple scenes?
		tinygltf::Scene &scene = gltfData.scenes[0];
		const bool parsed = this->parseNode(
			modelInfo, 
			gltfData, 
			scene.nodes, 
			glm::mat4(1.0f)
		);
		if (!parsed) {
			LOG("Model named: " << fileName << ", has no associated mesh.")
			return -1;
		}

		// TODO: Should be finding the first empty index here.
		return head++;
	}

	void unload(const std::string &name) {
		// TODO
	}

	void unloadAll() {
		for (const ModelInfo &model : this->loaded) {
			glDeleteVertexArrays(1, &model.vertexArrayId);
			glDeleteBuffers(
				sizeof(model.attributeBufferIds) / sizeof(uint32_t), 
				model.attributeBufferIds
			);
		}
	}
};