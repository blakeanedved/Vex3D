#ifndef _VEX3D_ENTITY_
#define _VEX3D_ENTITY_
#include "Core.hpp"
#include "GameObject.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <limits>

namespace Vex {
	class Entity : public GameObject {
		public:
			bool generateAABB = true;
		private:
			std::vector<float> vertices;
			std::vector<unsigned int> indices;
			std::vector<float> AABB;
			GLuint vaoID;
			GLuint iboID;
			GLuint vboID;

		public:
			Entity(std::string name, glm::vec3 position, glm::vec3 rotation, std::vector<float> vertices, std::vector<unsigned int> indices);
			~Entity();
			auto Init() -> void;

			auto GetVertices() -> std::vector<float>;
			auto GetAABB() -> std::vector<float>;

			auto GenerateAABB() -> void;
			auto internal_render() -> void;
	};
}

Vex::Entity::Entity(std::string name, glm::vec3 position, glm::vec3 rotation, std::vector<float> vertices, std::vector<unsigned int> indices) : GameObject(name, position, rotation) {
	this->vertices = vertices;
	this->indices = indices;

	glGenVertexArrays(1, &this->vaoID);
    glBindVertexArray(this->vaoID);

	glGenBuffers(1, &this->vboID);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices[0], GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, reinterpret_cast<void*>(3 * sizeof(GLfloat)));

	glGenBuffers(1, &this->iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

	this->AABB = std::vector<float>(6);
	this->GenerateAABB();
}

Vex::Entity::~Entity(){
}

auto Vex::Entity::GetVertices() -> std::vector<float> {
	return this->vertices;
}

auto Vex::Entity::GetAABB() -> std::vector<float> {
	return this->AABB;
}

auto Vex::Entity::GenerateAABB() -> void {
	this->AABB[0] = std::numeric_limits<float>::max();
	this->AABB[1] = std::numeric_limits<float>::max();
	this->AABB[2] = std::numeric_limits<float>::max();
	this->AABB[3] = std::numeric_limits<float>::min();
	this->AABB[4] = std::numeric_limits<float>::min();
	this->AABB[5] = std::numeric_limits<float>::min();
	for (int i = 0; i < this->vertices.size(); i+=5){
		this->AABB[0] = std::min(this->vertices[i], this->AABB[0]);
		this->AABB[1] = std::min(this->vertices[i+1], this->AABB[1]);
		this->AABB[2] = std::min(this->vertices[i+2], this->AABB[2]);
		this->AABB[3] = std::max(this->vertices[i], this->AABB[3]);
		this->AABB[4] = std::max(this->vertices[i+1], this->AABB[4]);
		this->AABB[5] = std::max(this->vertices[i+2], this->AABB[5]);
	}
}

auto Vex::Entity::internal_render() -> void {
	glBindVertexArray(this->vaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->iboID);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
}

#endif
