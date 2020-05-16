/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/

#include "MyGUI_OgreVertexBuffer.h"

#include <OgreHardwareVertexBuffer.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	const size_t VERTEX_BUFFER_REALLOCK_STEP = 5 * VertexQuad::VertexCount;

	OgreVertexBuffer::OgreVertexBuffer() :
		mVertexCount(0),
		mNeedVertexCount(0)
	{
	}

	OgreVertexBuffer::~OgreVertexBuffer()
	{
		destroy();
	}

	void OgreVertexBuffer::create()
	{
		mRenderOperation.vertexData = new Ogre::VertexData();
		mRenderOperation.vertexData->vertexStart = 0;

		Ogre::VertexDeclaration* vd = mRenderOperation.vertexData->vertexDeclaration;
		vd->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		vd->addElement(0, Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3), Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
		vd->addElement(
			0,
			Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3) + Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR),
			Ogre::VET_FLOAT2,
			Ogre::VES_TEXTURE_COORDINATES);

		// Create the Vertex Buffer, using the Vertex Structure we previously declared in _declareVertexStructure.
		mVertexBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			mRenderOperation.vertexData->vertexDeclaration->getVertexSize(0), // declared Vertex used
			mVertexCount,
			Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
			false);

		// Bind the created buffer to the renderOperation object.  Now we can manipulate the buffer, and the RenderOp keeps the changes.
		mRenderOperation.vertexData->vertexBufferBinding->setBinding(0, mVertexBuffer);
		mRenderOperation.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		mRenderOperation.useIndexes = false;
	}

	void OgreVertexBuffer::destroy()
	{
		delete mRenderOperation.vertexData;
		mRenderOperation.vertexData = nullptr;
		mVertexBuffer.reset();
	}

	void OgreVertexBuffer::resize()
	{
		mVertexCount = mNeedVertexCount + VERTEX_BUFFER_REALLOCK_STEP;
		destroy();
		create();
	}

	void OgreVertexBuffer::setVertexCount(size_t _count)
	{
		mNeedVertexCount = _count;
	}

	size_t OgreVertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* OgreVertexBuffer::lock()
	{
		if (mNeedVertexCount > mVertexCount || mVertexCount == 0)
			resize();

		return reinterpret_cast<Vertex*>(mVertexBuffer->lock(Ogre::HardwareVertexBuffer::HBL_DISCARD));
	}

	void OgreVertexBuffer::unlock()
	{
		mVertexBuffer->unlock();
	}

} // namespace MyGUI
