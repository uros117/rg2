#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include<glad/gl.h>
#include<stb/stb_image.h>

#include"Shader.hpp"

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);
	Texture(GLuint id, const char* texType, GLuint slot);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform) const;
	// Binds a texture
	void bind() const;
	// Unbinds a texture
	void unbind();
	// Deletes a texture
	void del();
};
#endif