/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#include "MyGUI_OsgVertexBuffer.h"

#include <osg/Array>
#include <osg/BufferObject>
#include <osg/GL>

namespace MyGUI
{

	OsgVertexBuffer::OsgVertexBuffer()
	{
	}

	OsgVertexBuffer::~OsgVertexBuffer()
	{
	}

	void OsgVertexBuffer::markUsed()
	{
		mUsed = true;
	}

	void OsgVertexBuffer::setVertexCount(size_t _count)
	{
		if (_count == mNeedVertexCount)
			return;

		mNeedVertexCount = _count;
	}

	size_t OsgVertexBuffer::getVertexCount() const
	{
		return mNeedVertexCount;
	}

	Vertex* OsgVertexBuffer::lock()
	{
		if (mUsed)
		{
			mCurrentBuffer = (mCurrentBuffer + 1) % 4;
			mUsed = false;
		}
		osg::UByteArray* array = mVertexArray[mCurrentBuffer];
		if (!array)
		{
			array = create();
		}
		else if (array->size() != mNeedVertexCount * sizeof(Vertex))
		{
			array->resize(mNeedVertexCount * sizeof(Vertex));
		}

		return static_cast<Vertex*>(static_cast<void*>(&(*array)[0]));
	}

	void OsgVertexBuffer::unlock()
	{
		mVertexArray[mCurrentBuffer]->dirty();
		mBuffer[mCurrentBuffer]->dirty();
	}

	osg::UByteArray* OsgVertexBuffer::create()
	{
		mVertexArray[mCurrentBuffer] =
			new osg::UByteArray(static_cast<unsigned int>(mNeedVertexCount * sizeof(Vertex)));

		mBuffer[mCurrentBuffer] = new osg::VertexBufferObject;
		mBuffer[mCurrentBuffer]->setDataVariance(osg::Object::DYNAMIC);
		mBuffer[mCurrentBuffer]->setUsage(GL_DYNAMIC_DRAW);
		// NB mBuffer does not own the array
		mBuffer[mCurrentBuffer]->setArray(0, mVertexArray[mCurrentBuffer].get());

		return mVertexArray[mCurrentBuffer];
	}

	osg::Array* OsgVertexBuffer::getVertexArray()
	{
		return mVertexArray[mCurrentBuffer];
	}

	osg::VertexBufferObject* OsgVertexBuffer::getVertexBuffer()
	{
		return mBuffer[mCurrentBuffer];
	}

} // namespace MyGUI
