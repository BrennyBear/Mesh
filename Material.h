#pragma once
#pragma once

#include "glad/glad.h"
#include<GLFW/glfw3.h>

#include<glm.hpp>

#include "Shader.h"

class Material
{
private:
	GLint diffuseTex1;
	GLint diffuseTex2;
	GLint specularTex;
	float shininess;
public:
	Material(
		GLint diffuseTex1,
		GLint diffuseTex2,
		GLint specularTex,
		float shininess = 50
	)
	{
		this->diffuseTex1 = diffuseTex1;
		this->diffuseTex2 = diffuseTex2;
		this->specularTex = specularTex;
		this->shininess = shininess;
	}
	~Material() {}

	//Function to load our Uniforms into GLSL shader
	void sendToShader(Shader& program)
	{
		program.use();
		program.setInt("material.diffuse1", this->diffuseTex1);
		program.setInt("material.diffuse2", this->diffuseTex2);
		program.setInt("material.specular", this->specularTex);
		program.setFloat("material.shininess", this->shininess);
		//program.unuse();    //Unbinding the program seems to cause all the textures to not load, even when the program is binded again before use
	
	}
};