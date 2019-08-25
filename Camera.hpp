#ifndef _VEX3D_CAMERA_
#define _VEX3D_CAMERA_

#include "GameObject.hpp"

namespace Vex {
	class Camera : public GameObject {
		private:
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;
			glm::mat4 MVP;
			float FoV;
			float aspectRatio;

		public:
			Camera(std::string name, glm::vec3 position, glm::vec3 rotation, float FoV, float aspectRatio);
			virtual ~Camera();

			auto GetMVP() -> glm::mat4;

			auto SetPosition(glm::vec3 position) -> void;
			auto SetRotation(glm::vec3 rotation) -> void;
			auto SetRotation(glm::vec3 rotation, glm::vec3 origin) -> void;

			auto SetChildPosition(glm::vec3 position) -> void;

			auto Move(glm::vec3 adjustment) -> void;
			auto Rotate(glm::vec3 rotation) -> void;
			auto Rotate(glm::vec3 rotation, glm::vec3 origin) -> void;
		private:
			auto update_model() -> void;
			auto update_view() -> void;
			auto update_projection() -> void;

			auto internal_render() -> void;
	};

	std::shared_ptr<Vex::Camera> ActiveCamera;
}

Vex::Camera::Camera(std::string name, glm::vec3 position, glm::vec3 rotation, float FoV, float aspectRatio) : GameObject(name, position, rotation) {
	this->FoV = FoV;
	this->aspectRatio = aspectRatio;

	this->update_projection();
	this->update_view();
	this->update_model();

	this->MVP = this->projection * this->view * this->model;
}

Vex::Camera::~Camera() {
}

auto Vex::Camera::GetMVP() -> glm::mat4 {
	return this->MVP;
}

auto Vex::Camera::SetPosition(glm::vec3 position) -> void {
	GameObject::SetPosition(position);
	this->update_view();
}

auto Vex::Camera::SetRotation(glm::vec3 rotation) -> void {
	GameObject::SetRotation(rotation);
	this->update_view();
}

auto Vex::Camera::SetRotation(glm::vec3 rotation, glm::vec3 origin) -> void {
	GameObject::SetRotation(rotation, origin);
	this->update_view();
};

auto Vex::Camera::SetChildPosition(glm::vec3 position) -> void {
	GameObject::SetChildPosition(position);
	this->update_view();
};

auto Vex::Camera::Move(glm::vec3 adjustment) -> void {
	GameObject::Move(adjustment);
	this->update_view();
};

auto Vex::Camera::Rotate(glm::vec3 rotation) -> void {
	GameObject::Rotate(rotation);
	this->update_view();
};

auto Vex::Camera::Rotate(glm::vec3 rotation, glm::vec3 origin) -> void {
	GameObject::Rotate(rotation, origin);
	this->update_view();
};

auto Vex::Camera::update_model() -> void {
	this->model = glm::mat4(1.0f);

	this->MVP = this->projection * this->view * this->model;
}

auto Vex::Camera::update_view() -> void {
	this->view = glm::mat4(1.0f);
	this->view = glm::rotate(this->view, this->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	this->view = glm::rotate(this->view, this->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	this->view = glm::rotate(this->view, this->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	this->view = glm::translate(this->view, -this->position);

	this->MVP = this->projection * this->view * this->model;
};

auto Vex::Camera::update_projection() -> void {
	this->projection = glm::perspective(
		glm::radians(this->FoV),
		this->aspectRatio,
		0.1f,
		100.0f
	);

	this->MVP = this->projection * this->view * this->model;
}

auto Vex::Camera::internal_render() -> void {} // Pretty sure this has to be here just to override the pure virual GameObject::internal_render()

#endif
