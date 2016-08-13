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

#include "Lock.h"

#include </usr/local/cuda-7.5/include/cuda_runtime.h>
#include </usr/local/cuda-7.5/include/cuda_gl_interop.h>

template<typename V>
class VBOBase
{
private:
	GLuint handle;
	const GLenum type;
	const GLenum usage;
public:
	VBOBase(GLenum t, GLenum u):handle(0),type(t),usage(u)
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
	void init(const std::vector<V>& v)
	{
		Bind<VBOBase<V> > b(*this);
		GLsizeiptr size = v.size() * sizeof(V);
		glBufferData(type, size, v.data(), usage);
	}
	GLuint getHandle()
	{
		return handle;
	}
};

//using VBOStatic        = VBOBase<        GL_ARRAY_BUFFER, GL_STATIC_DRAW,        float>;
//using VBOElementStatic = VBOBase<GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, unsigned int>;

class VBOStatic
{
private:
	VBOBase<float> base;
public:
	VBOStatic():base(GL_ARRAY_BUFFER,GL_STATIC_DRAW)
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
	void init(const std::vector<float>& v)
	{
		base.init(v);
	}
};

class VBOElementStatic
{
private:
	VBOBase<unsigned int> base;
public:
	VBOElementStatic():base(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
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
	void init(const std::vector<unsigned int>& v)
	{
		base.init(v);
	}
};


///////////////////////////

//VBODynamicBaseでのVBOのCUDA向けの設定

//VBODYNAMICBASE_STANDARD
//cudaのドキュメントに従った方法
//map(),unmap()でVBOのリソースをCUDA向けに確保する
//ドキュメントに従っているが、
//PCIeの通信量(rx, tx)が増加する(nvidia-smiで確認)
//
//VBODYNAMICBASE_LOWTHROUGHPUT
//cudaのドキュメントに従っていないが
//PCIeの通信量がほとんど増加しない方法
//init()時にリソースの確保とアドレスの取得をしてしまう
//map(),unmap()時は何もしない

//下記2行のうちのどちらか1方のみをコメントアウトして設定する
//#define VBODYNAMICBASE_STANDARD
#define VBODYNAMICBASE_LOWTHROUGHPUT

template <int VboType, typename ElementType>
class VBODynamicBase
{
private:
	VBOBase<ElementType> base;
	cudaGraphicsResource* resource;
	ElementType* device;
public:
	VBODynamicBase():base(VboType, GL_DYNAMIC_DRAW),resource(nullptr), device(nullptr)
	{
	}
	~VBODynamicBase()
	{
		if(resource != nullptr)
		{
			cudaGraphicsUnregisterResource(resource);
		}
	}
	void init(const std::vector<ElementType>& v)
	{
		base.init(v);
		cudaGraphicsGLRegisterBuffer(&resource, base.getHandle(), cudaGraphicsRegisterFlagsNone);
		
		#ifdef VBODYNAMICBASE_LOWTHROUGHPUT
		cudaGraphicsMapResources(1, &resource, nullptr);
		cudaGraphicsResourceGetMappedPointer((void**)&device, nullptr, resource);
		cudaGraphicsUnmapResources(1, &resource, nullptr);
		#endif
	}
	void bind()
	{
		base.bind();
	}
	void unbind()
	{
		base.unbind();
	}
	void map()
	{
		#ifdef VBODYNAMICBASE_STANDARD
		cudaGraphicsMapResources(1, &resource, nullptr);
		cudaGraphicsResourceGetMappedPointer((void**)&device, nullptr, resource);
		#endif
	}
	void unmap()
	{
		#ifdef VBODYNAMICBASE_STANDARD
		cudaGraphicsUnmapResources(1, &resource, nullptr);
		#endif
	}
	ElementType* getDevicePointer()
	{
		return device;
	}
};

///////////////////////////

using VBODynamic        = VBODynamicBase<GL_ARRAY_BUFFER        , float       >;
using VBOElementDynamic = VBODynamicBase<GL_ELEMENT_ARRAY_BUFFER, unsigned int>;



