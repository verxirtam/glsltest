/*
 * =====================================================================================
 *
 *       Filename:  ShaderProgram.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時41分25秒
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


#include <iostream>


class ShaderProgram
{
private:
	GLuint handle;
	
	//コピー・ムーブを禁止するため下記の宣言のみ行う
	//コピーコンストラクタ
	ShaderProgram(const ShaderProgram& s);
	//コピー代入演算子
	ShaderProgram& operator=(const ShaderProgram& s);
	//ムーブコンストラクタ
	ShaderProgram(ShaderProgram&& s);
	//ムーブ代入演算子
	ShaderProgram& operator=(ShaderProgram&& s);
public:
	ShaderProgram():handle(0)
	{
		handle = glCreateProgram();
		if(handle == 0)
		{
			std::cout << "error at glCreateProgram()." << std::endl;
		}
	}
	~ShaderProgram()
	{
		glDeleteProgram(handle);
	}
	void attach(const Shader& s)
	{
		glAttachShader(handle, s.getHandle());
	}
	void link()
	{
		glLinkProgram(handle);
		
		//リンクの結果確認
		GLint status;
		glGetProgramiv(handle, GL_LINK_STATUS, &status);
		if(status == GL_FALSE)
		{
			std::cout << "error at glLinkProgram(handle)." << std::endl;
		}
	}
	void use()
	{
		//OpenGLパイプラインにインストール
		glUseProgram(handle);
	}
	void unuse()
	{
		//OpenGLパイプラインプログラムを割り当てない
		glUseProgram(0);
	}
	GLuint getHandle()
	{
		return handle;
	}
};
