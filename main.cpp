#include <iostream>
#include <memory>
#include "Core.hpp"
#include "Entity.hpp"
#include "Collisions.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Input.hpp"

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

	Vex::Input::InputInit(window->GetWindow());

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
		Vex::Input::PollInput();
		if(Vex::Input::KeyDown("Space")){
			e.Rotate(glm::vec3(0.5f * dt, 0.5f * dt, 0.5f * dt));
		}
		//c->Rotate(glm::vec3(0.0f, 0.5f * dt, 0.0f));
		if(Vex::Input::KeyDown("Up")){
			e.SetPosition(e.GetPosition() + glm::vec3(0.0f, 0.9f*dt, 0.0f));
		}else if(Vex::Input::KeyDown("Down")){
			e.SetPosition(e.GetPosition() + glm::vec3(0.0f, -0.9f*dt, 0.0f));
		}
		if(Vex::Input::KeyDown("Left")){
			e.SetPosition(e.GetPosition() + glm::vec3(-0.9f*dt, 0.0f, 0.0f));
		}else if(Vex::Input::KeyDown("Right")){
			e.SetPosition(e.GetPosition() + glm::vec3(0.9f*dt, 0.0f, 0.0f));
		}
		if(Vex::Input::KeyPressed("Left_Shift")){
			glm::vec2 mousePos = Vex::Input::GetMousePosition();
			std::cout << "Left Shift Down, mouse position is: " << mousePos.x << ", " << mousePos.y << std::endl;
			Vex::Input::HideCursor(true);
		}
		if(Vex::Input::KeyReleased("Left_Shift")){
			std::cout << "Left Shift Up" << std::endl;
			Vex::Input::ShowCursor();
		}
		if(Vex::Input::MouseButtonPressed(1)){
			std::cout << "Right Mouse Button Down" << std::endl;
		}
		if(Vex::Input::MouseButtonReleased(1)){
			std::cout << "Right Mouse Button Up" << std::endl;
		}
		if(Vex::Input::MouseButtonDown(0)){
			glm::vec2 delta = Vex::Input::GetMouseDelta();
			std::cout << "Mouse Delta: " << delta.x << ", " << delta.y << std::endl;
		}
		if(Vex::Input::KeyDown("A")){
			std::cout << "Scroll: " << Vex::Input::GetScrollVertical() << std::endl;
		}
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
