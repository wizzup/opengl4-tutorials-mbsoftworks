#include "pyramid.h"

#include <glm/glm.hpp>

namespace static_meshes_3D {

glm::vec3 Pyramid::vertices[12] =
{
	// Front face
	glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
	// Back face
	glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f),
	// Left face
	glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
	// Right face
	glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f)
};

glm::vec2 Pyramid::textureCoordinates[3] =
{
	glm::vec2(0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f)
};

Pyramid::Pyramid(bool withPositions, bool withTextureCoordinates, bool withNormals)
	: StaticMesh3D(withPositions, withTextureCoordinates, withNormals)
{
	initializeData();
}

void Pyramid::render() const
{
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 12);
}

void Pyramid::initializeData()
{
	if (_isInitialized) {
		return;
	}

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	const int numVertices = 12;
	int vertexByteSize = getVertexByteSize();	
	_vbo.createVBO(vertexByteSize * numVertices);
	_vbo.bindVBO();
	
	if (hasPositions())
	{
		_vbo.addData(vertices, sizeof(glm::vec3)*numVertices);
	}

	if (hasTextureCoordinates())
	{
		for (auto i = 0; i < 4; i++)
		{
			_vbo.addData(textureCoordinates, sizeof(glm::vec2)*3);
		}
	}

	if (hasNormals())
	{
		for (auto i = 0; i < 4; i++)
		{
			const auto& posA = vertices[i * 3];
			const auto& posB = vertices[i * 3 + 1];
			const auto& posC = vertices[i * 3 + 2];
			const auto vecA = posB - posA;
			const auto vecB = posC - posA;
			auto cp = glm::normalize(glm::cross(vecA, vecB));
			_vbo.addData(&cp, sizeof(glm::vec3), 3);
		}
	}

	_vbo.uploadDataToGPU(GL_STATIC_DRAW);
	setVertexAttributesPointers(numVertices);

	_isInitialized = true;
}

} // namespace static_meshes_3D