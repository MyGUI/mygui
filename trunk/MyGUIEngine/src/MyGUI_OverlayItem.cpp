/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_OverlayItem.h"
#include "MyGUI_Gui.h"

#include <OgreHardwareBufferManager.h>

namespace MyGUI
{

	const size_t OVERLAY_ITEM_STEEP_REALLOCK = 30;
	const size_t OVERLAY_ITEM_POSITION_BINDING = 0;

	OverlayItem::OverlayItem(const Ogre::String& _material) :
		mNeedVertex(0),
		mCountVertex(OVERLAY_ITEM_STEEP_REALLOCK)
	{
		mRenderOp.vertexData = new Ogre::VertexData();
		// Vertex declaration: 1 position, add texcoords later depending on #layers
		// Create as separate buffers so we can lock & discard separately
		Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
		size_t offset = 0;
		decl->addElement(OVERLAY_ITEM_POSITION_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
		decl->addElement(OVERLAY_ITEM_POSITION_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
		decl->addElement(OVERLAY_ITEM_POSITION_BINDING, offset , Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);

		// Basic vertex data
		mRenderOp.vertexData->vertexStart = 0;
		mRenderOp.vertexData->vertexCount = mCountVertex;

		// Vertex buffer #1
		Ogre::HardwareVertexBufferSharedPtr vbuf =
			Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(OVERLAY_ITEM_POSITION_BINDING), mRenderOp.vertexData->vertexCount,
			// используем динамический, так как оч часто двигается
			Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
		// Bind buffer
		mRenderOp.vertexData->vertexBufferBinding->setBinding(OVERLAY_ITEM_POSITION_BINDING, vbuf);

		// No indexes & issue as a strip
		mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		mRenderOp.useIndexes = false;
		mRenderOp.vertexData->vertexStart = 0;

		updateView();
	}

	void OverlayItem::updatePositionGeometry()
	{
		if (mNeedVertex > mCountVertex) memoryAllocation();

		size_t count = 0;

		Ogre::HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(OVERLAY_ITEM_POSITION_BINDING);
		float* pPos = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

		for (VectorDrawItem::iterator iter=mVectorDrawItem.begin(); iter!=mVectorDrawItem.end(); ++iter) {
			(*iter)->_drawItem(pPos, count, mPixelScaleX, mPixelScaleY);
		}

		mRenderOp.vertexData->vertexCount = count;
		vbuf->unlock();

		mOutOfDate = false;
	}

	void OverlayItem::addDrawItem(DrawItem * _item, size_t _count)
	{
		mVectorDrawItem.push_back(_item);
		mNeedVertex += _count;
		mOutOfDate = true;
	}

	void OverlayItem::removeDrawItem(DrawItem * _item, size_t _count)
	{
		for (VectorDrawItem::iterator iter=mVectorDrawItem.begin(); iter!=mVectorDrawItem.end(); ++iter) {
			if ((*iter) == _item) {
				(*iter) = mVectorDrawItem.back();
				mVectorDrawItem.pop_back();
				break;
			}
		}
		mOutOfDate = true;
		mNeedVertex -= _count;
	}

	void OverlayItem::memoryAllocation()
	{
		mCountVertex = mNeedVertex + OVERLAY_ITEM_STEEP_REALLOCK;

		Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
		Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

		mRenderOp.vertexData->vertexCount = mCountVertex;

		// Create dynamic since text tends to change alot
		// positions & texcoords
		Ogre::HardwareVertexBufferSharedPtr vbuf =
			Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(OVERLAY_ITEM_POSITION_BINDING),
				mRenderOp.vertexData->vertexCount,
				Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
		bind->setBinding(OVERLAY_ITEM_POSITION_BINDING, vbuf);
	}

	void OverlayItem::updateView()
    {
		const FloatSize & size = Gui::getInstance().getViewSize();

		mPixelScaleX = 1.0 / size.width;
		mPixelScaleY = 1.0 / size.height;

		mOutOfDate = true;
	}

} // namespace MyGUI
