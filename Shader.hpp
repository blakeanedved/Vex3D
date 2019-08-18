#ifndef _SHADE_ENGINE_SHADER_
#define _SHADE_ENGINE_SHADER_

#include "Core.hpp"
#include <map>
#include <sstream>
#include <fstream>

namespace Shade {
	class Shader {
		private:
			unsigned int programID;
			std::map<std::string, unsigned int> uniforms;

		public:
			Shader(const char *fragmentShader, const char *vertexShader);
			~Shader();

			auto AddUniform(const std::string &uniformName) -> void;
			auto Bind() -> void;
			auto SetUniform(const std::string &uniformName, GLuint value) -> void;
			auto SetUniform(const std::string &uniformName, glm::mat4 value) -> void;
			auto SetUniform(const std::string &uniformName, glm::vec4 value) -> void;

		private:
			auto LoadShaders(const char *fragmentShader, const char *vertexShader) -> GLuint;
	};
}

Shade::Shader::Shader(const char *fragmentShader, const char *vertexShader) {
    this->programID = this->LoadShaders(fragmentShader, vertexShader);
}

Shade::Shader::~Shader() {}

auto Shade::Shader::Bind() -> void { glUseProgram(this->programID); }

auto Shade::Shader::AddUniform(const std::string &uniformName) -> void {
    if (auto it = this->uniforms.find(uniformName);
        it == this->uniforms.end()) {
        this->uniforms.insert(
            {uniformName,
             glGetUniformLocation(this->programID, uniformName.c_str())});
    }
}

auto Shade::Shader::SetUniform(const std::string &uniformName, GLuint value) -> void {
    if (auto it = this->uniforms.find(uniformName);
        it != this->uniforms.end()) {
        glUniform1i(it->second, value);
    }
}

auto Shade::Shader::SetUniform(const std::string &uniformName, glm::mat4 value)
    -> void {
    if (auto it = this->uniforms.find(uniformName);
        it != this->uniforms.end()) {
        glUniformMatrix4fv(it->second, 1, GL_FALSE, &value[0][0]);
    }
}

auto Shade::Shader::SetUniform(const std::string &uniformName, glm::vec4 value)
    -> void {
    if (auto it = this->uniforms.find(uniformName);
        it != this->uniforms.end()) {
        glUniform4fv(it->second, 4, &value[0]);
    }
}

auto Shade::Shader::LoadShaders(const char *vertex_file_path,
                         const char *fragment_file_path) -> GLuint {

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
        printf("Impossible to open %s. Are you in the right directory ? Don't "
               "forget to read the FAQ !\n",
               vertex_file_path);
        getchar();
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

#endif