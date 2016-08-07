/*
 * =====================================================================================
 *
 *       Filename:  TextureShaderProgram.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月08日 03時27分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "TextureShaderProgram.h"


void TextureShaderProgram::init(void)
{
	vertShader.compile();
	fragShader.compile();
	
	shaderProgram.attach(fragShader);
	shaderProgram.attach(vertShader);
	shaderProgram.link();
	
	mvpMatrix.setLocation(shaderProgram.getHandle(), "mvpMatrix");
	
	textureSampler.setLocation(shaderProgram.getHandle(), "tex");
	textureSampler.set(0);
}

