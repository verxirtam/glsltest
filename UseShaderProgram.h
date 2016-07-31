/*
 * =====================================================================================
 *
 *       Filename:  UseShaderProgram.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年07月31日 19時43分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "ShaderProgram.h"

//お遊びクラス
//このクラスのインスタンスがスコープ内にいる限りShaderProgramがuseになる
//mutex的な使い方。
//useのままスコープを外れることが防止できる
class UseShaderProgram
{
private:
	ShaderProgram& sp;
public:
	UseShaderProgram(ShaderProgram& s):sp(s)
	{
		sp.use();
	}
	~UseShaderProgram()
	{
		sp.unuse();
	}
};
