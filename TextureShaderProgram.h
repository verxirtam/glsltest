/*
 * =====================================================================================
 *
 *       Filename:  TextureShaderProgram.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月08日 02時59分45秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "Shader.h"
#include "ShaderProgram.h"
#include "UniformVariable.h"
#include "VAOPositionTexture.h"

class TextureShaderProgram
{
private:
	Shader vertShader;
	Shader fragShader;
	ShaderProgram shaderProgram;
	UniformVariable<glm::mat4> mvpMatrix;
	UniformVariable<int> textureSampler;
public:
	typedef VAOPositionTexture<TextureShaderProgram> vaoType;
	TextureShaderProgram()
		:
			vertShader(GL_VERTEX_SHADER,  "texture.vert"),
			fragShader(GL_FRAGMENT_SHADER,"texture.frag"),
			shaderProgram(),
			mvpMatrix(),
			textureSampler()
	{
	}
	void init(void);
	void use()
	{
		shaderProgram.use();
	}
	void unuse()
	{
		shaderProgram.unuse();
	}
	void setMVPMatrix(const glm::mat4& m)
	{
		shaderProgram.use();
		mvpMatrix.set(m);
		shaderProgram.unuse();
	}
};
