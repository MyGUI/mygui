#ifndef _PANELALPHAOVERLAYELEMENT_H_
#define _PANELALPHAOVERLAYELEMENT_H_

#include "Prerequest.h"
//#include <Ogre.h>
#include <OgrePanelOverlayElement.h>
#include <OgreHardwareBufferManager.h>
#include <OgreRenderSystem.h>
#include <OgreStringConverter.h>
#include <OgreRoot.h>

namespace MyGUI
{

	using namespace Ogre;

    // vertex buffer bindings, set at compile time (we could look these up but no point)
    #define POSITION_BINDING 0
    #define TEXCOORD_BINDING 1

	class _MyGUIExport PanelAlphaOverlayElement : public PanelOverlayElement
	{
	protected:
		// цвет вершины
		Ogre::uint32 m_color;

	public:

		PanelAlphaOverlayElement(const String& name) :
			PanelOverlayElement(name),
			m_color(0xFFFFFFFF)
		{
		}

		inline void setOverlay(Ogre::Overlay * _overlay)
		{
			mOverlay = _overlay;
		}

		void setColor(Ogre::uint32 _color)
		{
			m_color = _color;
			mGeomPositionsOutOfDate = true;
		}

		virtual void initialise()
		{
			bool init = !mInitialised;

			OverlayContainer::initialise();

			if (init) {
				// Setup render op in advance
				mRenderOp.vertexData = new VertexData();
				// Vertex declaration: 1 position, add texcoords later depending on #layers
				// Create as separate buffers so we can lock & discard separately
				VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
				size_t offset = 0;
				decl->addElement(POSITION_BINDING, offset, VET_FLOAT3, VES_POSITION);
				offset += VertexElement::getTypeSize(VET_FLOAT3);
				decl->addElement(POSITION_BINDING, offset , VET_COLOUR, VES_DIFFUSE); // добавленно
				offset += VertexElement::getTypeSize(VET_COLOUR);

				// Basic vertex data
				mRenderOp.vertexData->vertexStart = 0;
				mRenderOp.vertexData->vertexCount = 4;

				// Vertex buffer #1
				HardwareVertexBufferSharedPtr vbuf =
					HardwareBufferManager::getSingleton().createVertexBuffer(
					decl->getVertexSize(POSITION_BINDING), mRenderOp.vertexData->vertexCount,
					// попробовать с динамическим буфером
//					HardwareBuffer::HBU_STATIC_WRITE_ONLY // mostly static except during resizing
					HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY // mostly static except during resizing
					);
				// Bind buffer
				mRenderOp.vertexData->vertexBufferBinding->setBinding(POSITION_BINDING, vbuf);

				// No indexes & issue as a strip
				mRenderOp.useIndexes = false;
				mRenderOp.operationType = RenderOperation::OT_TRIANGLE_STRIP;

				mInitialised = true;
			}
		}

		void updatePositionGeometry(void)
		{
			/*
				0-----2
				|    /|
				|  /  |
				|/    |
				1-----3
			*/
			Real left, right, top, bottom;

			/* Convert positions into -1, 1 coordinate space (homogenous clip space).
				- Left / right is simple range conversion
				- Top / bottom also need inverting since y is upside down - this means
				that top will end up greater than bottom and when computing texture
				coordinates, we have to flip the v-axis (ie. subtract the value from
				1.0 to get the actual correct value).
			*/
			left = _getDerivedLeft() * 2 - 1;
			right = left + (mWidth * 2);
			top = -((_getDerivedTop() * 2) - 1);
			bottom =  top -  (mHeight * 2);

			HardwareVertexBufferSharedPtr vbuf =
				mRenderOp.vertexData->vertexBufferBinding->getBuffer(POSITION_BINDING);
			float* pPos = static_cast<float*>(
				vbuf->lock(HardwareBuffer::HBL_DISCARD) );

			// Use the furthest away depth value, since materials should have depth-check off
			// This initialised the depth buffer for any 3D objects in front

			Real zValue = Root::getSingleton().getRenderSystem()->getMaximumDepthInputValue();
			*pPos++ = left;
			*pPos++ = top;
			*pPos++ = zValue;
			*pPos++ = *((float*)(&m_color));

			*pPos++ = left;
			*pPos++ = bottom;
			*pPos++ = zValue;
			*pPos++ = *((float*)(&m_color));

			*pPos++ = right;
			*pPos++ = top;
			*pPos++ = zValue;
			*pPos++ = *((float*)(&m_color));

			*pPos++ = right;
			*pPos++ = bottom;
			*pPos++ = zValue;
			*pPos++ = *((float*)(&m_color));

			vbuf->unlock();
		}

		virtual void updateTextureGeometry()
		{
			// Generate for as many texture layers as there are in material
			if (!mpMaterial.isNull() && mInitialised) {
				// Assume one technique and pass for the moment
				size_t numLayers = mpMaterial->getTechnique(0)->getPass(0)->getNumTextureUnitStates();

				VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
				// Check the number of texcoords we have in our buffer now
				if (mNumTexCoordsInBuffer > numLayers) {
					// remove extras
					for (size_t i = mNumTexCoordsInBuffer; i > numLayers; --i) {
						decl->removeElement(VES_TEXTURE_COORDINATES, static_cast<unsigned short>(i));
					}
				} else if (mNumTexCoordsInBuffer < numLayers) {
					// Add extra texcoord elements
					size_t offset = VertexElement::getTypeSize(VET_FLOAT2) * mNumTexCoordsInBuffer;
					for (size_t i = mNumTexCoordsInBuffer; i < numLayers; ++i) {
						decl->addElement(TEXCOORD_BINDING, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES, static_cast<unsigned short>(i));
						offset += VertexElement::getTypeSize(VET_FLOAT2);
					}
				}

				// if number of layers changed at all, we'll need to reallocate buffer
				if (mNumTexCoordsInBuffer != numLayers) {
					// NB reference counting will take care of the old one if it exists
					HardwareVertexBufferSharedPtr newbuf =
						HardwareBufferManager::getSingleton().createVertexBuffer(
						decl->getVertexSize(TEXCOORD_BINDING), mRenderOp.vertexData->vertexCount,
						HardwareBuffer::HBU_STATIC_WRITE_ONLY // mostly static except during resizing
						);
					// Bind buffer, note this will unbind the old one and destroy the buffer it had
					mRenderOp.vertexData->vertexBufferBinding->setBinding(TEXCOORD_BINDING, newbuf);
					// Set num tex coords in use now
					mNumTexCoordsInBuffer = numLayers;
				}

				// Get the tcoord buffer & lock
				if (mNumTexCoordsInBuffer) {
					HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(TEXCOORD_BINDING);
					float* pVBStart = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD) );

					size_t uvSize = (VertexElement::getTypeSize(VET_FLOAT2) + VertexElement::getTypeSize(VET_COLOUR)) / sizeof(float);
					size_t vertexSize = decl->getVertexSize(TEXCOORD_BINDING) / sizeof(float);

					for (ushort i = 0; i < numLayers; ++i) {
						// Calc upper tex coords
						Real upperX = mU2 * mTileX[i];
						Real upperY = mV2 * mTileY[i];


						/*
							0-----2
							|    /|
							|  /  |
							|/    |
							1-----3
						*/
						// Find start offset for this set
						float* pTex = pVBStart + (i * uvSize);

						pTex[0] = mU1;
						pTex[1] = mV1;

						pTex += vertexSize; // jump by 1 vertex stride
						pTex[0] = mU1;
						pTex[1] = upperY;

						pTex += vertexSize;
						pTex[0] = upperX;
						pTex[1] = mV1;

						pTex += vertexSize;
						pTex[0] = upperX;
						pTex[1] = upperY;

					}
					vbuf->unlock();
				}
			}
		}

	}; // class PanelAlphaOverlayElement : public PanelOverlayElement

} // namespace MyGUI

#endif

