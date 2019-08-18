#include <iostream>
#include <memory>
#include "Core.hpp"
#include "Entity.hpp"
#include "Collisions.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

void temp(){
	std::cout << "ONE" << std::endl;
	Vex::CheckForErrors();
	std::cout << "TWO" << std::endl;
}

auto main() -> int {
	auto window = std::make_unique<Vex::Window>(1920, 1080, const_cast<char*>("Hello, World!"));

	//Shade::Entity e1("e1", "box", {-1.0f,-1.0f,-3.0f,0.0f,0.0f, -1.0f,-1.0f,-2.0f,0.0f,0.0f, -1.0f,1.0f,-3.0f,0.0f,0.0f, -1.0f,1.0f,-2.0f,0.0f,0.0f, 1.0f,-1.0f,-3.0f,0.0f,0.0f, 1.0f,-1.0f,-2.0f,0.0f,0.0f, 1.0f,1.0f,-3.0f,0.0f,0.0f, 1.0f,1.0f,-2.0f,0.0f,0.0f}, {0,1,2, 1,2,3, 0,2,4, 4,2,6, 0,1,4, 1,4,5, 1,3,5, 3,5,7, 2,3,6, 3,6,7, 4,5,6, 5,6,7});
	
	Vex::Entity e("e", {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f
			}, {
			1, 3, 0,
			2, 0, 3
	});

	auto t = std::make_unique<Vex::Texture>("blackhole.jpg");

	auto s = std::make_unique<Vex::Shader>("shaders/vert.glsl","shaders/frag.glsl");
	s->Bind();
	s->AddUniform("u_Texture");
	s->SetUniform("u_Texture", 0);


	Vex::Init = [](){
		std::cout << "Vex3D Initialized" << std::endl;
	};
	Vex::Update = [](float dt){
		for (auto& p : Vex::object_table){
			p.second->Update(dt);
		}
	};
	Vex::Render = [&s,&t](){
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		t->Bind();
		s->Bind();
		
		for (auto& p : Vex::object_table){
			p.second->internal_render();
			p.second->Render();
		}
	};

	window->Start();

	return 0;
}
