/*
 * =====================================================================================
 *
 *       Filename:  Texture2D.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年08月08日 02時39分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Texture2D.h"


void Texture2D::init(const std::string& filepath, int width, int height)
{
	//テクスチャデータの格納先
	std::vector<GLubyte> texture(width * height * 3, 0);
	
	//テクスチャファイルのファイルポインタ
	std::FILE* fp;
	//テクスチャファイルの読み込み
	if ((fp = std::fopen(filepath.c_str(), "rb")) != NULL)
	{
		std::fread(texture.data(), sizeof(GLubyte) * width * height * 3,1,fp);
		std::fclose(fp);
	}
	else
	{
		printf("error.\n");
		std::perror(filepath.c_str());
	}
	
	//テクスチャユニットの選択
	Active<Texture2D> a(*this);
	//テスクチャの選択
	Bind<Texture2D> b(*this);
	
	//画像データの格納方式を指定する(1画素が何バイト単位かを指定)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	//設定するテクスチャの指定
	glBindTexture(target, name);
	
	//テクスチャの割り当て
	gluBuild2DMipmaps(target, GL_RGB, width, height, GL_RGB , GL_UNSIGNED_BYTE, texture.data());
	//テクスチャを拡大・縮小するときの保管方法を設定 
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
}


