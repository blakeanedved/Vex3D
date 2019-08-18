#ifndef _SHADE_ENGINE_ENTITY_
#define _SHADE_ENGINE_ENTITY_
#include "Core.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <string>

namespace Shade {
	class Entity {
		public:
			std::string name;
			std::string type;
			bool generateAABB = true;
			std::function<void(float)> Update = [](float dt){};
			std::function<void()> Render = [](){};
			glm::vec3 position;
			glm::vec3 rotation;
		private:
			std::vector<float> vertices;
			std::vector<unsigned int> indices;
			std::vector<float> AABB;
			GLuint vaoID;
			GLuint iboID;
			GLuint vboID;
			// make children

		public:
			Entity(std::string name, std::string type, std::vector<float> vertices, std::vector<unsigned int> indices);
			~Entity();
			auto Init() -> void;

			// Getters
			auto GetVertices() -> std::vector<float>;
			auto GetAABB() -> std::vector<float>;

			auto GenerateAABB() -> void;
			auto internal_render() -> void;
	};

	std::map<std::string, Shade::Entity*> entity_table;
	std::map<std::string, std::vector<Shade::Entity*>> type_table;
}

Shade::Entity::Entity(std::string name, std::string type, std::vector<float> vertices, std::vector<unsigned int> indices){
	if (entity_table.find(name) != entity_table.end()){
		std::cout << "Entity " << name << " already exists" << std::endl;
		exit(1);
	}
	entity_table.insert(std::make_pair(name, this));
	if (auto tt = type_table.find(type); tt == type_table.end()){
		type_table.insert(std::make_pair(type, std::vector<Shade::Entity*>{this}));
	} else {
		tt->second.push_back(this);
	}
	this->name = std::move(name);
	this->type = std::move(type);
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
	//this->GenerateAABB();
}

Shade::Entity::~Entity(){
	if (entity_table.find(this->name)->second == this){
		entity_table.erase(this->name);
		auto temp = type_table.find(this->type);
		temp->second.erase(std::find(temp->second.begin(), temp->second.end(), this));
	}
}

auto Shade::Entity::GetVertices() -> std::vector<float> {
	return this->vertices;
}

auto Shade::Entity::GetAABB() -> std::vector<float> {
	return this->AABB;
}

auto Shade::Entity::GenerateAABB() -> void {
	this->AABB[0] = INT_MAX;
	this->AABB[1] = INT_MAX;
	this->AABB[2] = INT_MAX;
	this->AABB[3] = INT_MIN;
	this->AABB[4] = INT_MIN;
	this->AABB[5] = INT_MIN;
	for (int i = 0; i < this->vertices.size(); i+=3){
		this->AABB[0] = std::min(this->vertices[i], this->AABB[0]);
		this->AABB[1] = std::min(this->vertices[i+1], this->AABB[1]);
		this->AABB[2] = std::min(this->vertices[i+2], this->AABB[2]);
		this->AABB[3] = std::max(this->vertices[i], this->AABB[3]);
		this->AABB[4] = std::max(this->vertices[i+1], this->AABB[4]);
		this->AABB[5] = std::max(this->vertices[i+2], this->AABB[5]);
	}
}

auto Shade::Entity::internal_render() -> void {
	glBindVertexArray(this->vaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->iboID);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
}
#endif
