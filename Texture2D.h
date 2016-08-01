/*
 * =====================================================================================
 *
 *       Filename:  Texture2D.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月02日 00時43分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

class Texture2D
{
private:
	GLuint name;
	const GLenum unit;
	const GLenum target;
	std::string filePath;
public:
	Texture2D(GLenum u, const std::string& path):name(0), unit(u), target(GL_TEXTURE_2D), filePath(path)
	{
		
		glGenTextures(1, &name);
		
		
	}
	void init()
	{

	}
};

