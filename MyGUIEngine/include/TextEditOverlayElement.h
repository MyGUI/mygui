#ifndef __TEXT_EDIT_OVERLAY_ELEMENT_H__
#define __TEXT_EDIT_OVERLAY_ELEMENT_H__

#include "Prerequest.h"
#include <OgreTextAreaOverlayElement.h>
#include "MyGUI_Font.h"
#include "MyGUI_FontManager.h"
#include "AlignInfo.h"

namespace MyGUI
{

	#define DEFAULT_INITIAL_CHARS 12
	#define MAIN_BUFFER_BINDING 0

	class _MyGUIExport TextEditOverlayElement : public Ogre::TextAreaOverlayElement
	{

	protected:
		typedef std::vector<size_t> VectorCharInfo;
		typedef std::vector<VectorCharInfo> VectorLineInfo;

		int m_left_margin, m_right_margin, m_top_margin, m_bottom_margin; // ����������
		char m_align;

		bool mRenderGL;// ��� ��������������� ����� ������
		Ogre::RGBA mDefaultColor; // ���� ������
		Ogre::RGBA mInverseColor; // ��������� ���� ������
		size_t mStartSelect, mEndSelect; // ������ � ����� ���������
		IntPoint mPointShift; // �������� ������
		FloatSize mContextSize; // ������ ����� ������
		VectorLineInfo mLinesInfo;
		bool mRawDataOutOfDate;
		float mOldViewportAspectCoef; // ��������� �����������
		float mTextureHeightOne, mTextureWidthOne; // ������ ����� ���������� �������
        FontPtr mpFont;
		FloatPoint mBackgroundEmpty, mBackgroundFill;


	public:
		TextEditOverlayElement(const String& name) :
			TextAreaOverlayElement(name),
			m_left_margin (0),
			m_right_margin (0),
			m_top_margin (0),
			m_bottom_margin (0),
			m_align(ALIGN_CENTER),
			mDefaultColor(0xFFFFFFFF),
			mInverseColor(0xFF000000),
			mStartSelect(0), mEndSelect(0),
			mRawDataOutOfDate(false),
			mOldViewportAspectCoef(1.0f)
		{
			// ��� ��������������� ����� ������
			mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());
		}

		inline void setSelect(size_t _start, size_t _end) {mStartSelect=_start;mEndSelect=_end;mGeomPositionsOutOfDate = true;}
		inline size_t getSelectStart() {return mStartSelect;}
		inline size_t getSelectEnd() {return mEndSelect;}

		// ����������� �����������, � �� �� ��� ������� �����������
		void updateColours(void) { }

		// ���������� �������� ��� ��� �������� � ��������
		inline void setAlignment(char _align)	
		{
			// ������������ �����
			m_align = _align;
			// ������������ ����� ������ �����
			if (_align & ALIGN_RIGHT) mAlignment = Right;
			else if (! (_align & ALIGN_LEFT)) mAlignment = Center;
			else mAlignment = Left;
			mGeomPositionsOutOfDate = true;
		}

		// ������������ ������� �� ������� ������ �����
		inline void setMargin(int _left, int _top, int _right, int _bottom)
		{
			m_left_margin = _left;
			m_top_margin = _top;
			m_right_margin = _right;
			m_bottom_margin = _bottom;
			mDerivedOutOfDate = true;
		}

		virtual void updatePositionGeometry()
		{
			// ��� ������, �� ���������
			if (mpFont.isNull()) return;

			// ���� ���� ��������, ��� ���������� ��������� ������
			updateRawData();

			// ������� ������������� �������
			size_t cursor = 0;

			// ������� �����
			Ogre::RGBA color_current, color = mDefaultColor;
			Ogre::RGBA color_inverse = mInverseColor;

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
			float left_margin = (mPixelScaleX * (float)m_left_margin * 2.0) + left;
			float top_margin = top - (mPixelScaleY * (float)m_top_margin * 2.0);
			float right_margin = (left + realWidth) - (mPixelScaleX * (float)m_right_margin * 2.0);
			float bottom_margin = (top - realHeight) + (mPixelScaleY * (float)m_bottom_margin * 2.0);

			// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
			float left_shift = 0;
			if (mContextSize.width <= realWidth) {
				if ( mAlignment == Right ) left_shift = mContextSize.width - realWidth; // ������������ �� ������ �������
				else if ( mAlignment == Center ) left_shift = (mContextSize.width - realWidth) * 0.5; // ������������ �� ������
			}
			else left_shift = mPixelScaleX * (float)mPointShift.left * 2.0;
			right = left;

			// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
			if (mContextSize.height <= realHeight) {
				if ( m_align & ALIGN_BOTTOM ) top += (mContextSize.height - realHeight);
				else if ( !(m_align & ALIGN_TOP) ) top += (mContextSize.height - realHeight) * 0.5;
			}
			else top += mPixelScaleY * (float)mPointShift.top * 2.0;
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
				size_t count = (*index);
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
								color_inverse = color ^ 0x00FFFFFF;
							}
						}

						cursor += count;
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
						cursor += count;
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

				// ������� ��������� � ������
				size_t cur = cursor;

				// ���������� ���� ������
				for (;index != end_index; ++index) {

					size_t data = (*index);
					// ��������� �� ����� �����
					if ( (data & 0xFF000000) == 0xFF000000) {
						color = (Ogre::RGBA) (data & 0x00FFFFFF) | (color & 0xFF000000);
						color_inverse = color ^ 0x00FFFFFF;
						continue;
					}

					// ������������ ������
					Font::GlyphInfo * info = (Font::GlyphInfo * )data;
					Ogre::Real horiz_height = info->aspectRatio * mViewportAspectCoef * realCharHeight;

					// �������� ������� ������
					left = right;
					right += horiz_height;

					// �������� �������� ��� ����
					FloatPoint background_current;

					// ������ �� �������
					if ( (cur >= mEndSelect) || (cur < mStartSelect) ) {

						// ���� ������ ��� ��������� �� ������ ������ ��� ���������
						if ( (info->codePoint == Font::FONT_CODE_SPACE) || (info->codePoint == Font::FONT_CODE_TAB) ) {
							cur ++;
							continue;
						}

						color_current = color;
						background_current = mBackgroundEmpty;
					}
					// ������ �������
					else {
						// ��������� �����
						color_current = color_inverse;
						background_current = mBackgroundFill;
					}

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
						if (vertex_right < left_margin) {
							cur ++;
							continue;
						}
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
									color_inverse = color ^ 0x00FFFFFF;
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
					*pVert++ = background_current.left;
					*pVert++ = background_current.top;
					*((Ogre::RGBA *)(pVert++)) = color_current;

					// Bottom left
					*pVert++ = vertex_left;
					*pVert++ = vertex_bottom;
					*pVert++ = -1.0;
					*pVert++ = texture_left;
					*pVert++ = texture_bottom;
					*pVert++ = background_current.left;
					*pVert++ = background_current.top;
					*((Ogre::RGBA *)(pVert++)) = color_current;

					// Top right
					*pVert++ = vertex_right;
					*pVert++ = vertex_top;
					*pVert++ = -1.0;
					*pVert++ = texture_right;
					*pVert++ = texture_top;
					*pVert++ = background_current.left;
					*pVert++ = background_current.top;
					*((Ogre::RGBA *)(pVert++)) = color_current;
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
					*pVert++ = background_current.left;
					*pVert++ = background_current.top;
					*((Ogre::RGBA *)(pVert++)) = color_current;

					// Bottom left (again)
					*pVert++ = vertex_left;
					*pVert++ = vertex_bottom;
					*pVert++ = -1.0;
					*pVert++ = texture_left;
					*pVert++ = texture_bottom;
					*pVert++ = background_current.left;
					*pVert++ = background_current.top;
					*((Ogre::RGBA *)(pVert++)) = color_current;

					// Bottom right
					*pVert++ = vertex_right;
					*pVert++ = vertex_bottom;
					*pVert++ = -1.0;
					*pVert++ = texture_right;
					*pVert++ = texture_bottom;
					*pVert++ = background_current.left;
					*pVert++ = background_current.top;
					*((Ogre::RGBA *)(pVert++)) = color_current;
					//-------------------------------------------------------------------------------------

					mRenderOp.vertexData->vertexCount += 6;
					cur ++;

				}

				// ��������� ������
				cursor += count;
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

				decl->addElement(MAIN_BUFFER_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 1);
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
				// ����������� ��� �� �������
				numChars += DEFAULT_INITIAL_CHARS;

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

		void setColour(const Ogre::ColourValue & _color)
		{
			Ogre::Root::getSingleton().convertColourValue(_color, &mDefaultColor);

			// ������������� ����
			mInverseColor = mDefaultColor ^ 0x00FFFFFF;

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
				OGRE_EXCEPT( Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find font " + font,
					"TextAreaOverlayElement::setFontName" );
			mpFont->load();
			mpMaterial = mpFont->getMaterialSelectedFont();
			mpMaterial->setDepthCheckEnabled(false);
			mpMaterial->setLightingEnabled(false);

			// ������� ������� ����� �� �������� ��� ��������� ������
			Font::GlyphInfo info = mpFont->getGlyphInfo(Font::FONT_CODE_SPACE);
			mBackgroundEmpty.set(info.uvRect.left + ((info.uvRect.right-info.uvRect.left)*0.5), info.uvRect.top + ((info.uvRect.bottom-info.uvRect.top)*0.5));
			info = mpFont->getGlyphInfo(Font::FONT_CODE_SELECT);
			mBackgroundFill.set(info.uvRect.left + ((info.uvRect.right-info.uvRect.left)*0.5), info.uvRect.top + ((info.uvRect.bottom-info.uvRect.top)*0.5));
			
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

		// ���������� ������ ������ � ��������
		inline IntSize getTextSize()
		{
			// ���� ���� ��������, ��� ���������� ��������� ������
			updateRawData();
			return IntSize( (int)(mContextSize.width / (mPixelScaleX * 2.0)), (int)(mLinesInfo.size() * mPixelCharHeight) );
		}

		// ������������� �������� ������ � ��������
		inline void setTextShift(IntPoint _point)
		{
			mPointShift = _point;
			mRawDataOutOfDate = true;
			mGeomPositionsOutOfDate = true;
			//mIsAutoOffsetContext = false;
		}

		inline IntPoint getTextShift() {return mPointShift;}

		size_t getTextCursorFromPoint(IntPoint & _point)
		{
			// ���� ���� ��������, ��� ���������� ��������� ������
			updateRawData();

			// ������� �������
			size_t position = 0;

			// ��� ���������� ���������, ������� ����������� =)
			float realCharHeight = mCharHeight * 2.0;
			float realWidth = _getWidth() * 2.0;
			float realHeight = _getHeight() * 2.0;
			float realLeft = _getDerivedLeft() * 2.0;
			float realTop = _getDerivedTop() * 2.0;

			// ���������� ����������
			float x = mPixelScaleX * (float)_point.left * 2.0;
			float y = ( 1.0f - (mPixelScaleY * (float)_point.top * 2.0) );

			// ������� �������� ������
			float right, left = realLeft - 1.0;
			float bottom, top = 1.0 - realTop;

			// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
			float left_shift = 0;
			if (mContextSize.width <= realWidth) {
				if ( mAlignment == Right ) left_shift = mContextSize.width - realWidth; // ������������ �� ������ �������
				else if ( mAlignment == Center ) left_shift = (mContextSize.width - realWidth) * 0.5; // ������������ �� ������
			}
			else left_shift = mPixelScaleX * (float)mPointShift.left * 2.0;
			right = left;

			// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
			if (mContextSize.height <= realHeight) {
				if ( m_align & ALIGN_BOTTOM ) top += (mContextSize.height - realHeight);
				else if ( !(m_align & ALIGN_TOP) ) top += (mContextSize.height - realHeight) * 0.5;
			}
			else top += mPixelScaleY * (float)mPointShift.top * 2.0;
			bottom = top;

			// ������������ ���������� �� ������ ������
			if (y < (bottom - mContextSize.height)) y = bottom - mContextSize.height;

			// �������� ����
			VectorLineInfo::iterator end = mLinesInfo.end();
			for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

				// �������� ������� ������
				top = bottom;
				bottom -= realCharHeight;

				VectorCharInfo::iterator index = line->begin();
				VectorCharInfo::iterator end_index = line->end();

				// ������ ������ ������ ������
				float len = *((float*)(&(*index)));
				++index;

				// ����������� ��������
				size_t count = (*index);
				++index;

				// ��������� �������
				if (y < bottom) {
					position += count;
					continue;
				}

				// �������� ������� ������
				right = (realLeft - 1.0) - left_shift; // ������������ �� ����� �������
				if ( mAlignment == Right ) right += (mContextSize.width - len); // ������������ �� ������ �������
				else if ( mAlignment == Center ) right += (mContextSize.width - len) * 0.5; // ������������ �� ������

				if (x <= (1.0 + right)) {
					// � ������ ������
					_point.left = (int)((1.0f + right) / (mPixelScaleX * 2.0));
					_point.top = (int)((1.0f - top) / (mPixelScaleY * 2.0));
					return position;

				} else if (x >= (1.0 + right + len)) {
					// � ����� ������
					_point.left = (int)((1.0f + right + len) / (mPixelScaleX * 2.0));
					_point.top = (int)((1.0f - top) / (mPixelScaleY * 2.0));
					position += count - 1;
					return position;
				}

				// ���������� ���� ������
				for (;index != end_index; ++index) {

					size_t data = (*index);
					// ��������� �� ����� �����
					if ( (data & 0xFF000000) == 0xFF000000) continue;

					// ������������ ������
					Font::GlyphInfo * info = (Font::GlyphInfo * )data;
					Ogre::Real horiz_height = info->aspectRatio * mViewportAspectCoef * realCharHeight;

					// �������� ������� ������
					left = right;
					right += horiz_height;

					// ������ � ������, ���������� � ��������� ������
					if (x < (1.0 + right)) {
						if (x < ((1.0 + right) - (horiz_height * 0.5)) ) {
							// � ����� �������� �������
							_point.left = (int)((1.0f + left) / (mPixelScaleX * 2.0));

						} else {
							// � ������ �������� �������
							position ++;
							_point.left = (int)((1.0f + right) / (mPixelScaleX * 2.0));
						}
						_point.top = (int)((1.0f - top) / (mPixelScaleY * 2.0));
						return position;
					}

					// ��������� ������
					position ++;

				}
			}

			return position;
		}

		IntPoint getTextCursorFromPosition(size_t _position)
		{
			// ���� ���� ��������, ��� ���������� ��������� ������
			updateRawData();

			// ������� �������
			size_t pos = _position;

			// ��� ���������� ���������, ������� ����������� =)
			float realCharHeight = mCharHeight * 2.0;
			float realWidth = _getWidth() * 2.0;
			float realHeight = _getHeight() * 2.0;
			float realLeft = _getDerivedLeft() * 2.0;
			float realTop = _getDerivedTop() * 2.0;

			// ������� �������� ������
			float right, left = realLeft - 1.0;
			float bottom, top = 1.0 - realTop;

			// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
			float left_shift = 0;
			if (mContextSize.width <= realWidth) {
				if ( mAlignment == Right ) left_shift = mContextSize.width - realWidth; // ������������ �� ������ �������
				else if ( mAlignment == Center ) left_shift = (mContextSize.width - realWidth) * 0.5; // ������������ �� ������
			}
			else left_shift = mPixelScaleX * (float)mPointShift.left * 2.0;
			right = left;

			// ����� ������, ���� ��� ������ ��� �������������� ������������ �� �������� �� ���������� ��������
			if (mContextSize.height <= realHeight) {
				if ( m_align & ALIGN_BOTTOM ) top += (mContextSize.height - realHeight);
				else if ( !(m_align & ALIGN_TOP) ) top += (mContextSize.height - realHeight) * 0.5;
			}
			else top += mPixelScaleY * (float)mPointShift.top * 2.0;
			bottom = top;

			// �������� ����
			VectorLineInfo::iterator end = mLinesInfo.end();
			for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

				// �������� ������� ������
				top = bottom;
				bottom -= realCharHeight;

				VectorCharInfo::iterator index = line->begin();
				VectorCharInfo::iterator end_index = line->end();

				// ������ ������ ������ ������
				float len = *((float*)(&(*index)));
				++index;

				// ����������� ��������
				size_t count = (*index);
				++index;

				// ��������� �������
				if (pos >= count) {
					pos -= count;
					continue;
				}

				// �������� ������� ������
				right = (realLeft - 1.0) - left_shift; // ������������ �� ����� �������
				if ( mAlignment == Right ) right += (mContextSize.width - len); // ������������ �� ������ �������
				else if ( mAlignment == Center ) right += (mContextSize.width - len) * 0.5; // ������������ �� ������

				// �����, ����������
				if (pos == 0) return IntPoint((int)((1.0f + right) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));

				int currect_pos = 0;

				// ���������� ���� ������
				for (;index != end_index; ++index) {

					size_t data = (*index);
					// ��������� �� ����� �����
					if ( (data & 0xFF000000) == 0xFF000000) continue;

					// ������������ ������
					Font::GlyphInfo * info = (Font::GlyphInfo * )data;
					Ogre::Real horiz_height = info->aspectRatio * mViewportAspectCoef * realCharHeight;

					// �������� ������� ������
					left = right;
					right += horiz_height;

					// �����, ����������
					if (pos == currect_pos) return IntPoint((int)((1.0f + left) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));
					else if (pos == (currect_pos+1)) return IntPoint((int)((1.0f + right) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));

					// ��������� ������
					currect_pos ++;

				}

			}

			// � ����� �����
			return IntPoint((int)((1.0f + right) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));
		}

	}; // class TextEditOverlayElement : public TextAreaOverlayElement

} // namespace MyGUI

#endif // __TEXT_EDIT_OVERLAY_ELEMENT_H__
