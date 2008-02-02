/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_Prerequest.h"
#include "MyGUI_SharedPanelAlphaOverlayElement.h"

#include <OgreHardwareBufferManager.h>
#include <OgreRoot.h>
#include <OgreRenderSystem.h>

namespace MyGUI
{
    String SharedPanelAlphaOverlayElement::msTypeName = "SharedPanelAlpha";

    //---------------------------------------------------------------------
	SharedPanelAlphaOverlayElement::SharedPanelAlphaOverlayElement(const String& name) :
		OverlayContainer(name),
        mTransparent(false),
		mColour(0xFFFFFFFF)
    {
    }
    //---------------------------------------------------------------------
    SharedPanelAlphaOverlayElement::~SharedPanelAlphaOverlayElement()
    {
        delete mRenderOp.vertexData;
    }
    //---------------------------------------------------------------------
    void SharedPanelAlphaOverlayElement::initialise(void)
    {
		bool init = !mInitialised;

		OverlayContainer::initialise();

		if (init) {
			// нужен хоть один саб оверлей
			MYGUI_DEBUG_ASSERT(0 != mVectorSharedOverlay.size(), "shared geometry overlay is empty");

			// Setup render op in advance
			mRenderOp.vertexData = new Ogre::VertexData();
			// Vertex declaration: 1 position, add texcoords later depending on #layers
			// Create as separate buffers so we can lock & discard separately
			Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
			size_t offset = 0;
			decl->addElement(OVERLAY_POSITION_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
			decl->addElement(OVERLAY_POSITION_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
			decl->addElement(OVERLAY_POSITION_BINDING, offset , Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);

			// Basic vertex data
			mRenderOp.vertexData->vertexStart = 0;
			mRenderOp.vertexData->vertexCount = 6 * mVectorSharedOverlay.size();

			// Vertex buffer #1
			Ogre::HardwareVertexBufferSharedPtr vbuf =
				Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				decl->getVertexSize(OVERLAY_POSITION_BINDING), mRenderOp.vertexData->vertexCount,
				// используем динамический, так как оч часто двигается
				Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY
				);
			// Bind buffer
			mRenderOp.vertexData->vertexBufferBinding->setBinding(OVERLAY_POSITION_BINDING, vbuf);

			// No indexes & issue as a strip
			mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
			mRenderOp.useIndexes = false;
			mRenderOp.vertexData->vertexStart = 0;

			mInitialised = true;
		}
    }
    
    void SharedPanelAlphaOverlayElement::setTransparent(bool isTransparent)
    {
        mTransparent = isTransparent;
    }
    //---------------------------------------------------------------------
    bool SharedPanelAlphaOverlayElement::isTransparent(void) const
    {
        return mTransparent;
    }
    
    const String& SharedPanelAlphaOverlayElement::getTypeName(void) const
    {
        return msTypeName;
    }
    //---------------------------------------------------------------------
    void SharedPanelAlphaOverlayElement::getRenderOperation(RenderOperation& op)
    {
        op = mRenderOp;
    }
    //---------------------------------------------------------------------
    void SharedPanelAlphaOverlayElement::setMaterialName(const String& matName)
    {
        OverlayContainer::setMaterialName(matName);
    }
    //---------------------------------------------------------------------
    void SharedPanelAlphaOverlayElement::_updateRenderQueue(RenderQueue* queue)
    {
        if (mVisible)
        {

            if (!mTransparent && !mpMaterial.isNull())
            {
                OverlayElement::_updateRenderQueue(queue);
            }

            // Also add children
            ChildIterator it = getChildIterator();
            while (it.hasMoreElements())
            {
                // Give children ZOrder 1 higher than this
                it.getNext()->_updateRenderQueue(queue);
            }
        }
    }
    
	void SharedPanelAlphaOverlayElement::updateTextureGeometry(void)
	{
	}

    void SharedPanelAlphaOverlayElement::updatePositionGeometry(void)
    {
		float left = _getDerivedLeft() * 2 - 1;
		float top = -((_getDerivedTop() * 2) - 1);

		Ogre::HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(OVERLAY_POSITION_BINDING);
		float* pPos = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD) );

		// Use the furthest away depth value, since materials should have depth-check off
		// This initialised the depth buffer for any 3D objects in front
		Ogre::Real zValue = Ogre::Root::getSingleton().getRenderSystem()->getMaximumDepthInputValue();

		for (VectorSharedOverlay::iterator iter=mVectorSharedOverlay.begin(); iter!=mVectorSharedOverlay.end(); ++iter) {

			OneOverlayDataInfo & info = *iter;

			// если прозрачны, то убираем вершины далеко далеко
			if (info.transparent) {
				*pPos++ = OVERLAY_POSITION_HIDE;
				*pPos++ = OVERLAY_POSITION_HIDE;
				pPos+=4;
				*pPos++ = OVERLAY_POSITION_HIDE;
				*pPos++ = OVERLAY_POSITION_HIDE;
				pPos+=4;
				*pPos++ = OVERLAY_POSITION_HIDE;
				*pPos++ = OVERLAY_POSITION_HIDE;
				pPos+=4;
				*pPos++ = OVERLAY_POSITION_HIDE;
				*pPos++ = OVERLAY_POSITION_HIDE;
				pPos+=4;
				*pPos++ = OVERLAY_POSITION_HIDE;
				*pPos++ = OVERLAY_POSITION_HIDE;
				pPos+=4;
				*pPos++ = OVERLAY_POSITION_HIDE;
				*pPos++ = OVERLAY_POSITION_HIDE;
				pPos+=4;
				continue;
			}

			float vertex_left = (mPixelScaleX * (info.left - mPixelLeft) * 2.0) + left;
			float vertex_top = top - (mPixelScaleY * (info.top - mPixelTop) * 2.0);
			float vertex_right = (mPixelScaleX * info.width * 2.0) + vertex_left;
			float vertex_bottom = vertex_top - (mPixelScaleY * info.height * 2.0);

			// first triangle
			// left top
			*pPos++ = vertex_left;
			*pPos++ = vertex_top;
			*pPos++ = zValue;
			*pPos++ = info.u1;
			*pPos++ = info.v1;
			*pPos++ = *((float*)(&mColour));

			// first triangle
			// left bottom
			*pPos++ = vertex_left;
			*pPos++ = vertex_bottom;
			*pPos++ = zValue;
			*pPos++ = info.u1;
			*pPos++ = info.v2;
			*pPos++ = *((float*)(&mColour));

			// first triangle
			// right top
			*pPos++ = vertex_right;
			*pPos++ = vertex_top;
			*pPos++ = zValue;
			*pPos++ = info.u2;
			*pPos++ = info.v1;
			*pPos++ = *((float*)(&mColour));

			// second triangle
			// right top
			*pPos++ = vertex_right;
			*pPos++ = vertex_top;
			*pPos++ = zValue;
			*pPos++ = info.u2;
			*pPos++ = info.v1;
			*pPos++ = *((float*)(&mColour));

			// second triangle
			// left bottom
			*pPos++ = vertex_left;
			*pPos++ = vertex_bottom;
			*pPos++ = zValue;
			*pPos++ = info.u1;
			*pPos++ = info.v2;
			*pPos++ = *((float*)(&mColour));

			// second triangle
			// right botton
			*pPos++ = vertex_right;
			*pPos++ = vertex_bottom;
			*pPos++ = zValue;
			*pPos++ = info.u2;
			*pPos++ = info.v2;
			*pPos++ = *((float*)(&mColour));

		}

		vbuf->unlock();
    }

	void SharedPanelAlphaOverlayElement::addBaseParameters(void)
    {
    }

} // namespace MyGUI
