#pragma once

#include "Core.hpp"

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Vex {
	class Shader {
		private:
			unsigned int programID;
			std::map<std::string, unsigned int> uniforms;

		public:
			Shader(const char* fragmentShader, const char* vertexShader);
			~Shader();

			auto AddUniform(const std::string& uniformName) -> void;
			auto Bind() -> void;
			auto SetUniform(const std::string& uniformName, GLuint value) -> void;
			auto SetUniform(const std::string& uniformName, glm::mat4 value) -> void;
			auto SetUniform(const std::string& uniformName, glm::vec4 value) -> void;
			auto SetUniform(const std::string& uniformName, glm::vec3 value) -> void;

		private:
			auto LoadShaders(const char* fragmentShader, const char* vertexShader) -> GLuint;
	};

	std::unique_ptr<Vex::Shader> DefaultShader;
	auto ShaderInit() -> void {
		Vex::DefaultShader = std::make_unique<Vex::Shader>("shaders/default/vert.glsl", "shaders/default/frag.glsl");
		Vex::DefaultShader->AddUniform("MVP");
		Vex::DefaultShader->AddUniform("pos");
		Vex::DefaultShader->AddUniform("rotMatrix");
	}
}

Vex::Shader::Shader(const char* fragmentShader, const char* vertexShader) {
    this->programID = this->LoadShaders(fragmentShader, vertexShader);
}

Vex::Shader::~Shader() {}

auto Vex::Shader::Bind() -> void {
	glUseProgram(this->programID);
}

auto Vex::Shader::AddUniform(const std::string& uniformName) -> void {
    if (auto it = this->uniforms.find(uniformName);
        it == this->uniforms.end()) {
        this->uniforms.insert(
            {uniformName,
             glGetUniformLocation(this->programID, uniformName.c_str())});
    }
}

auto Vex::Shader::SetUniform(const std::string& uniformName, GLuint value) -> void {
    if (auto it = this->uniforms.find(uniformName);
        it != this->uniforms.end()) {
        glUniform1i(it->second, value);
    }
}

auto Vex::Shader::SetUniform(const std::string& uniformName, glm::mat4 value) -> void {
    if (auto it = this->uniforms.find(uniformName);
        it != this->uniforms.end()) {
        glUniformMatrix4fv(it->second, 1, GL_FALSE, &value[0][0]);
    }
}

auto Vex::Shader::SetUniform(const std::string& uniformName, glm::vec4 value) -> void {
    if (auto it = this->uniforms.find(uniformName);
        it != this->uniforms.end()) {
        glUniform4fv(it->second, 1, &value[0]);
    }
}

auto Vex::Shader::SetUniform(const std::string& uniformName, glm::vec3 value) -> void {
    if (auto it = this->uniforms.find(uniformName);
        it != this->uniforms.end()) {
        glUniform3fv(it->second, 1, &value[0]);
    }
}

auto Vex::Shader::LoadShaders(const char* vertex_file_path, const char* fragment_file_path) -> GLuint {

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    } else {
		std::cout << "Error at Vex::Shader::LoadShaders(const char*, const char*) in Vex::Shader.hpp\nError loading file \"" << vertex_file_path << "\" file does not exist" << std::endl;
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    } else {
		std::cout << "Error at Vex::Shader::LoadShaders(const char*, const char*) in Vex::Shader.hpp\nError loading file \"" << fragment_file_path << "\" file does not exist" << std::endl;
        return 0;
	}

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL,
                           &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL,
                           &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL,
                            &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}
