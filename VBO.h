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

//using VBODynamic        = VBOBase<        GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,        float>;
//using VBOElementDynamic = VBOBase<GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW, unsigned int>;


class VBODynamic
{
private:
	VBOBase<float> base;
	cudaGraphicsResource* resource;
	float* device;
public:
	VBODynamic():base(GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW),resource(nullptr), device(nullptr)
	{
	}
	~VBODynamic()
	{
		if(resource != nullptr)
		{
			cudaGraphicsUnregisterResource(resource);
		}
	}
	void init(const std::vector<float>& v)
	{
		base.init(v);
		cudaGraphicsGLRegisterBuffer(&resource, base.getHandle(), cudaGraphicsRegisterFlagsNone);
		
		//cudaGraphicsMapResources(1, &resource, 0);
		//cudaGraphicsResourceGetMappedPointer((void**)&device, nullptr, resource);
		//cudaGraphicsUnmapResources(1, &resource, 0);
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
		cudaGraphicsMapResources(1, &resource, NULL);
		cudaGraphicsResourceGetMappedPointer((void**)&device, NULL, resource);
	}
	void unmap()
	{
		cudaGraphicsUnmapResources(1, &resource, NULL);
	}
	float* getDevicePointer()
	{
		return device;
	}
};

class VBOElementDynamic
{
private:
	VBOBase<unsigned int> base;
	cudaGraphicsResource* resource;
	unsigned int* device;
public:
	VBOElementDynamic():base(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW), resource(nullptr), device(nullptr)
	{
	}
	~VBOElementDynamic()
	{
		if(resource != nullptr)
		{
			cudaGraphicsUnregisterResource(resource);
		}
	}
	void init(const std::vector<unsigned int>& v)
	{
		base.init(v);
		cudaGraphicsGLRegisterBuffer(&resource, base.getHandle(), cudaGraphicsRegisterFlagsNone);
		
		//cudaGraphicsMapResources(1, &resource, 0);
		//cudaGraphicsResourceGetMappedPointer((void**)&device, nullptr, resource);
		//cudaGraphicsUnmapResources(1, &resource, 0);
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
		cudaGraphicsMapResources(1, &resource, NULL);
		cudaGraphicsResourceGetMappedPointer((void**)&device, NULL, resource);
	}
	void unmap()
	{
		cudaGraphicsUnmapResources(1, &resource, NULL);
	}
	unsigned int* getDevicePointer()
	{
		return device;
	}
};


