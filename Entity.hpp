#ifndef _SHADE_ENGINE_ENTITY_
#define _SHADE_ENGINE_ENTITY_
#include <vector>

namespace Shade {
	class Entity {
		public:
			bool generateAABB = true;
		private:
			std::vector<float> vertices;
			std::vector<float> AABB;

		public:
			Entity(std::vector<float> vertices){
				this->vertices = vertices;
				this->AABB = std::vector<float>(6);
				this->GenerateAABB();
			}
			~Entity(){}
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
