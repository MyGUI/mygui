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

	class _MyGUIExport TextSimpleOverlayElement : public Ogre::TextAreaOverlayElement
	{
	protected:
		enum {
			OVERLAY_POSITION_BINDING = 0,
			OVERLAY_DEFAULT_INITIAL_CHARS = 12
		};

		// ��������������� ����� ��� �������� ���������� � �������
		class EnumCharInfo
		{
		public:
			explicit EnumCharInfo() : mData(0) {}
			explicit EnumCharInfo(size_t _value, bool _colour = false) : mData(_colour ? (_value | 0xFF000000) : _value) {}
			explicit EnumCharInfo(float _value) : mData(*((size_t*)(&_value))) {}
			explicit EnumCharInfo(Font::GlyphInfo * _info) : mData((size_t)_info) {}

			inline size_t getValueSizeT() { return mData; }
			inline float getValueFloat() { return *((float*)(&mData)); }
			inline Ogre::RGBA getColour() { return (Ogre::RGBA) (mData & 0x00FFFFFF); }
			inline Font::GlyphInfo * getGlyphInfo() {return (Font::GlyphInfo *)mData;}
			inline bool isColour() { return (mData & 0xFF000000) == 0xFF000000; }

		private:
			size_t mData;
		};

		typedef std::vector<EnumCharInfo> VectorCharInfo;
		typedef std::vector<VectorCharInfo> VectorLineInfo;

		IntRect mMargin; // ����������
		Align mAlign;

		bool mRenderGL;// ��� ��������������� ����� ������
		Ogre::RGBA mDefaultColour; // ���� ������
		FloatSize mContextSize; // ������ ����� ������
		VectorLineInfo mLinesInfo;
		bool mRawDataOutOfDate;
		float mOldViewportAspectCoef; // ��������� �����������
		float mTextureHeightOne, mTextureWidthOne; // ������ ����� ���������� �������
        FontPtr mpFont;
		Font::GlyphInfo * mSpaceGlyphInfo;
		Font::GlyphInfo * mTabGlyphInfo;


	public:
		TextSimpleOverlayElement(const Ogre::String& name) :
			TextAreaOverlayElement(name),
			mAlign(ALIGN_CENTER),
			mDefaultColour(0xFFFFFFFF),
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
			if (IS_ALIGN_RIGHT(_align)) mAlignment = Right;
			else if (false == IS_ALIGN_LEFT(_align)) mAlignment = Center;
			else mAlignment = Left;
			mGeomPositionsOutOfDate = true;
		}

		// ������������ ������� �� ������� ������ �����
		inline void setMargin(const IntRect& _margin)
		{
			mMargin = _margin;
			mDerivedOutOfDate = true;
		}

		virtual void updatePositionGeometry()
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
			if ( mAlignment == Right ) left_shift = mContextSize.width - realWidth; // ������������ �� ������ �������
			else if ( mAlignment == Center ) left_shift = (mContextSize.width - realWidth) * 0.5; // ������������ �� ������
			right = left;

			if ( mAlign & ALIGN_BOTTOM ) top += (mContextSize.height - realHeight);
			else if ( !(IS_ALIGN_TOP(mAlign)) ) top += (mContextSize.height - realHeight) * 0.5;
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
				if ( mAlignment == Right ) right += (mContextSize.width - len); // ������������ �� ������ �������
				else if ( mAlignment == Center ) right += (mContextSize.width - len) * 0.5; // ������������ �� ������

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
							decl->getVertexSize(OVERLAY_POSITION_BINDING), 
							mRenderOp.vertexData->vertexCount,
							Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
				bind->setBinding(OVERLAY_POSITION_BINDING, vbuf);

				mAllocSize = numChars;
			}

		}

		void setColour(const Ogre::ColourValue & _colour)
		{
			Ogre::Root::getSingleton().convertColourValue(_colour, &mDefaultColour);
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
				MYGUI_EXCEPT("Could not find font " + font);
			mpFont->load();
			mpMaterial = mpFont->getMaterial();
			mpMaterial->setDepthCheckEnabled(false);
			mpMaterial->setLightingEnabled(false);

			// ������� ������ � ���������
			mSpaceGlyphInfo = mpFont->getSpaceGlyphInfo();
			mTabGlyphInfo = mpFont->getTabGlyphInfo();
			
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
			static const char convert_colour[128] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

			// ���������� ������� ����� ������� � ���������� �����������
			float realCharHeight = mCharHeight * 2.0;
			Font::GlyphInfo * info = mpFont->getGlyphInfo('A');
			mTextureHeightOne = (info->uvRect.bottom - info->uvRect.top) / (realCharHeight);
			mTextureWidthOne = (info->uvRect.right - info->uvRect.left) / (info->aspectRatio * mViewportAspectCoef * realCharHeight);

			mLinesInfo.clear();

			// ������� ������ �������
			mLinesInfo.push_back(VectorCharInfo());
			mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������ ������ ������ � �������� �����������
			mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������, ����������� �������� ��������
			float len = 0, width = 0;
			size_t count = 1;

			Ogre::DisplayString::const_iterator end = mCaption.end();
			for (Ogre::DisplayString::const_iterator index=mCaption.begin(); index!=end; ++index) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);

				if (character == Font::FONT_CODE_CR || character == Font::FONT_CODE_NEL || character == Font::FONT_CODE_LF) {
					// ���������� ������ ���������� ������
					mLinesInfo.back()[0] = EnumCharInfo(len);
					mLinesInfo.back()[1] = EnumCharInfo(count);
					if (width < len) width = len;
					count = 1;
					len = 0;

					// � ������� �����
					mLinesInfo.push_back(VectorCharInfo());
					mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������ ������ ������ � ��������
					mLinesInfo.back().push_back(EnumCharInfo()); // ������ ������, ����������� �������� ��������

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
						Ogre::RGBA colour = convert_colour[character & 0x7F];

						// � ��� ���� �������� ����� �����
						for (char i=0; i<5; i++) {
							++ index;
							if (index == end) {--index ;continue;} // ��� ������
							colour <<= 4;
							colour += convert_colour[ OGRE_DEREF_DISPLAYSTRING_ITERATOR(index) & 0x7F];
						}

						// ���� �����, �� ������ ������� � ����� ����������
						if (mRenderGL) colour = ((colour&0x00FF0000)>>16)|((colour&0x000000FF)<<16)|(colour&0xFF00FF00);
				
						// ���������� ����, � ������� ����� �������
						mLinesInfo.back().push_back( EnumCharInfo(colour, true) );

						continue;
					}
				}

				Font::GlyphInfo * info;
				if (Font::FONT_CODE_SPACE == character) info = mpFont->getSpaceGlyphInfo();
				else if (Font::FONT_CODE_TAB == character) info = mpFont->getTabGlyphInfo();
				else info = mpFont->getGlyphInfo(character);

				len += info->aspectRatio * realCharHeight * mViewportAspectCoef;

				// ��������� �� ���� � �������
				mLinesInfo.back().push_back( EnumCharInfo(info) );
				count ++;

			}

			// ���������� ������ ���������� ������
			mLinesInfo.back()[0] = EnumCharInfo(len);
			mLinesInfo.back()[1] = EnumCharInfo(count);
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