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


//このクラスのインスタンスがスコープ内にいる限りuseになる
//mutex的な使い方。
//useのままスコープを外れることが防止できる
template<typename Usable>
class Use
{
private:
	Usable& u;
public:
	Use(Usable& _u):u(_u)
	{
		u.use();
	}
	~Use()
	{
		u.unuse();
	}
};


//このクラスのインスタンスがスコープ内にいる限りbindされる
template<typename Bindable>
class Bind
{
private:
	Bindable& b;
public:
	Bind(Bindable& _b):b(_b)
	{
		b.bind();
	}
	~Bind()
	{
		b.unbind();
	}
};
//このクラスのインスタンスがスコープ内にいる限りactiveされる
template<typename Activatable>
class Active
{
private:
	Activatable& a;
public:
	Active(Activatable& _a):a(_a)
	{
		a.active();
	}
	~Active()
	{
		a.inactive();
	}
};

/*

template<typename Lockable>
class Lock
{
private:
	Lockable& l;
	auto unlock;
public:
	Lock(Lockable& _l, auto _lock, auto _unlock):l(_l),unlock(_unlock)
	{
		_lock(_l);
	}
	~Lock()
	{
		unlock(l);
	}
};

*/

