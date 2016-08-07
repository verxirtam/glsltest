/*
 * =====================================================================================
 *
 *       Filename:  VAOPositionTexture.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月08日 03時49分15秒
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
#include "Texture2D.h"


template <typename S>
class VAOPositionTexture
{
private:
	GLuint handle;
	VBOStatic position;
	VBOStatic textureCoord;
	VBOElementStatic element;
	int vertexCount;
	S& shaderProgram;
	Texture2D& texture;
public:
	VAOPositionTexture(S& s, Texture2D& t):
		handle(0),
		position(),
		textureCoord(),
		element(),
		vertexCount(0),
		shaderProgram(s),
		texture(t)
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
		//テクスチャのバインド
		Active<Texture2D> ta(texture);
		Bind<Texture2D> tb(texture);
		//自身のVAOをバインド
		Bind<VAOPositionTexture> b(*this);
		//インデックス配列をバインド
		Bind<VBOElementStatic> be(element);
		
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
		
	}
};



template <typename S>
void VAOPositionTexture<S>::init
	(
		const std::vector<float>& p,
		const std::vector<float>& t,
		const std::vector<unsigned int>& e
	)
{
	//引数をバッファに格納
	position.bufferData(p);
	textureCoord.bufferData(t);
	element.bufferData(e);
	//頂点数を格納
	vertexCount = e.size();
	
	//自身のVAOをバインド
	Bind<VAOPositionTexture> b(*this);
	
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
	//textureCoordの設定
	{
		//テクスチャ座標のの配列をバインド
		Bind<VBOStatic> bt(textureCoord);
		
		glVertexAttribPointer
			(
				1,		//設定するバーテックスシェーダの引数のインデックスを指定する
				2,		//1頂点あたりの要素数(ここでは2次元座標なので2)
				GL_FLOAT,	//要素の型
				GL_FALSE,	//正規化の要否 位置座標なのでFalse
				0,		//頂点データ同士の間隔(byte単位) 0なら隙間なく配置されているとみなされる
				(GLubyte*)NULL	//頂点データの開始アドレスから指定するデータの位置までの間隔
			);
		glEnableVertexAttribArray(1);
	}
}

