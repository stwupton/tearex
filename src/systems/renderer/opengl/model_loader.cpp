#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <tinygltf/tiny_gltf.h>

#include "components/model.hpp"
#include "util/debug.h"

tinygltf::Model loadViaTinyGltf(const std::string &fileName) {
	tinygltf::TinyGLTF loader;
	tinygltf::Model model;
	std::string error;
	std::string warning;

	const bool success = loader.LoadASCIIFromFile(
		&model, 
		&error, 
		&warning,
		"./../assets/models/" + fileName
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

	// TODO:: make sure we are not reallocating all the buffer data by return the
	// model object here.
	return model;
}

// TODO: Cleanup
Model loadModel(const std::string &fileName) {
	Model modelComponent;

	tinygltf::Model model = loadViaTinyGltf(fileName);

	for (const tinygltf::Scene &scene : model.scenes) {
		for (const int &nodeIndex : scene.nodes) {
			// TODO: Temporary solution to render a spoon. We should be traversing 
			// node and applying any transforms.
			tinygltf::Node &node = model.nodes[nodeIndex];
			while (node.mesh == -1 && node.children.size() > 0) {
				node = model.nodes[node.children[0]];
			}

			const tinygltf::Mesh &mesh = model.meshes[node.mesh];
			// TODO: Loop through all primitives
			const tinygltf::Primitive &primitive = mesh.primitives[0];

			glGenVertexArrays(1, &modelComponent.vertexArrayId);
			glBindVertexArray(modelComponent.vertexArrayId);

			// Generate index buffer
			{
				const tinygltf::Accessor &accessor = model.accessors[primitive.indices];
				const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
				const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

				modelComponent.indexType = accessor.componentType;
				modelComponent.vertexLength = accessor.count;

				glGenBuffers(1, &modelComponent.indexBufferId);
				glBindBuffer(bufferView.target, modelComponent.indexBufferId);
				glBufferData(
					bufferView.target,
					bufferView.byteLength,
					&buffer.data.at(0) + bufferView.byteOffset,
					GL_STATIC_DRAW
				);
			}

			// Generate vertex buffers
			glGenBuffers(
				sizeof(modelComponent.bufferIds) / sizeof(uint32_t), 
				modelComponent.bufferIds
			);

			int8_t bufferIdIndex = 0;
			for (const std::pair<const std::string, int> &attribute : primitive.attributes) {
				const GLuint bufferId = modelComponent.bufferIds[bufferIdIndex];

				const tinygltf::Accessor &accessor = model.accessors[attribute.second];
				const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
				const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

				// TODO: Handle all types (scalar, vector & matrix)
				uint8_t typeSize;
				switch (accessor.type) {
					case TINYGLTF_TYPE_VEC3:
						typeSize = 3;
						break;
				}

				glBindBuffer(bufferView.target, bufferId);
				glBufferData(
					bufferView.target,
					bufferView.byteLength,
					&buffer.data.at(0) + bufferView.byteOffset,
					GL_STATIC_DRAW
				);

				// TODO: Come up with a way to make sure that all attributes are in the 
				// same location.
				const GLuint vertexAttribId = bufferIdIndex;
				glVertexAttribPointer(
					vertexAttribId, 
					typeSize, 
					accessor.componentType, 
					GL_FALSE, 
					sizeof(float) * typeSize, 
					0
				);
				glEnableVertexAttribArray(vertexAttribId);

				bufferIdIndex++;
			}
		}
	}

	return modelComponent;
}