/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_Prerequest.h"
#include "MyGUI_TextSimpleOverlayElement.h"

#include <OgreRoot.h>
#include <OgreOverlayManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwareVertexBuffer.h>

namespace MyGUI
{

    Ogre::String TextSimpleOverlayElement::msTypeName = "TextSimple";

	TextSimpleOverlayElement::TextSimpleOverlayElement(const Ogre::String& name) :
		TextEditOverlayElement(name)
    {
    }

    void TextSimpleOverlayElement::initialise(void)
    {
		if (!mInitialised)
		{
			// Set up the render op
			// Combine positions and texture coords since they tend to change together
			// since character sizes are different
			mRenderOp.vertexData = new Ogre::VertexData();
			Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
			size_t offset = 0;
			// Positions
			decl->addElement(OVERLAY_POSITION_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
			// Texcoords
			decl->addElement(OVERLAY_POSITION_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

			decl->addElement(OVERLAY_POSITION_BINDING, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
			offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);

			mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
			mRenderOp.useIndexes = false;
			mRenderOp.vertexData->vertexStart = 0;
			// Vertex buffer will be created in checkMemoryAllocation

			checkMemoryAllocation( OVERLAY_DEFAULT_INITIAL_CHARS );

			mInitialised = true;
		}
    }

    void TextSimpleOverlayElement::updatePositionGeometry()
    {

		// ��� ������, �� ���������
		if (mpFont.isNull()) return;

		// ���� ���� ��������, ��� ���������� ��������� ������
		updateRawData();

		// ������� �����
		Ogre::RGBA colour = mDefaultColour;

		checkMemoryAllocation( mCaption.size() );
		mRenderOp.vertexData->vertexCount = 0;

		// Get position / texcoord buffer
		Ogre::HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(OVERLAY_POSITION_BINDING);
		float *pVert = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		// ��� ���������� ���������, ������� ����������� =)
		float realCharHeight = mCharHeight * 2.0;
		float realWidth = _getWidth() * 2.0;
		float realHeight = _getHeight() * 2.0;
		float realLeft = _getDerivedLeft() * 2.0;
		float realTop = _getDerivedTop() * 2.0;

		// ������� �������� ������
		float right, left = realLeft - 1.0;
		float bottom, top = 1.0 - realTop;

		// ���� ������� ������
		float left_margin = (mPixelScaleX * (float)mMargin.left * 2.0) + left;
		float top_margin = top - (mPixelScaleY * (float)mMargin.top * 2.0);
		float right_margin = (left + realWidth) - (mPixelScaleX * (float)mMargin.right * 2.0);
		float bottom_margin = (top - realHeight) + (mPixelScaleY * (float)mMargin.bottom * 2.0);

		// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
		float left_shift = 0;
		if ( IS_ALIGN_RIGHT(mAlign) ) left_shift = mContextSize.width - realWidth; // ������������ �� ������ �������
		else if ( IS_ALIGN_HCENTER(mAlign) ) left_shift = (mContextSize.width - realWidth) * 0.5; // ������������ �� ������
		right = left;

		if ( IS_ALIGN_BOTTOM(mAlign) ) top += (mContextSize.height - realHeight);
		else if ( IS_ALIGN_VCENTER(mAlign) ) top += (mContextSize.height - realHeight) * 0.5;
		bottom = top;

		// ������ ��������������� ��� ������
		float vertex_top, vertex_bottom, vertex_left, vertex_right;

		// �������� ����
		VectorLineInfo::iterator end = mLinesInfo.end();
		for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

			// �������� ������� ������
			top = bottom;
			bottom -= realCharHeight;
			// ����������� � ���������
			vertex_top = top;
			vertex_bottom = bottom;

			VectorCharInfo::iterator index = line->begin();
			VectorCharInfo::iterator end_index = line->end();
			// ������ ������ ������ ������
			float len = index->getValueFloat();
			++index;
			// ������ ����������� ��������
			++index;

			// ���� �� ������������� ���������� ����������
			bool texture_crop_height = false;

			if (vertex_top > top_margin) {
				// �������� �� ������ �����
				if (vertex_bottom > top_margin) {

					// ���������� ������� ���� ������ �������
					for (;index != end_index; ++index) {
						// ��������� �� ����� �����
						if ( index->isColour() ) {
							colour = index->getColour() | (colour & 0xFF000000);
						}
					}

					continue;
				}
				// ��������
				vertex_top = top_margin;
				texture_crop_height = true;
			}
			if (vertex_bottom < bottom_margin) {
				// ������ ���� ����
				if (vertex_top < bottom_margin) {
					line = end;
					line --;
					continue;
				}
				// ��������
				vertex_bottom = bottom_margin;
				texture_crop_height = true;
			}

			// �������� ������� ������
			right = (realLeft - 1.0) - left_shift; // ������������ �� ����� �������
			if ( IS_ALIGN_RIGHT(mAlign) ) right += (mContextSize.width - len); // ������������ �� ������ �������
			else if ( IS_ALIGN_HCENTER(mAlign) ) right += (mContextSize.width - len) * 0.5; // ������������ �� ������

			// ���������� ���� ������
			for (;index != end_index; ++index) {

				// ��������� �� ����� �����
				if ( index->isColour() ) {
					colour = index->getColour() | (colour & 0xFF000000);
					continue;
				}

				// ������������ ������
				Font::GlyphInfo * info = index->getGlyphInfo();
				Ogre::Real horiz_height = info->aspectRatio * mViewportAspectCoef * realCharHeight;

				// �������� ������� ������
				left = right;
				right += horiz_height;

				// ���� ������ ��� ��������� �� �� ������
				if ( (info->codePoint == Font::FONT_CODE_SPACE) || (info->codePoint == Font::FONT_CODE_TAB) ) continue;

				// ����������� � ���������
				vertex_left = left;
				vertex_right = right;

				// ������� ���������� ����������
				float texture_left = info->uvRect.left;
				float texture_right = info->uvRect.right;
				float texture_top = info->uvRect.top;
				float texture_bottom = info->uvRect.bottom;

				// ���� �� ������������� ���������� ����������
				bool texture_crop_width = false;

				if (vertex_left < left_margin) {
					// �������� �� ������ �����
					if (vertex_right < left_margin) continue;
					// ��������
					vertex_left = left_margin;
					texture_crop_width = true;
				}
				if (vertex_right > right_margin) {
					// ������ ������ �� ����� �� ����
					if (vertex_left > right_margin) {
						index ++;
						// ��� ���� ����� ���� ����� �� ��������, ����� ������ �� ����� ������
						while (index != end_index) {
							// ��������� �� ����� �����
							if ( index->isColour() ) {
								colour = index->getColour() | (colour & 0xFF000000);
							}
							index ++;
						};
						index --; // ����� ��� ���������� ��������� �� end
						continue;
					}
					// ��������
					vertex_right = right_margin;
					texture_crop_width = true;
				}

				// �������� �������� �� ���������
				if (texture_crop_height) {
					// ���������� ������ �������� � ���������� �����������
					texture_top += (top - vertex_top) * mTextureHeightOne;
					// �������� ������ �������� � ���������� �����������
					texture_bottom -= (vertex_bottom - bottom) * mTextureHeightOne;
				}

				// �������� �������� �� �����������
				if (texture_crop_width) {
					// ���������� ������ �������� � ���������� �����������
					texture_left += (vertex_left - left) * mTextureWidthOne;
					// �������� ������ �������� � ���������� �����������
					texture_right -= (right - vertex_right) * mTextureWidthOne;
				}

				// each vert is (x, y, z, u, v)
				//-------------------------------------------------------------------------------------
				// First tri
				//
				// Upper left
				*pVert++ = vertex_left;
				*pVert++ = vertex_top;
				*pVert++ = -1.0;
				*pVert++ = texture_left;
				*pVert++ = texture_top;
				*((Ogre::RGBA *)(pVert++)) = colour;

				// Bottom left
				*pVert++ = vertex_left;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_left;
				*pVert++ = texture_bottom;
				*((Ogre::RGBA *)(pVert++)) = colour;

				// Top right
				*pVert++ = vertex_right;
				*pVert++ = vertex_top;
				*pVert++ = -1.0;
				*pVert++ = texture_right;
				*pVert++ = texture_top;
				*((Ogre::RGBA *)(pVert++)) = colour;
				//-------------------------------------------------------------------------------------

				//-------------------------------------------------------------------------------------
				// Second tri
				//
				// Top right (again)
				*pVert++ = vertex_right;
				*pVert++ = vertex_top;
				*pVert++ = -1.0;
				*pVert++ = texture_right;
				*pVert++ = texture_top;
				*((Ogre::RGBA *)(pVert++)) = colour;

				// Bottom left (again)
				*pVert++ = vertex_left;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_left;
				*pVert++ = texture_bottom;
				*((Ogre::RGBA *)(pVert++)) = colour;

				// Bottom right
				*pVert++ = vertex_right;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_right;
				*pVert++ = texture_bottom;
				*((Ogre::RGBA *)(pVert++)) = colour;
				//-------------------------------------------------------------------------------------

				mRenderOp.vertexData->vertexCount += 6;

			}
		}

		// Unlock vertex buffer
		vbuf->unlock();
	
    }

	const Ogre::String& TextSimpleOverlayElement::getTypeName(void) const
	{
		return msTypeName;
	}

} // namespace MyGUI
