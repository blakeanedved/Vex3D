#include <iostream>
#include <memory>
#include "Core.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Input.hpp"

auto main() -> int {
	auto window = std::make_unique<Vex::Window>(1920, 1080, const_cast<char*>("Hello, World!"));
    
    Vex::Mesh e("e", glm::vec3(0.0f), glm::vec3(0.0f), "resources/models/head.dae");

	Vex::Input::Init(window->GetWindow());

	auto c = std::make_shared<Vex::Camera>("main_camera", glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), 60.0f, 16.0f / 10.0f);
	Vex::ActiveCamera = c;

	auto t = std::make_unique<Vex::Texture>("resources/images/headUVcolor.png");

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
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

		// Default Shader Setup
		Vex::ShaderInit();
		Vex::DefaultShader->Bind();
		Vex::DefaultShader->SetUniform("MVP", c->GetMVP());
	};
	Vex::Update = [&window,&e](float dt){
		Vex::Input::Poll();
        e.Rotate(glm::vec3(0.2f * dt));
		//c->Rotate(glm::vec3(0.0f, 0.5f * dt, 0.0f));
		if (Vex::Input::KeyReleased("Escape")){
			window->Close();
		}
		for (auto& p : Vex::object_table){
			p.second->Update(dt);
		}
	};
	Vex::Render = [&s,&t](){
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set MVP uniform on the Default Shader
		Vex::DefaultShader->Bind();
		Vex::DefaultShader->SetUniform("MVP", Vex::ActiveCamera->GetMVP());
	
		t->Bind();

		s->Bind();
		s->SetUniform("MVP", Vex::ActiveCamera->GetMVP());

		for (auto& p : Vex::object_table){
			// Model Specific Default Shader Uniform assignment
			Vex::DefaultShader->Bind();
			Vex::DefaultShader->SetUniform("pos", p.second->GetPosition());
			Vex::DefaultShader->SetUniform("rotMatrix", p.second->GenerateRotMatrix());

			s->Bind();
			s->SetUniform("pos", p.second->GetPosition());
			s->SetUniform("rotMatrix", p.second->GenerateRotMatrix());
			p.second->internal_render();
			p.second->Render();
		}
	};

	window->Start();

	return 0;
}
