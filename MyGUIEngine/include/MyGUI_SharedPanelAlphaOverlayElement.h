/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_SHARED_PANEL_ALPHA_OVERLAY_ELEMENT_H__
#define __MYGUI_SHARED_PANEL_ALPHA_OVERLAY_ELEMENT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include <OgrePanelOverlayElement.h>
#include <OgreHardwareBufferManager.h>
#include <OgreRenderSystem.h>
#include <OgreStringConverter.h>
#include <OgreRoot.h>
#include "MyGUI_RenderOut.h"

namespace MyGUI
{

	class _MyGUIExport SharedPanelAlphaOverlayElement : public Ogre::PanelOverlayElement
	{
	protected:
		enum {
			OVERLAY_POSITION_BINDING = 0,
			OVERLAY_POSITION_HIDE = -50000
		};

		struct OneOverlayDataInfo
		{
			OneOverlayDataInfo() : offsetX(0), offsetY(0), left(0), top(0), width(0), height(0), u1(0), v1(0), u2(0), v2(0), transparent(false) {}

			float offsetX, offsetY;
			float left, top, width, height;
			float u1, v1, u2, v2;
			bool transparent;
		};
		typedef std::vector<OneOverlayDataInfo> VectorSharedOverlay;

	protected:
		// цвет вершины
		Ogre::uint32 mColour;
		// вектор всех наших саб оверлеев
		VectorSharedOverlay mVectorSharedOverlay;
		// свой способ сокрытия и показа
		//bool mIsShowOverlay;

	public:

		SharedPanelAlphaOverlayElement(const Ogre::String& _name) :
			PanelOverlayElement(_name),
			mColour(0xFFFFFFFF)
			//mIsShowOverlay(true)
		{
		}

		inline void setOverlay(Ogre::Overlay * _overlay)
		{
			mOverlay = _overlay;
		}

		void setColour(Ogre::uint32 _colour)
		{
			mColour = _colour;
			mGeomPositionsOutOfDate = true;
		}

		inline void setCountSharedOverlay(size_t _count)
		{
			mVectorSharedOverlay.resize(_count);
			mGeomPositionsOutOfDate = true;
		}

		inline void setPositionInfo(float _left, float _top, size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id);
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			info.left = _left;
			info.top = _top;

			// если это главный оверлей, то дублируем
			if (_id == 0) Ogre::PanelOverlayElement::setPosition(_left, _top);

			mGeomPositionsOutOfDate = true;
		}

		inline void setPositionInfo(float _left, float _top, float _width, float _height, size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id);
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			info.left = _left;
			info.top = _top;
			info.width = _width;
			info.height = _height;

			// если это главный оверлей, то дублируем
			if (_id == 0) Ogre::PanelOverlayElement::setPosition(_left, _top);

			mGeomPositionsOutOfDate = true;
		}

		inline void setDimensionInfo(float _width, float _height, size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id);
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			info.width = _width;
			info.height = _height;
			mGeomPositionsOutOfDate = true;
		}

		inline void setTransparentInfo(bool _transparent, size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id);
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			info.transparent = _transparent;
			mGeomPositionsOutOfDate = true;
		}

		inline void setUVInfo(float _u1, float _v1, float _u2, float _v2, size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id);
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			info.u1 = _u1;
			info.v1 = _v1;
			info.u2 = _u2;
			info.v2 = _v2;
			mGeomPositionsOutOfDate = true;
		}

		inline bool isTransparentInfo(size_t _id)
		{
			MYGUI_DEBUG_ASSERT(mVectorSharedOverlay.size() > _id);
			OneOverlayDataInfo & info = mVectorSharedOverlay.at(_id);
			return info.transparent;
		}

		virtual void initialise()
		{
			bool init = !mInitialised;

			OverlayContainer::initialise();

			if (init) {
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

		void updatePositionGeometry(void)
		{

			/*if (false == mIsShowOverlay) {
				mRenderOp.vertexData->vertexCount = 0;
				return;
			} else {
				mRenderOp.vertexData->vertexCount = 6 * mVectorSharedOverlay.size();
			}*/

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

		virtual void updateTextureGeometry() { }

		/*void setTransparent(bool isTransparent)
		{
			assert(0 || !"setTransparent");
		}*/

		/** Shows this element if it was hidden. */
        /*void show(void)
		{
			Ogre::PanelOverlayElement::show();
			MYGUI_OUT("show");
			//mIsShowOverlay = true;
			//mGeomPositionsOutOfDate = true;
			//assert(0 || !"show");
		}*/

        /** Hides this element if it was visible. */
        /*void hide(void)
		{
			Ogre::PanelOverlayElement::hide();
			MYGUI_OUT("hide");
			//mIsShowOverlay = false;
			//mGeomPositionsOutOfDate = true;
			//assert(0 || !"hide");
		}*/

	}; // class SharedPanelAlphaOverlayElement : public PanelOverlayElement

} // namespace MyGUI

#endif // __MYGUI_SHARED_PANEL_ALPHA_OVERLAY_ELEMENT_H__