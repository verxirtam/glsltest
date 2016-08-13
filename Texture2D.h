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


#include <GL/glew.h>
#include <GL/glut.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "Lock.h"


class Texture2D
{
private:
	GLuint name;
	const GLenum unit;
	const GLenum target;
public:
	Texture2D(GLenum u):name(0), unit(u), target(GL_TEXTURE_2D)
	{
		
		//テクスチャユニットの選択
		Active<Texture2D> a(*this);
		
		//テクスチャの生成
		glGenTextures(1, &name);
		
	}
	void active()
	{
		//テクスチャユニットの選択
		glActiveTexture(unit);
	}
	void inactive()
	{
		glActiveTexture(GL_TEXTURE0);
	}
	void bind()
	{
		glBindTexture(target, name);
	}
	void unbind()
	{
		glBindTexture(target, 0);
	}
	
	void init(const std::string& filepath, int width, int height);
	
};

