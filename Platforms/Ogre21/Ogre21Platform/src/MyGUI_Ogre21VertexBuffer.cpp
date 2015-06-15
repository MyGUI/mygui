/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/

#include "MyGUI_Ogre21VertexBuffer.h"

#include <OgreHardwareVertexBuffer.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	unsigned int Ogre21VertexBuffer::GlobalID = 0;


	Ogre21VertexBuffer::Ogre21VertexBuffer() :
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK),
		mNeedVertexCount(0)
	{
		mUniqueID = GlobalID++;
		createVertexBuffer();
	}

	Ogre21VertexBuffer::~Ogre21VertexBuffer()
	{
		destroyVertexBuffer();
	}

	void Ogre21VertexBuffer::createVertexBuffer()
	{
		mRenderOperation.vertexData = new Ogre::v1::VertexData();
		mRenderOperation.vertexData->vertexStart = 0;

		Ogre::v1::VertexDeclaration* vd = mRenderOperation.vertexData->vertexDeclaration;
		vd->addElement( 0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
		vd->addElement( 0, Ogre::v1::VertexElement::getTypeSize( Ogre::VET_FLOAT3 ), Ogre::VET_COLOUR, Ogre::VES_DIFFUSE );
		vd->addElement( 0, Ogre::v1::VertexElement::getTypeSize( Ogre::VET_FLOAT3 ) +
			Ogre::v1::VertexElement::getTypeSize( Ogre::VET_COLOUR ),
			Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES );

		// Create the Vertex Buffer, using the Vertex Structure we previously declared in _declareVertexStructure.
		mVertexBuffer = Ogre::v1::HardwareBufferManager::getSingleton( ).createVertexBuffer(
			mRenderOperation.vertexData->vertexDeclaration->getVertexSize(0), // declared Vertex used
			mVertexCount,
			Ogre::v1::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
			false );

		// Bind the created buffer to the renderOperation object.  Now we can manipulate the buffer, and the RenderOp keeps the changes.
		mRenderOperation.vertexData->vertexBufferBinding->setBinding( 0, mVertexBuffer );
		mRenderOperation.operationType = Ogre::v1::RenderOperation::OT_TRIANGLE_LIST;
		mRenderOperation.useIndexes = false;
	}

	void Ogre21VertexBuffer::destroyVertexBuffer()
	{
		delete mRenderOperation.vertexData;
		mRenderOperation.vertexData = 0;
		mVertexBuffer.setNull();
	}

	void Ogre21VertexBuffer::resizeVertexBuffer()
	{
		mVertexCount = mNeedVertexCount + RENDER_ITEM_STEEP_REALLOCK;
		destroyVertexBuffer();
		createVertexBuffer();
	}

	void Ogre21VertexBuffer::setVertexCount(size_t _count)
	{
		mNeedVertexCount = _count;
	}

	void Ogre21VertexBuffer::setOperationVertexCount(size_t _count)
	{
		this->mRenderOperation.vertexData->vertexCount = _count;
	}

	size_t Ogre21VertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* Ogre21VertexBuffer::lock()
	{
		if (mNeedVertexCount > mVertexCount) resizeVertexBuffer();

		return reinterpret_cast<Vertex*>(mVertexBuffer->lock(Ogre::v1::HardwareVertexBuffer::HBL_DISCARD));
	}

	void Ogre21VertexBuffer::unlock()
	{
		mVertexBuffer->unlock();
	}
} // namespace MyGUI
