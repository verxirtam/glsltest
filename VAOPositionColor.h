/*
 * =====================================================================================
 *
 *       Filename:  VAOPositionColor.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時48分15秒
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

#include "Util.h"
#include "VBO.h"



template <typename S, typename VBOClass, typename VBOElementClass>
class VAOPositionColorBase
{
private:
	GLuint handle;
	VBOClass position;
	VBOClass color;
	VBOElementClass element;
	GLenum mode;
	int vertexCount;
	S& shaderProgram;
public:
	VAOPositionColorBase(S& s)
		:
			handle(0),
			position(),
			color(),
			element(),
			mode(0),
			vertexCount(0),
			shaderProgram(s)
	{
		glGenVertexArrays(1, &handle);
	}
	void bind()
	{
		glBindVertexArray(handle);
	}
	void unbind()
	{
		glBindVertexArray(0);
	}
	void init(const std::vector<float>& p, const std::vector<float>& c, const std::vector<unsigned int>& e, GLenum m);
	void display(void)
	{
		//シェーダを使用開始
		Use<S> s(shaderProgram);
		//自身のVAOをバインド
		Bind<VAOPositionColorBase<S, VBOClass, VBOElementClass> > b(*this);
		//インデックス配列をバインド
		Bind<VBOElementClass> be(element);
		
		glDrawElements(mode, vertexCount, GL_UNSIGNED_INT, 0);
		
	}
	int getVertexCount()
	{
		return vertexCount;
	}
	VBOClass& getPosition()
	{
		return position;
	}
	VBOClass& getColor()
	{
		return color;
	}
	VBOElementClass& getElement()
	{
		return element;
	}
};



template <typename S, typename VBOClass, typename VBOElementClass>
void VAOPositionColorBase<S, VBOClass, VBOElementClass>::init
	(
		const std::vector<float>& p,
		const std::vector<float>& c,
		const std::vector<unsigned int>& e,
		GLenum m
	)
{
	//引数をバッファに格納
	position.init(p);
	color.init(c);
	element.init(e);
	
	//modeの設定
	mode = m;
	
	//頂点数を格納
	vertexCount = e.size();
	
	//自身のVAOをバインド
	Bind<VAOPositionColorBase<S, VBOClass, VBOElementClass> > b(*this);
	
	//positionの設定
	{
		//頂点配列をバインド
		Bind<VBOClass> bp(position);
		
		glVertexAttribPointer
			(
				0,		//設定するバーテックスシェーダの引数のインデックスを指定する
				3,		//1頂点あたりの要素数(ここでは3次元座標なので3)
				GL_FLOAT,	//要素の型
				GL_FALSE,	//正規化の要否 位置座標なのでFalse
				0,		//頂点データ同士の間隔(byte単位) 0なら隙間なく配置されているとみなされる
				(GLubyte*)NULL	//頂点データの開始アドレスから指定するデータの位置までの間隔
			);
		glEnableVertexAttribArray(0);
	}
	//colorの設定
	{
		//頂点色の配列をバインド
		Bind<VBOClass> bc(color);
		
		glVertexAttribPointer
			(
				1,		//設定するバーテックスシェーダの引数のインデックスを指定する
				3,		//1頂点あたりの要素数(ここでは3次元座標なので3)
				GL_FLOAT,	//要素の型
				GL_FALSE,	//正規化の要否 位置座標なのでFalse
				0,		//頂点データ同士の間隔(byte単位) 0なら隙間なく配置されているとみなされる
				(GLubyte*)NULL	//頂点データの開始アドレスから指定するデータの位置までの間隔
			);
		glEnableVertexAttribArray(1);
	}
}

template <typename S>
using VAOPositionColor = VAOPositionColorBase<S, VBOStatic, VBOElementStatic>;

//template <typename S>
//using VAOPositionColorDynamic = VAOPositionColorBase<S, VBODynamic, VBOElementDynamic>;
template <typename S>
class VAOPositionColorDynamic
{
private:
	VAOPositionColorBase<S, VBODynamic, VBOElementDynamic> base;


public:
	VAOPositionColorDynamic(S& s):base(s)
	{
	}
	void bind()
	{
		base.bind();
	}
	void unbind()
	{
		base.unbind();
	}
	void init(const std::vector<float>& p, const std::vector<float>& c, const std::vector<unsigned int>& e, GLenum m)
	{
		base.init(p, c, e, m);
	}
	void display()
	{
		base.display();
	}
	void map()
	{
		base.getPosition().map();
		base.getColor().map();
		base.getElement().map();
	}
	void unmap()
	{
		base.getPosition().unmap();
		base.getColor().unmap();
		base.getElement().unmap();
	}
	float* getPositionDevicePointer()
	{
		return base.getPosition().getDevicePointer();
	}
	float* getColorDevicePointer()
	{
		return base.getColor().getDevicePointer();
	}
	unsigned int* getElementDevicePointer()
	{
		return base.getElement().getDevicePointer();
	}
	int getVertexCount()
	{
		return base.getVertexCount();
	}
};


