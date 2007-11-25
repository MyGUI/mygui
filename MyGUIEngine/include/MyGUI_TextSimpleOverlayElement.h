/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TEXT_SIMPLE_OVERLAY_ELEMENT_H__
#define __MYGUI_TEXT_SIMPLE_OVERLAY_ELEMENT_H__

#include "MyGUI_Prerequest.h"
#include <OgreTextAreaOverlayElement.h>
#include "MyGUI_Font.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_AlignInfo.h"

namespace MyGUI
{

	#define DEFAULT_INITIAL_CHARS 12
	#define MAIN_BUFFER_BINDING 0

	class _MyGUIExport TextSimpleOverlayElement : public Ogre::TextAreaOverlayElement
	{

	protected:
		typedef std::vector<size_t> VectorCharInfo;
		typedef std::vector<VectorCharInfo> VectorLineInfo;

		int mLeftMargin, mRightMargin, mTopMargin, mBottomMargin; // ����������
		Align mAlign;

		bool mRenderGL;// ��� ��������������� ����� ������
		Ogre::RGBA mDefaultColor; // ���� ������
		FloatSize mContextSize; // ������ ����� ������
		VectorLineInfo mLinesInfo;
		bool mRawDataOutOfDate;
		float mOldViewportAspectCoef; // ��������� �����������
		float mTextureHeightOne, mTextureWidthOne; // ������ ����� ���������� �������
        FontPtr mpFont;


	public:
		TextSimpleOverlayElement(const Ogre::String& name) :
			TextAreaOverlayElement(name),
			mLeftMargin (0),
			mRightMargin (0),
			mTopMargin (0),
			mBottomMargin (0),
			mAlign(ALIGN_CENTER),
			mDefaultColor(0xFFFFFFFF),
			mRawDataOutOfDate(false),
			mOldViewportAspectCoef(1.0f)
		{
			// ��� ��������������� ����� ������
			mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());
		}

		// ����������� �����������, � �� �� ��� ������� �����������
		void updateColours(void) { }

		// ���������� �������� ��� ��� �������� � ��������
		inline void setAlignment(Align _align)	
		{
			// ������������ �����
			mAlign = _align;
			// ������������ ����� ������ �����
			if (_align & ALIGN_RIGHT) mAlignment = Right;
			else if (! (_align & ALIGN_LEFT)) mAlignment = Center;
			else mAlignment = Left;
			mGeomPositionsOutOfDate = true;
		}

		// ������������ ������� �� ������� ������ �����
		inline void setMargin(int _left, int _top, int _right, int _bottom)
		{
			mLeftMargin = _left;
			mTopMargin = _top;
			mRightMargin = _right;
			mBottomMargin = _bottom;
			mDerivedOutOfDate = true;
		}

		virtual void updatePositionGeometry()
		{
			// ��� ������, �� ���������
			if (mpFont.isNull()) return;

			// ���� ���� ��������, ��� ���������� ��������� ������
			updateRawData();

			// ������� �����
			Ogre::RGBA color = mDefaultColor;

			checkMemoryAllocation( mCaption.size() );
			mRenderOp.vertexData->vertexCount = 0;

			// Get position / texcoord buffer
			Ogre::HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(MAIN_BUFFER_BINDING);
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
			float left_margin = (mPixelScaleX * (float)mLeftMargin * 2.0) + left;
			float top_margin = top - (mPixelScaleY * (float)mTopMargin * 2.0);
			float right_margin = (left + realWidth) - (mPixelScaleX * (float)mRightMargin * 2.0);
			float bottom_margin = (top - realHeight) + (mPixelScaleY * (float)mBottomMargin * 2.0);

			// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
			float left_shift = 0;
			if ( mAlignment == Right ) left_shift = mContextSize.width - realWidth; // ������������ �� ������ �������
			else if ( mAlignment == Center ) left_shift = (mContextSize.width - realWidth) * 0.5; // ������������ �� ������
			right = left;

			if ( mAlign & ALIGN_BOTTOM ) top += (mContextSize.height - realHeight);
			else if ( !(mAlign & ALIGN_TOP) ) top += (mContextSize.height - realHeight) * 0.5;
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
				float len = *((float*)(&(*index)));
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
							size_t data = (*index);
							// ��������� �� ����� �����
							if ( (data & 0xFF000000) == 0xFF000000) {
								color = (Ogre::RGBA) (data & 0x00FFFFFF) | (color & 0xFF000000);
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
				if ( mAlignment == Right ) right += (mContextSize.width - len); // ������������ �� ������ �������
				else if ( mAlignment == Center ) right += (mContextSize.width - len) * 0.5; // ������������ �� ������

				// ���������� ���� ������
				for (;index != end_index; ++index) {

					size_t data = (*index);
					// ��������� �� ����� �����
					if ( (data & 0xFF000000) == 0xFF000000) {
						color = (Ogre::RGBA) (data & 0x00FFFFFF) | (color & 0xFF000000);
						continue;
					}

					// ������������ ������
					Font::GlyphInfo * info = (Font::GlyphInfo * )data;
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
								size_t data = (*index);
								// ��������� �� ����� �����
								if ( (data & 0xFF000000) == 0xFF000000) {
									color = (Ogre::RGBA) (data & 0x00FFFFFF) | (color & 0xFF000000);
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
					*((Ogre::RGBA *)(pVert++)) = color;

					// Bottom left
					*pVert++ = vertex_left;
					*pVert++ = vertex_bottom;
					*pVert++ = -1.0;
					*pVert++ = texture_left;
					*pVert++ = texture_bottom;
					*((Ogre::RGBA *)(pVert++)) = color;

					// Top right
					*pVert++ = vertex_right;
					*pVert++ = vertex_top;
					*pVert++ = -1.0;
					*pVert++ = texture_right;
					*pVert++ = texture_top;
					*((Ogre::RGBA *)(pVert++)) = color;
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
					*((Ogre::RGBA *)(pVert++)) = color;

					// Bottom left (again)
					*pVert++ = vertex_left;
					*pVert++ = vertex_bottom;
					*pVert++ = -1.0;
					*pVert++ = texture_left;
					*pVert++ = texture_bottom;
					*((Ogre::RGBA *)(pVert++)) = color;

					// Bottom right
					*pVert++ = vertex_right;
					*pVert++ = vertex_bottom;
					*pVert++ = -1.0;
					*pVert++ = texture_right;
					*pVert++ = texture_bottom;
					*((Ogre::RGBA *)(pVert++)) = color;
					//-------------------------------------------------------------------------------------

					mRenderOp.vertexData->vertexCount += 6;
					//cur ++;

				}

				// ��������� ������
				//cursor += count;
			}

			// Unlock vertex buffer
			vbuf->unlock();

		}

		void initialise(void)
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
				decl->addElement(MAIN_BUFFER_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
				offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
				// Texcoords
				decl->addElement(MAIN_BUFFER_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
				offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

				decl->addElement(MAIN_BUFFER_BINDING, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
				offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);

				mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
				mRenderOp.useIndexes = false;
				mRenderOp.vertexData->vertexStart = 0;
				// Vertex buffer will be created in checkMemoryAllocation

				checkMemoryAllocation( DEFAULT_INITIAL_CHARS );

				mInitialised = true;
			}

		}

		void checkMemoryAllocation( size_t numChars )
		{
			if( mAllocSize < numChars)
			{
				// Create and bind new buffers
				// Note that old buffers will be deleted automatically through reference counting
	            
				// 6 verts per char since we're doing tri lists without indexes
				// Allocate space for positions & texture coords
				Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
				Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

				mRenderOp.vertexData->vertexCount = numChars * 6;

				// Create dynamic since text tends to change alot
				// positions & texcoords
				Ogre::HardwareVertexBufferSharedPtr vbuf = 
					Ogre::HardwareBufferManager::getSingleton().
						createVertexBuffer(
							decl->getVertexSize(MAIN_BUFFER_BINDING), 
							mRenderOp.vertexData->vertexCount,
							Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
				bind->setBinding(MAIN_BUFFER_BINDING, vbuf);

				mAllocSize = numChars;
			}

		}

		void setColour(const Ogre::ColourValue & _colour)
		{
			Ogre::Root::getSingleton().convertColourValue(_colour, &mDefaultColor);
			mGeomPositionsOutOfDate = true;
		}

		void setCaption(const Ogre::DisplayString& text)
		{
			TextAreaOverlayElement::setCaption(text);
			mRawDataOutOfDate = true;
		}

		void setFontName( const Ogre::String& font )
		{
			mpFont = FontManager::getInstance().getByName( font );
			if (mpFont.isNull())
				MYGUI_EXCEPT("Could not find font " + font, "TextAreaOverlayElement::setFontName" );
			mpFont->load();
			mpMaterial = mpFont->getMaterial();
			mpMaterial->setDepthCheckEnabled(false);
			mpMaterial->setLightingEnabled(false);
			
			mGeomPositionsOutOfDate = true;
			mGeomUVsOutOfDate = true;
			mRawDataOutOfDate = true;
		}

		void setCharHeight( Ogre::Real height )
		{
			TextAreaOverlayElement::setCharHeight( height );
			mRawDataOutOfDate = true;
		}

		void updateRawData()
		{
			if (mpFont.isNull()) return;
			if ( !mRawDataOutOfDate && (mOldViewportAspectCoef == mViewportAspectCoef) ) return;

			// ������ ��� ������� ����������� ������
			static const char convert_color[128] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

			// ���������� ������� ����� ������� � ���������� �����������
			float realCharHeight = mCharHeight * 2.0;
			const Font::GlyphInfo & info = mpFont->getGlyphInfo('A');
			mTextureHeightOne = (info.uvRect.bottom - info.uvRect.top) / (realCharHeight);
			mTextureWidthOne = (info.uvRect.right - info.uvRect.left) / (info.aspectRatio * mViewportAspectCoef * realCharHeight);

			mLinesInfo.clear();

			// ������� ������ �������
			mLinesInfo.push_back(VectorCharInfo());
			mLinesInfo.back().push_back(0); // ������ ������ ������ ������ � �������� �����������
			mLinesInfo.back().push_back(0); // ������ ������, ����������� �������� ��������
			float len = 0, width = 0;
			size_t count = 1;

			Ogre::DisplayString::const_iterator end = mCaption.end();
			for (Ogre::DisplayString::const_iterator index=mCaption.begin(); index!=end; ++index) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);

				if (character == Font::FONT_CODE_CR || character == Font::FONT_CODE_NEL || character == Font::FONT_CODE_LF) {
					// ���������� ������ ���������� ������
					mLinesInfo.back()[0] = *((size_t*)(&len));
					mLinesInfo.back()[1] = count;
					if (width < len) width = len;
					count = 1;
					len = 0;

					// � ������� �����
					mLinesInfo.push_back(VectorCharInfo());
					mLinesInfo.back().push_back(0); // ������ ������ ������ ������ � ��������
					mLinesInfo.back().push_back(0); // ������ ������, ����������� �������� ��������

					if (character == Font::FONT_CODE_CR) {
						Ogre::DisplayString::const_iterator peeki = index;
						peeki++;
						if (peeki != end && OGRE_DEREF_DISPLAYSTRING_ITERATOR(peeki) == Font::FONT_CODE_LF) index = peeki; // skip both as one newline
					} 
					// ��������� ������
					continue;

				} else if (character == '#') {
					// ����� ��������� ������
					++ index;
					if (index == end) {--index ;continue;} // ��� ������

					character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);
					// ���� ��� ������, �� ������ ���� ����, ���� ��� �� ������ ����
					if (character != '#') {

						// ������ ������ ������
						size_t color = convert_color[character & 0x7F];

						// � ��� ���� �������� ����� �����
						for (char i=0; i<5; i++) {
							++ index;
							if (index == end) {--index ;continue;} // ��� ������
							color <<= 4;
							color += convert_color[ OGRE_DEREF_DISPLAYSTRING_ITERATOR(index) & 0x7F];
						}

						// ���� �����, �� ������ ������� � ����� ����������
						if (mRenderGL) color = ((color&0x00FF0000)>>16)|((color&0x000000FF)<<16)|(color&0xFF00FF00);
				
						// ���������� ����, � ������� ����� �������
						mLinesInfo.back().push_back( color | 0xFF000000 );

						continue;
					}
				}

				const Font::GlyphInfo & info = mpFont->getGlyphInfo(character);
				len += info.aspectRatio * realCharHeight * mViewportAspectCoef;

				// ��������� �� ���� � �������
				mLinesInfo.back().push_back( (size_t) &(const_cast<Font::GlyphInfo &>(info)) );
				count ++;

			}

			// ���������� ������ ���������� ������
			mLinesInfo.back()[0] = *((size_t*)(&len));
			mLinesInfo.back()[1] = count;
			if (width < len) width = len;


			// ��������� �����
			mRawDataOutOfDate = false;
			mOldViewportAspectCoef = mViewportAspectCoef;

			// ������������� ������ ������
			mContextSize.set(width, (float)mLinesInfo.size() * mCharHeight * 2.0);

		} // void updateRawData()

	}; // class TextSimpleOverlayElement : public TextAreaOverlayElement

} // namespace MyGUI

#endif // __MYGUI_TEXT_SIMPLE_OVERLAY_ELEMENT_H__