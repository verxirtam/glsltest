/*
 * =====================================================================================
 *
 *       Filename:  VBOStatic.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時47分08秒
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

#include <vector>


template<int T, int U, typename V>
class VBOBase
{
private:
	GLuint handle;
	const GLenum type;
	const GLenum usage;
public:
	VBOBase():handle(0),type(T),usage(U)
	{
		glGenBuffers(1, &handle);
	}
	void bind()
	{
		glBindBuffer(type, handle);
	}
	void unbind()
	{
		glBindBuffer(type, 0);
	}
	void bufferData(const std::vector<V>& v)
	{
		this->bind();
		GLsizeiptr size = v.size() * sizeof(V);
		glBufferData(type, size, v.data(), usage);
	}
};

typedef VBOBase<GL_ARRAY_BUFFER, GL_STATIC_DRAW, float> VBOStatic;
typedef VBOBase<GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, unsigned int> VBOElementStatic;

