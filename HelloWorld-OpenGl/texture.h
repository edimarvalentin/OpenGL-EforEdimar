#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad.h>
#include<string>
#include<iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

class Texture {
public:
	unsigned int ID;

	Texture(const char* texturePath) {
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID); // All GL_TEXTURE_2D operations take effect on this texture

		//Texture Wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set the wrapping to GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Texture Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// load image, create texture and generate mipmaps
		int width, height, nrChannels;

		unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	void use() {
		glBindTexture(GL_TEXTURE_2D, ID);
	}
};


#endif