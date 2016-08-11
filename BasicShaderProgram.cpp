/*
 * =====================================================================================
 *
 *       Filename:  BasicShaderProgram.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 20時08分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "BasicShaderProgram.h"


void BasicShaderProgram::init(void)
{
	vertShader.compile();
	fragShader.compile();
	
	shaderProgram.attach(fragShader);
	shaderProgram.attach(vertShader);
	shaderProgram.link();
	
	mvpMatrix.setLocation(shaderProgram.getHandle(), "mvpMatrix");
}

