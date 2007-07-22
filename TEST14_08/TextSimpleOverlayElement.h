#pragma once

#include <Ogre.h>
#include <OgreTextAreaOverlayElement.h>
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

			float height = 20;
			float cy = height;
			float cx = 0;
			float len = 0;

			DisplayString::const_iterator iend = mCaption.end();

			for (DisplayString::const_iterator index = mCaption.begin(); index != iend; index++) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);

				if ( character == UNICODE_SPACE) len += mSpaceWidth;
				else if (character == 10) { // ������� ������

					if (len > cx) cx = len;
					len = 0;
					cy += height;

				} else len += mpFont->getGlyphAspectRatio(character) * height;
			}

			if (len > cx) cx = len;
	
			_cx = (int)cx;
			_cy = (int)cy;

		}

		virtual void updatePositionGeometry()
		{

            //Real vpWidth, vpHeight;
            //OverlayManager& oMgr = OverlayManager::getSingleton();
         //   vpWidth = (Real) (oMgr.getViewportWidth());
      //      vpHeight = (Real) (oMgr.getViewportHeight());

   //         float mPixelScaleX = 1.0 / vpWidth;
//            float mPixelScaleY = 1.0 / vpHeight;

//            mTop = mPixelTop * mPixelScaleY;
   //         mWidth = mPixelWidth * mPixelScaleX;
      //      mHeight = mPixelHeight * mPixelScaleY;


			float *pVert;

			if (mpFont.isNull()) return;

			size_t charlen = mCaption.size();
			checkMemoryAllocation( charlen );

			mRenderOp.vertexData->vertexCount = charlen * 6;
			// Get position / texcoord buffer
			HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(POS_TEX_BINDING);
			pVert = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD) );

			float largestWidth = 0;
			float left = _getDerivedLeft() * 2.0 - 1.0;
			float top = -( (_getDerivedTop() * 2.0 ) - 1.0 );

			// ����� ���� ������� ������
			float left_margin = (mPixelScaleX * (float)m_left_margin * 2.0) + left;
			// ������ ���� ������� ������
			float right_margin = (left + (this->_getWidth() * 2.0)) - (mPixelScaleX * (float)m_right_margin * 2.0);
			

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

						if (character == UNICODE_CR || character == UNICODE_NEL || character == UNICODE_LF) {
							break;
						} else if (character == UNICODE_SPACE) {// space
							len += mSpaceWidth;
						} else {
							len += mpFont->getGlyphAspectRatio(character) * mCharHeight * 2.0 * mViewportAspectCoef;
						}
					}

					newLine = false;
				}

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(i);

				if (character == UNICODE_CR || character == UNICODE_NEL || character == UNICODE_LF) {

					left = _getDerivedLeft() * 2.0 - 1.0;
					top -= mCharHeight * 2.0;
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

				} else if (character == UNICODE_SPACE) {// space
					// Just leave a gap, no tris
					left += mSpaceWidth;
					// Also reduce tri count
					mRenderOp.vertexData->vertexCount -= 6;
					continue;
				}

				Real horiz_height = mpFont->getGlyphAspectRatio(character) * mViewportAspectCoef ;
				const Font::UVRect& uvRect = mpFont->getGlyphTexCoords(character);

				// �������� ��� �����������
				#define OFFSET_OUT 1000

				// ����� �� ������������� ���������� ����������
				bool texture_crop = false;

				// ���������� ������
				float vertex_left = left;
				float vertex_right = left + horiz_height * mCharHeight * 2.0;
				float vertex_top = top;
				float vertex_bottom = top - mCharHeight * 2.0;

				// ���������� ��������
				float text_left = uvRect.left;
				float text_right = uvRect.right;
				float text_top = uvRect.top;
				float text_bottom = uvRect.bottom;

				// ���� �� ����� �������
				if (left_margin > vertex_left) {
					if (left_margin > vertex_right) {
						// ������ ���� �����
						vertex_left = OFFSET_OUT;
						vertex_right = OFFSET_OUT;
					} else {
						// �� ������� ���� �����
						vertex_left = left_margin;
						texture_crop = true;
					}
				}

				// ���� �� ������ �������
				if (right_margin < vertex_right) {
					if (right_margin < vertex_left) {
						// ������ ���� ������
						vertex_left = OFFSET_OUT;
						vertex_right = OFFSET_OUT;
					} else {
						// �� ������� ���� ������
						vertex_right = right_margin;
						texture_crop = true;
					}
				}

				// each vert is (x, y, z, u, v)
				//-------------------------------------------------------------------------------------
				// First tri
				//
				// Upper left
				*pVert++ = vertex_left;
				*pVert++ = vertex_top;
				*pVert++ = -1.0;
				*pVert++ = text_left;
				*pVert++ = text_top;

				// Bottom left
				*pVert++ = vertex_left;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = text_left;
				*pVert++ = text_bottom;

				// Top right
				*pVert++ = vertex_right;
				*pVert++ = vertex_top;
				*pVert++ = -1.0;
				*pVert++ = text_right;
				*pVert++ = text_top;
				//-------------------------------------------------------------------------------------

				//-------------------------------------------------------------------------------------
				// Second tri
				//
				// Top right (again)
				*pVert++ = vertex_right;
				*pVert++ = vertex_top;
				*pVert++ = -1.0;
				*pVert++ = text_right;
				*pVert++ = text_top;

				// Bottom left (again)
				*pVert++ = vertex_left;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = text_left;
				*pVert++ = text_bottom;

				// Bottom right
				*pVert++ = vertex_right;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = text_right;
				*pVert++ = text_bottom;
				//-------------------------------------------------------------------------------------

				left += horiz_height * mCharHeight * 2.0;

				float currentWidth = (left + 1)/2 - _getDerivedLeft();
				if (currentWidth > largestWidth) largestWidth = currentWidth;

			} // for( i = mCaption.begin(); i != iend; ++i )

			// Unlock vertex buffer
			vbuf->unlock();

			Real vpWidth;
			vpWidth = (Real) (OverlayManager::getSingleton().getViewportWidth());

			largestWidth *= vpWidth;

			if (getWidth() < largestWidth) setWidth(largestWidth);

		} // virtual void updatePositionGeometry()

	};

} // namespace widget