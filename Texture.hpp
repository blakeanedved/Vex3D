#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include "Core.hpp"
#include "stb_image/stb_image.h"


namespace Vex {
	class Texture {
		private:
			unsigned char *localBuffer;
			int width, height, channels;
			unsigned int textureID;

		public:
			Texture(const std::string &filepath);
			~Texture();

			auto Bind(unsigned int slot = 0) -> void;
			auto Unbind() -> void;
	};
}


Vex::Texture::Texture(const std::string &filepath) {
    stbi_set_flip_vertically_on_load(1);
    this->localBuffer = stbi_load(filepath.c_str(), &this->width, &this->height, &this->channels, 4);

    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->localBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (this->localBuffer)
        stbi_image_free(this->localBuffer);
}

Vex::Texture::~Texture() {
	glDeleteTextures(1, &this->textureID);
}

auto Vex::Texture::Bind(GLuint slot) -> void {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
}

auto Vex::Texture::Unbind() -> void {
	glBindTexture(GL_TEXTURE_2D, 0);
}
