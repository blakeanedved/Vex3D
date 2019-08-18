#include <iostream>
#include <memory>
#include "Core.hpp"
#include "Entity.hpp"
#include "Collisions.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

auto main() -> int {
	auto window = std::make_unique<Vex::Window>(1920, 1080, const_cast<char*>("Hello, World!"));
	
	Vex::Entity e("e", glm::vec3(0.0f), glm::vec3(0.0f), {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f
			}, {
			1, 3, 0,
			2, 0, 3
	});

	auto c = std::make_unique<Vex::Camera>("main_camera", glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), 60.0f, 16.0f / 10.0f);

	auto t = std::make_unique<Vex::Texture>("resources/images/blackhole.jpg");

	auto s = std::make_unique<Vex::Shader>("shaders/basic/vert.glsl","shaders/basic/frag.glsl");
	s->Bind();
	s->AddUniform("u_Texture");
	s->AddUniform("MVP");
	s->AddUniform("pos");
	s->AddUniform("rotMatrix");
	s->SetUniform("u_Texture", 0);
	s->SetUniform("MVP", c->GetMVP());

	Vex::Init = [&c](){
		std::cout << "Vex3D Initialized" << std::endl;
		Vex::ShaderInit();
		Vex::DefaultShader->Bind();
		Vex::DefaultShader->SetUniform("MVP", c->GetMVP());
	};
	Vex::Update = [&c,&e](float dt){
		c->Rotate(glm::vec3(0.0f, 0.5f * dt, 0.0f));
		e.Rotate(glm::vec3(0.5f * dt, 0.5f * dt, 0.5f * dt));
		for (auto& p : Vex::object_table){
			p.second->Update(dt);
		}
	};
	Vex::Render = [&s,&t,&c](){
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Vex::DefaultShader->Bind();
		Vex::DefaultShader->SetUniform("MVP", c->GetMVP());
	
		t->Bind();
		s->Bind();

		s->SetUniform("MVP", c->GetMVP());
		for (auto& p : Vex::object_table){
			s->SetUniform("pos", p.second->GetPosition());
			s->SetUniform("rotMatrix", p.second->GenerateRotMatrix());
			p.second->internal_render();
			p.second->Render();
		}
	};

	window->Start();

	return 0;
}
