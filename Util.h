/*
 * =====================================================================================
 *
 *       Filename:  Util.h
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


//このクラスのインスタンスがスコープ内にいる限りbeginになる
//mutex的な使い方。
//beginのままスコープを外れることが防止できる
//define文の引数：
//		クラス名
//		コンストラクタで実行する関数名
//		デストラクタで実行する関数名
#define DEF_LOCK(ClassName, begin, end) \
	template<typename Lockable>\
	class ClassName\
	{\
	private:\
		Lockable& l;\
	public:\
		ClassName(Lockable& _l):l(_l)\
		{\
			l.begin();\
		}\
		~ClassName()\
		{\
			l.end();\
		}\
	};

DEF_LOCK(   Use,    use,    unuse)
DEF_LOCK(  Bind,   bind,   unbind)
DEF_LOCK(Active, active, inactive)


