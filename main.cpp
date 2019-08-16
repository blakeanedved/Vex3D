#include <iostream>
#include <memory>
#include "Core.hpp"
#include "Entity.hpp"
#include "Collisions.hpp"

// temp function
auto printAABB(Shade::Entity& e) -> void {
	auto aabb = e.GetAABB();
	std::cout << e.name << "\nmin: (" << aabb[0] << "," << aabb[1] << "," << aabb[2] << ")\nmax: (" << aabb[3] << "," << aabb[4] << "," << aabb[5] << ")\n" << std::endl;
}

auto main() -> int {
	//Shade::Entity e1("e1", "box", {-1,-1,-1, -1,-1,1, -1,1,-1, -1,1,1, 1,-1,-1, 1,-1,1, 1,1,-1, 1,1,1});
	//Shade::Entity e2("e2", "box", {2,2,2, 2,2,4, 2,4,2, 2,4,4, 4,2,2, 4,2,4, 4,4,2, 4,4,4});
	//Shade::Entity e3("e3", "box", {0,0,0, 0,0,2, 0,2,0, 0,2,2, 2,0,0, 2,0,2, 2,2,0, 2,2,2});
	
	//printAABB(e1);
	//printAABB(e2);
	//printAABB(e3);
	
	//std::cout << (Shade::Collisions::AABB(e1, e2)?"colliding":"not colliding") << std::endl;
	//std::cout << (Shade::Collisions::AABB(e1, e3)?"colliding":"not colliding") << std::endl;
	
	Shade::Init = [](){
		std::cout << "Shade Engine Initialized" << std::endl;
	};
	Shade::Update = [](float dt){
	};
	Shade::Render = [](){
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	};

	auto window = std::make_unique<Shade::Window>(1920, 1080, const_cast<char*>("Gaymer Time"));

	return 0;
}
