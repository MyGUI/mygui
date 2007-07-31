#pragma once

#include <Ogre.h>
#include <OgreTextAreaOverlayElement.h>
#include "OgreFont.h"
#include "OgreFontManager.h"

#include "debugOut.h"


namespace widget
{

	#define POS_TEX_BINDING 0
    #define COLOUR_BINDING 1
	#define UNICODE_NEL 0x0085
	#define UNICODE_CR 0x000D
	#define UNICODE_LF 0x000A
	#define UNICODE_SPACE 0x0020
	#define UNICODE_ZERO 0x0030

	using namespace Ogre;

	class TextSimpleOverlayElement : public TextAreaOverlayElement
	{

	private:
		int m_left_margin, m_right_margin, m_top_margin, m_bottom_margin; // ����������

	public:
		TextSimpleOverlayElement(const String& name) :
			TextAreaOverlayElement(name),
			m_left_margin (0),
			m_right_margin (0),
			m_top_margin (0),
			m_bottom_margin (0)
		  {}

		// ������������ ������� �� ������� ������ �����
		void setMargin(int _left, int _top, int _right, int _bottom)
		{
			m_left_margin = _left;
			m_top_margin = _top;
			m_right_margin = _right;
			m_bottom_margin = _bottom;
		}

		void getTextSize(int & _cx, int & _cy)
		{

			if (mpFont.isNull()) return;

			// Derive space with from a number 0
			if (mSpaceWidth == 0) mSpaceWidth = mpFont->getGlyphAspectRatio(UNICODE_ZERO) * mCharHeight * 2.0 * mViewportAspectCoef;

			float cy = mPixelCharHeight;
			float cx = 0;
			float len = 0;

			DisplayString::const_iterator iend = mCaption.end();

			for (DisplayString::const_iterator index = mCaption.begin(); index != iend; index++) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);

				if ( character == UNICODE_SPACE) len += mSpaceWidth;
				else if (character == UNICODE_LF) { // ������� ������, ��������� ������ ������ 0, ��� ������ �� ���� ������

					if (len > cx) cx = len;
					len = 0;
					cy += mPixelCharHeight;

				} else len += mpFont->getGlyphAspectRatio(character) * mPixelCharHeight;
			}

			if (len > cx) cx = len;
	
			_cx = (int)cx;
			_cy = (int)cy;
			// ��������� � ������� ����������
			if ((float)_cx < cx) _cx ++;

			// ��� ����������� �������� ���������
            mPixelWidth = _cx;
            mPixelHeight = _cy;
	        mDerivedOutOfDate = true;

		}

		virtual void updatePositionGeometry()
		{

			if (mpFont.isNull()) return;

			size_t charlen = mCaption.size();
			checkMemoryAllocation( charlen );

			mRenderOp.vertexData->vertexCount = charlen * 6;
			// Get position / texcoord buffer
			HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(POS_TEX_BINDING);
			float *pVert = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD) );

			// ������� �������� ������
			float left = _getDerivedLeft() * 2.0 - 1.0;
			float top = -( (_getDerivedTop() * 2.0 ) - 1.0 );
			float right = left;
			float bottom = top - mCharHeight * 2.0;

			// �������� ��� ������
			float vertex_left = left;
			float vertex_top = top;
			float vertex_right = right;
			float vertex_bottom = bottom;

			// ���� ������� ������
			float left_margin = (mPixelScaleX * (float)m_left_margin * 2.0) + left;
			float right_margin = (left + (this->_getWidth() * 2.0)) - (mPixelScaleX * (float)m_right_margin * 2.0);
			float top_margin = top - (mPixelScaleY * (float)m_top_margin * 2.0);
			float bottom_margin = (top - (this->_getHeight() * 2.0)) + (mPixelScaleY * (float)m_bottom_margin * 2.0);

			// ���������� ���������� ��� ������
			float texture_left;
			float texture_right;
			float texture_top;
			float texture_bottom;

			// ���������� ������� ����� ������� � ���������� �����������
			float char_aspect = mpFont->getGlyphAspectRatio('A') * mViewportAspectCoef;
			const Font::UVRect& uvRect = mpFont->getGlyphTexCoords('A');
			float texture_height_one = (uvRect.bottom - uvRect.top) / (mCharHeight * 2.0);
			float texture_width_one = (uvRect.right - uvRect.left) / (char_aspect * mCharHeight * 2.0);

			// ������� ������������ �������
			bool skip_line = false;
			bool texture_crop_height = false;
			bool texture_crop_width = false;

			// ���� �� ������� �������
			if (top_margin < vertex_top) {
				if (top_margin < vertex_bottom) {
					// ������ ���� �����
					skip_line = true;
				} else {
					// �� ������� ���� �����
					vertex_top = top_margin;
					texture_crop_height= true;
				}
			}

			// ���� �� ������ �������
			if ((bottom_margin > vertex_bottom) && (!skip_line)) { // skip_line ���� �� �������������
				if (bottom_margin > vertex_top) {
					// ������ ���� ����
					skip_line = true;
				} else {
					// �� ������� ���� ����
					vertex_bottom = bottom_margin;
					texture_crop_height = true;
				}
			}

			// Derive space with from a number 0
			if (mSpaceWidth == 0) mSpaceWidth = mpFont->getGlyphAspectRatio(UNICODE_ZERO) * mCharHeight * 2.0 * mViewportAspectCoef;

			// Use iterator
			DisplayString::iterator i, iend;
			iend = mCaption.end();
			bool newLine = true;

			for( i = mCaption.begin(); i != iend; ++i ) {

				if( newLine ) {
					Real len = 0.0f;
					for( DisplayString::iterator j = i; j != iend; j++ ) {
						Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(j);
						if (character == UNICODE_CR || character == UNICODE_NEL || character == UNICODE_LF) break;
						else if (character == UNICODE_SPACE) len += mSpaceWidth;
						else len += mpFont->getGlyphAspectRatio(character) * mCharHeight * 2.0 * mViewportAspectCoef;
					}

					right = _getDerivedLeft() * 2.0 - 1.0; // ������������ �� ����� �������
					if ( mAlignment == Right ) right += ((this->_getWidth() * 2.0) - len); // ������������ �� ������ �������
					else if ( mAlignment == Center ) right += ((this->_getWidth() * 2.0) - len) * 0.5; // ������������ �� ������

					newLine = false;

				} // if ( newLine ) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(i);
				if (character == UNICODE_CR || character == UNICODE_NEL || character == UNICODE_LF) {

					// �������� ������� ������
					top = bottom;
					bottom -= mCharHeight * 2.0;

					// ����������� � ���������
					vertex_top = top;
					vertex_bottom = bottom;

					// ������� ������������ �������
					skip_line = false;
					texture_crop_height = false;
					// ���� �� ������� �������
					if (top_margin < vertex_top) {
						if (top_margin < vertex_bottom) {
							// ������ ���� �����
							skip_line = true;
						} else {
							// �� ������� ���� �����
							vertex_top = top_margin;
							texture_crop_height= true;
						}
					}

					// ���� �� ������ �������
					if ((bottom_margin > vertex_bottom) && (!skip_line)) { // skip_line ���� �� �������������
						if (bottom_margin > vertex_top) {
							// ������ ���� ����
							skip_line = true;
						} else {
							// �� ������� ���� ����
							vertex_bottom = bottom_margin;
							texture_crop_height = true;
						}
					}


					newLine = true;
					// Also reduce tri count
					mRenderOp.vertexData->vertexCount -= 6;

					// consume CR/LF in one
					if (character == UNICODE_CR) {
						DisplayString::iterator peeki = i;
						peeki++;
						if (peeki != iend && OGRE_DEREF_DISPLAYSTRING_ITERATOR(peeki) == UNICODE_LF) {
							i = peeki; // skip both as one newline
							// Also reduce tri count
							mRenderOp.vertexData->vertexCount -= 6;
						}

					}
					continue;

				} else if (character == UNICODE_SPACE) {
					// Just leave a gap, no tris
					left = right;
					right += mSpaceWidth;
					// Also reduce tri count
					mRenderOp.vertexData->vertexCount -= 6;
					continue;
				}

				Real horiz_height = mpFont->getGlyphAspectRatio(character) * mViewportAspectCoef ;
				const Font::UVRect& uvRect = mpFont->getGlyphTexCoords(character);
				texture_left = uvRect.left;
				texture_right = uvRect.right;
				texture_top = uvRect.top;
				texture_bottom = uvRect.bottom;

				// �������� ������� ������
				left = right;
				right += horiz_height * mCharHeight * 2.0;
				// ����������� � ���������
				vertex_left = left;
				vertex_right = right;

				if (skip_line) {
					// ������ �� ������ ������ � ��������� ������ ������
					mRenderOp.vertexData->vertexCount -= 6;
					continue;
				}

				// ������� �������������� �������
				texture_crop_width = false;
				// ���� �� ����� �������
				if (left_margin > vertex_left) {
					if (left_margin > vertex_right) {
						// ������ ���� �����
						// ������ �� ������ ������ � ��������� ������ ������
						mRenderOp.vertexData->vertexCount -= 6;
						continue;

					} else {
						// �� ������� ���� �����
						vertex_left = left_margin;
						texture_crop_width= true;
					}
				}

				// ���� �� ������ �������
				if (right_margin < vertex_right) {
					if (right_margin < vertex_left) {
						// ������ ���� ������
						// ������ �� ������ ������ � ��������� ������ ������
						mRenderOp.vertexData->vertexCount -= 6;
						continue;
					} else {
						// �� ������� ���� ������
						vertex_right = right_margin;
						texture_crop_width = true;
					}
				}

				// �������� �������� �� ���������
				if (texture_crop_height) {
					// ���������� ������ �������� � ���������� �����������
					texture_top += (top - vertex_top) * texture_height_one;
					// �������� ������ �������� � ���������� �����������
					texture_bottom -= (vertex_bottom - bottom) * texture_height_one;
				}

				// �������� �������� �� �����������
				if (texture_crop_width) {
					// ���������� ������ �������� � ���������� �����������
					texture_left += (vertex_left - left) * texture_width_one;
					// �������� ������ �������� � ���������� �����������
					texture_right -= (right - vertex_right) * texture_width_one;
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

				// Bottom left
				*pVert++ = vertex_left;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_left;
				*pVert++ = texture_bottom;

				// Top right
				*pVert++ = vertex_right;
				*pVert++ = vertex_top;
				*pVert++ = -1.0;
				*pVert++ = texture_right;
				*pVert++ = texture_top;
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

				// Bottom left (again)
				*pVert++ = vertex_left;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_left;
				*pVert++ = texture_bottom;

				// Bottom right
				*pVert++ = vertex_right;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_right;
				*pVert++ = texture_bottom;
				//-------------------------------------------------------------------------------------

			}
			// Unlock vertex buffer
			vbuf->unlock();

		}

	}; // class TextSimpleOverlayElement : public TextAreaOverlayElement

} // namespace widget