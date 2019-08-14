#ifndef _SHADE_ENGINE_ENTITY_
#define _SHADE_ENGINE_ENTITY_
#include <iostream>
#include <vector>
#include <map>
#include <string>

namespace Shade {
	class Entity;
	std::map<std::string, Shade::Entity*> entity_table;
	std::map<std::string, std::vector<Shade::Entity*>> type_table;
	class Entity {
		public:
			std::string name;
			std::string type;
			bool generateAABB = true;
		private:
			std::vector<float> vertices;
			std::vector<float> AABB;

		public:
			Entity(std::string name, std::string type, std::vector<float> vertices){
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
				this->AABB = std::vector<float>(6);
				this->GenerateAABB();
			}
			~Entity(){
				if (entity_table.find(this->name)->second == this){
					entity_table.erase(this->name);
					auto temp = type_table.find(this->type);
					temp->second.erase(std::find(temp->second.begin(), temp->second.end(), this));
				}	
			}
			auto GetVertices() -> std::vector<float> {
				return this->vertices;
			}
			auto GetAABB() -> std::vector<float> {
				return this->AABB;
			}
			auto GenerateAABB() -> void {
				this->AABB[0] = this->vertices[0];
				this->AABB[3] = this->vertices[0];
				this->AABB[1] = this->vertices[1];
				this->AABB[4] = this->vertices[1];
				this->AABB[2] = this->vertices[2];
				this->AABB[5] = this->vertices[2];
				for (int i = 3; i < this->vertices.size(); i+=3){
					this->AABB[0] = std::min(this->vertices[i], this->AABB[0]);
					this->AABB[1] = std::min(this->vertices[i+1], this->AABB[1]);
					this->AABB[2] = std::min(this->vertices[i+2], this->AABB[2]);
					this->AABB[3] = std::max(this->vertices[i], this->AABB[3]);
					this->AABB[4] = std::max(this->vertices[i+1], this->AABB[4]);
					this->AABB[5] = std::max(this->vertices[i+2], this->AABB[5]);
				}
			}
	};
}
#endif
