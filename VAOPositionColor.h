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
#include "VBOStatic.h"



template <typename S>
class VAOPositionColor
{
private:
	GLuint handle;
	VBOStatic position;
	VBOStatic color;
	VBOElementStatic element;
	int vertexCount;
	S& shaderProgram;
public:
	VAOPositionColor(S& s):handle(0), position(), color(), element(), vertexCount(0), shaderProgram(s)
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
	void init(const std::vector<float>& p, const std::vector<float>& c, const std::vector<unsigned int>& e);
	void display(void)
	{
		//シェーダを使用開始
		Use<S> s(shaderProgram);
		//自身のVAOをバインド
		Bind<VAOPositionColor> b(*this);
		//インデックス配列をバインド
		Bind<VBOElementStatic> be(element);
		
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
		
	}
};



template <typename S>
void VAOPositionColor<S>::init
	(
		const std::vector<float>& p,
		const std::vector<float>& c,
		const std::vector<unsigned int>& e
	)
{
	//引数をバッファに格納
	position.bufferData(p);
	color.bufferData(c);
	element.bufferData(e);
	//頂点数を格納
	vertexCount = e.size();
	
	//自身のVAOをバインド
	Bind<VAOPositionColor> b(*this);
	
	//positionの設定
	{
		//頂点配列をバインド
		Bind<VBOStatic> bp(position);
		
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
		Bind<VBOStatic> bc(color);
		
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

