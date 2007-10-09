#ifndef _TEXTSIMPLEOVERLAYELEMENT_H_
#define _TEXTSIMPLEOVERLAYELEMENT_H_

#include "Prerequest.h"
//#include <Ogre.h>
#include <OgreTextAreaOverlayElement.h>
#include "OgreFont.h"
#include "OgreFontManager.h"
#include "AlignInfo.h"

namespace MyGUI
{

	#define DEFAULT_INITIAL_CHARS 12

	#define MAIN_BUFFER_BINDING 0

	#define UNICODE_NEL 0x0085
	#define UNICODE_CR 0x000D
	#define UNICODE_LF 0x000A
	#define UNICODE_SPACE 0x0020
	#define UNICODE_ZERO 0x0030

	using namespace Ogre;

	class _MyGUIExport TextSimpleOverlayElement : public TextAreaOverlayElement
	{

	private:
		int m_left_margin, m_right_margin, m_top_margin, m_bottom_margin; // перекрытие
		float m_textHeight; // высота всех строк в тексте
		char m_align;
		bool mRenderGL;// для конвертирования цвета вершин
		Ogre::RGBA mDefaultColor; // цвет текста
		Ogre::RGBA mInverseColor; // инверсный цвет текста
		size_t mStartSelect, mEndSelect; // начало и конец выделения


	public:
		TextSimpleOverlayElement(const String& name) :
			TextAreaOverlayElement(name),
			m_left_margin (0),
			m_right_margin (0),
			m_top_margin (0),
			m_bottom_margin (0),
			m_textHeight(0),
			m_align(ALIGN_CENTER),
			mDefaultColor(0xFFFFFFFF),
			mInverseColor(0xFF000000),
			mStartSelect(0), mEndSelect(0)
		{
			// для конвертирования цвета вершин
			mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());
		}

		inline void setSelect(size_t _start, size_t _end) {mStartSelect=_start;mEndSelect=_end;mDerivedOutOfDate = true;}
		inline size_t getSelectStart() {return mStartSelect;}
		inline size_t getSelectEnd() {return mEndSelect;}

		// необходимо обновить все что связанно с стекстом
		inline void setAlignment(char _align)	
		{
			// выравнивание бокса
			m_align = _align;
			// выравнивание строк внутри бокса
			if (_align & ALIGN_RIGHT) mAlignment = Right;
			else if (! (_align & ALIGN_LEFT)) mAlignment = Center;
		}

		// устанавливет размеры по которым резать текст
		inline void setMargin(int _left, int _top, int _right, int _bottom)
		{
			m_left_margin = _left;
			m_top_margin = _top;
			m_right_margin = _right;
			m_bottom_margin = _bottom;
		}

		// необходимо обновить все что связанно с стекстом
		void updateText()
		{
			if (mpFont.isNull()) return;

			// Derive space with from a number 0
			if (mSpaceWidth == 0) mSpaceWidth = mpFont->getGlyphAspectRatio(UNICODE_ZERO) * mCharHeight * 2.0 * mViewportAspectCoef;

			m_textHeight = mPixelCharHeight;

			DisplayString::const_iterator iend = mCaption.end();
			for (DisplayString::const_iterator index = mCaption.begin(); index != iend; index++) {
				if (OGRE_DEREF_DISPLAYSTRING_ITERATOR(index) == UNICODE_LF) m_textHeight += mPixelCharHeight;
			}

	        mDerivedOutOfDate = true;
		}

		virtual void updatePositionGeometry()
		{

			// позиция отображаемого символа
			size_t index = 0;

			if (mpFont.isNull()) return;

			// массив для быстрой конвертации цветов
			static const char convert_color[128] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

			Ogre::RGBA color_current, color = mDefaultColor;
			Ogre::RGBA color_inverse = mInverseColor;
			Ogre::RGBA spec_current, spec = ((mDefaultColor>>2) & 0xFF000000);

			size_t charlen = mCaption.size();
			checkMemoryAllocation( charlen );

			mRenderOp.vertexData->vertexCount = charlen * 6;
			// Get position / texcoord buffer
			HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(MAIN_BUFFER_BINDING);
			float *pVert = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD) );

			// опорное смещение вершин
			float left = _getDerivedLeft() * 2.0 - 1.0;
			float top = -( (_getDerivedTop() * 2.0 ) - 1.0 );
			float right = left;
			float bottom;

			// края обрезки текста
			float left_margin = (mPixelScaleX * (float)m_left_margin * 2.0) + left;
			float top_margin = top - (mPixelScaleY * (float)m_top_margin * 2.0);
			float right_margin = (left + (this->_getWidth() * 2.0)) - (mPixelScaleX * (float)m_right_margin * 2.0);
			float bottom_margin = (top - (this->_getHeight() * 2.0)) + (mPixelScaleY * (float)m_bottom_margin * 2.0);

			// высчитываем смещения текста внутри бокса по вертикали, по горизонтали считается автоматически
			if (m_align & ALIGN_BOTTOM) top -= mPixelScaleY * (mPixelHeight - m_textHeight) * 2.0;
			else if (! (m_align & ALIGN_TOP)) top -= mPixelScaleY * (mPixelHeight - m_textHeight);
			bottom = top - mCharHeight * 2.0;

			// смещение для вывода
			float vertex_left = left;
			float vertex_top = top;
			float vertex_right = right;
			float vertex_bottom = bottom;

			// текстурные координаты для вывода
			float texture_left, texture_right, texture_top, texture_bottom;

			// вычисление размера одной единицы в текстурных координатах
			float char_aspect = mpFont->getGlyphAspectRatio('A') * mViewportAspectCoef;
			const Font::UVRect& uvRect = mpFont->getGlyphTexCoords('A');
			float texture_height_one = (uvRect.bottom - uvRect.top) / (mCharHeight * 2.0);
			float texture_width_one = (uvRect.right - uvRect.left) / (char_aspect * mCharHeight * 2.0);

			// просчет вертикальной обрезки
			bool skip_line = false;
			bool texture_crop_height = false;
			bool texture_crop_width = false;

			// уход за верхнюю границу
			if (top_margin < vertex_top) {
				if (top_margin < vertex_bottom) {
					// совсем ушел вверх
					skip_line = true;
				} else {
					// не доконца ушел вверх
					vertex_top = top_margin;
					texture_crop_height= true;
				}
			}

			// уход за нижнюю границу
			if ((bottom_margin > vertex_bottom) && (!skip_line)) { // skip_line чтоб не пересчитывать
				if (bottom_margin > vertex_top) {
					// совсем ушел вниз
					skip_line = true;
				} else {
					// не доконца ушел вниз
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

			for( i = mCaption.begin(); i != iend; ++i, index ++) {

				if( newLine ) {
					Real len = 0.0f;
					for( DisplayString::iterator j = i; j != iend; j++ ) {
						Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(j);
						if (character == UNICODE_CR || character == UNICODE_NEL || character == UNICODE_LF) break;
						else if (character == UNICODE_SPACE) len += mSpaceWidth;
						else if (character == '#') { // это цвет -> шарп + 6 символов

							// берем следующий символ
							++ j;
							if (j == iend) {--j ;break;} // это защита
							character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(j);

							// если два подряд, то оставляем один
							if (character == '#') len += mpFont->getGlyphAspectRatio(character) * mCharHeight * 2.0 * mViewportAspectCoef;

							// либо еще пять для цвета
							else {
								for (char ii=0; ii<5; ii++) {
									++ j;
									if (j == iend) {--j ;break;} // это защита
								}
							}
						}
						else len += mpFont->getGlyphAspectRatio(character) * mCharHeight * 2.0 * mViewportAspectCoef;
					}

					right = _getDerivedLeft() * 2.0 - 1.0; // выравнивание по левой стороне
					if ( mAlignment == Right ) right += ((this->_getWidth() * 2.0) - len); // выравнивание по правой стороне
					else if ( mAlignment == Center ) right += ((this->_getWidth() * 2.0) - len) * 0.5; // выравнивание по центру

					newLine = false;

				} // if ( newLine ) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(i);
				if (character == UNICODE_CR || character == UNICODE_NEL || character == UNICODE_LF) {

					// пересчет опорных данных
					top = bottom;
					bottom -= mCharHeight * 2.0;

					// присваиваем и вершинным
					vertex_top = top;
					vertex_bottom = bottom;

					// просчет вертикальной обрезки
					skip_line = false;
					texture_crop_height = false;
					// уход за верхнюю границу
					if (top_margin < vertex_top) {
						if (top_margin < vertex_bottom) {
							// совсем ушел вверх
							skip_line = true;
						} else {
							// не доконца ушел вверх
							vertex_top = top_margin;
							texture_crop_height= true;
						}
					}

					// уход за нижнюю границу
					if ((bottom_margin > vertex_bottom) && (!skip_line)) { // skip_line чтоб не пересчитывать
						if (bottom_margin > vertex_top) {
							// совсем ушел вниз
							skip_line = true;
						} else {
							// не доконца ушел вниз
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

					const Font::UVRect& uvRect = mpFont->getGlyphTexCoords(character);
					texture_left = uvRect.left;
					texture_right = uvRect.right;
					texture_top = uvRect.top;
					texture_bottom = uvRect.bottom;

					// пересчет опорных данных
					left = right;
					right += mSpaceWidth;
					// присваиваем и вершинным
					vertex_left = left;
					vertex_right = right;

				} else if (character == '#') { // меняем цвет
					// уменьшаем на 6 вершин
					mRenderOp.vertexData->vertexCount -= 6;

					// берем следующий символ
					++ i;
					if (i == iend) {--i ;continue;} // это защита
					character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(i);

					// если два подряд, то рисуем один шарп, если нет то меняем цвет
					if (character != '#') {

						color >>= 24;
						color <<= 4;
						color += convert_color[character & 0x7F];;
						// уменьшаем на 6 вершин
						mRenderOp.vertexData->vertexCount -= 6;

						// и еще пять символов после шарпа
						for (char ii=0; ii<5; ii++) {
							++ i;
							if (i == iend) {--i ;continue;} // это защита
							color <<= 4;
							color += convert_color[OGRE_DEREF_DISPLAYSTRING_ITERATOR(i) & 0x7F];
							// уменьшаем на 6 вершин
							mRenderOp.vertexData->vertexCount -= 6;
						}

						// если нужно, то меняем красный и синий компоненты
						if (mRenderGL) color = ((color&0x00FF0000)>>16)|((color&0x000000FF)<<16)|(color&0xFF00FF00);
						// инвертируемый цвет
						color_inverse = color ^ 0x00FFFFFF;

						// следующий символ
						continue;
					}
				} else {

					// обыкновенный символ
					Real horiz_height = mpFont->getGlyphAspectRatio(character) * mViewportAspectCoef ;
					const Font::UVRect& uvRect = mpFont->getGlyphTexCoords(character);
					texture_left = uvRect.left;
					texture_right = uvRect.right;
					texture_top = uvRect.top;
					texture_bottom = uvRect.bottom;

					// пересчет опорных данных
					left = right;
					right += horiz_height * mCharHeight * 2.0;
					// присваиваем и вершинным
					vertex_left = left;
					vertex_right = right;
				}

				if (skip_line) {
					// вообще не рисуем символ и умешьшаем размер буфера
					mRenderOp.vertexData->vertexCount -= 6;
					continue;
				}

				// просчет горизонтальной обрезки
				texture_crop_width = false;
				// уход за левую границу
				if (left_margin > vertex_left) {
					if (left_margin > vertex_right) {
						// совсем ушел влево
						// вообще не рисуем символ и умешьшаем размер буфера
						mRenderOp.vertexData->vertexCount -= 6;
						continue;

					} else {
						// не доконца ушел влево
						vertex_left = left_margin;
						texture_crop_width= true;
					}
				}

				// уход за правую границу
				if (right_margin < vertex_right) {
					if (right_margin < vertex_left) {
						// совсем ушел вправо
						// вообще не рисуем символ и умешьшаем размер буфера
						mRenderOp.vertexData->vertexCount -= 6;
						continue;
					} else {
						// не доконца ушел вправо
						vertex_right = right_margin;
						texture_crop_width = true;
					}
				}

				// смещение текстуры по вертикили
				if (texture_crop_height) {
					// прибавляем размер смещения в текстурных координатах
					texture_top += (top - vertex_top) * texture_height_one;
					// отнимаем размер смещения в текстурных координатах
					texture_bottom -= (vertex_bottom - bottom) * texture_height_one;
				}

				// смещение текстуры по горизонтали
				if (texture_crop_width) {
					// прибавляем размер смещения в текстурных координатах
					texture_left += (vertex_left - left) * texture_width_one;
					// отнимаем размер смещения в текстурных координатах
					texture_right -= (right - vertex_right) * texture_width_one;
				}


				// выделение текста
				if ( (index > mEndSelect) || (index <= mStartSelect) ) {
					color_current = color;
					spec_current = 0;
				} else {
					color_current = color_inverse;
					spec_current = spec;
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
				*((RGBA *)(pVert++)) = color_current;
				*((RGBA *)(pVert++)) = spec_current;


				// Bottom left
				*pVert++ = vertex_left;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_left;
				*pVert++ = texture_bottom;
				*((RGBA *)(pVert++)) = color_current;
				*((RGBA *)(pVert++)) = spec_current;

				// Top right
				*pVert++ = vertex_right;
				*pVert++ = vertex_top;
				*pVert++ = -1.0;
				*pVert++ = texture_right;
				*pVert++ = texture_top;
				*((RGBA *)(pVert++)) = color_current;
				*((RGBA *)(pVert++)) = spec_current;
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
				*((RGBA *)(pVert++)) = color_current;
				*((RGBA *)(pVert++)) = spec_current;

				// Bottom left (again)
				*pVert++ = vertex_left;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_left;
				*pVert++ = texture_bottom;
				*((RGBA *)(pVert++)) = color_current;
				*((RGBA *)(pVert++)) = spec_current;

				// Bottom right
				*pVert++ = vertex_right;
				*pVert++ = vertex_bottom;
				*pVert++ = -1.0;
				*pVert++ = texture_right;
				*pVert++ = texture_bottom;
				*((RGBA *)(pVert++)) = color_current;
				*((RGBA *)(pVert++)) = spec_current;
				//-------------------------------------------------------------------------------------

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
				mRenderOp.vertexData = new VertexData();
				VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
				size_t offset = 0;
				// Positions
				decl->addElement(MAIN_BUFFER_BINDING, offset, VET_FLOAT3, VES_POSITION);
				offset += VertexElement::getTypeSize(VET_FLOAT3);
				// Texcoords
				decl->addElement(MAIN_BUFFER_BINDING, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
				offset += VertexElement::getTypeSize(VET_FLOAT2);

				decl->addElement(MAIN_BUFFER_BINDING, offset, VET_COLOUR, VES_DIFFUSE);
				offset += VertexElement::getTypeSize(VET_COLOUR);

				decl->addElement(MAIN_BUFFER_BINDING, offset, VET_COLOUR, VES_SPECULAR);
				offset += VertexElement::getTypeSize(VET_COLOUR);
				

				mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST;
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
				VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
				VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

				mRenderOp.vertexData->vertexCount = numChars * 6;

				// Create dynamic since text tends to change alot
				// positions & texcoords
				HardwareVertexBufferSharedPtr vbuf = 
					HardwareBufferManager::getSingleton().
						createVertexBuffer(
							decl->getVertexSize(MAIN_BUFFER_BINDING), 
							mRenderOp.vertexData->vertexCount,
							HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
				bind->setBinding(MAIN_BUFFER_BINDING, vbuf);

				// colours
				/*vbuf = HardwareBufferManager::getSingleton().
						createVertexBuffer(
							decl->getVertexSize(COLOUR_BINDING), 
							mRenderOp.vertexData->vertexCount,
							HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
				bind->setBinding(COLOUR_BINDING, vbuf);*/

				mAllocSize = numChars;
				//mColoursChanged = true; // force colour buffer regeneration
			}

		}

		void updateColours(void)
		{
			// Convert to system-specific
			/*RGBA topColour, bottomColour;
			Root::getSingleton().convertColourValue(mColourTop, &topColour);
			Root::getSingleton().convertColourValue(mColourBottom, &bottomColour);

			HardwareVertexBufferSharedPtr vbuf = 
				mRenderOp.vertexData->vertexBufferBinding->getBuffer(COLOUR_BINDING);

			RGBA* pDest = static_cast<RGBA*>(vbuf->lock(HardwareBuffer::HBL_DISCARD) );

			for (size_t i = 0; i < mAllocSize; ++i)
			{
				// First tri (top, bottom, top)
				*pDest++ = topColour;
				*pDest++ = bottomColour;
				*pDest++ = topColour;
				// Second tri (top, bottom, bottom)
				*pDest++ = topColour;
				*pDest++ = bottomColour;
				*pDest++ = bottomColour;
			}
			vbuf->unlock();*/

		}

		void setColour(const ColourValue & _color)
		{
			Ogre::Root::getSingleton().convertColourValue(_color, &mDefaultColor);

			// инвертируемый цвет
			mInverseColor = mDefaultColor ^ 0x00FFFFFF;

			mGeomPositionsOutOfDate = true;
		}

	}; // class TextSimpleOverlayElement : public TextAreaOverlayElement

} // namespace MyGUI

#endif
