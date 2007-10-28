#ifndef _TEXTSIMPLEOVERLAYELEMENT_H_
#define _TEXTSIMPLEOVERLAYELEMENT_H_

#include "Prerequest.h"
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

	using namespace Ogre;

	class _MyGUIExport TextSimpleOverlayElement : public TextAreaOverlayElement
	{

	private:
		int m_left_margin, m_right_margin, m_top_margin, m_bottom_margin; // перекрытие
		char m_align;
		bool mRenderGL;// для конвертирования цвета вершин
		Ogre::RGBA mDefaultColor; // цвет текста
		Ogre::RGBA mInverseColor; // инверсный цвет текста
		size_t mStartSelect, mEndSelect; // начало и конец выделения

	protected:
		typedef std::vector<size_t> VectorCharInfo;
		typedef std::vector<VectorCharInfo> VectorLineInfo;
		VectorLineInfo mLinesInfo;
		bool mRawDataOutOfDate;
		float mOldViewportAspectCoef; // последний коээфициент
		float mTextureHeightOne, mTextureWidthOne; // размер одной текстурной единицы


	public:
		TextSimpleOverlayElement(const String& name) :
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
			// для конвертирования цвета вершин
			mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());
		}

		inline void setSelect(size_t _start, size_t _end) {mStartSelect=_start;mEndSelect=_end;mGeomPositionsOutOfDate = true;}
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
			mDerivedOutOfDate = true;
		}

		virtual void updatePositionGeometry()
		{
			// нет шрифта, не вечеринки
			if (mpFont.isNull()) return;

			// если нуно обновить, или изменились пропорции экрана
			if (mRawDataOutOfDate || (mOldViewportAspectCoef != mViewportAspectCoef) ) updateRawData();

			// позиция отображаемого символа
			size_t cursor = 0;

			// для уменьшения умножений, поможем компилятору =)
			float charHeightDouble = mCharHeight * 2.0;

			// текущие цвета
			Ogre::RGBA color_current, color = mDefaultColor;
			Ogre::RGBA color_inverse = mInverseColor;
			Ogre::RGBA spec_current, spec = ((mDefaultColor>>2) & 0xFF000000);

			checkMemoryAllocation( mCaption.size() );
			mRenderOp.vertexData->vertexCount = 0;

			// Get position / texcoord buffer
			HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(MAIN_BUFFER_BINDING);
			float *pVert = static_cast<float*>(vbuf->lock(HardwareBuffer::HBL_DISCARD) );

			// опорное смещение вершин
			float left = _getDerivedLeft() * 2.0 - 1.0;
			float top = - ( (_getDerivedTop() * 2.0 ) - 1.0 );
			float right = left;
			float bottom = top;

			// края обрезки текста
			float left_margin = (mPixelScaleX * (float)m_left_margin * 2.0) + left;
			float top_margin = top - (mPixelScaleY * (float)m_top_margin * 2.0);
			float right_margin = (left + (_getWidth() * 2.0)) - (mPixelScaleX * (float)m_right_margin * 2.0);
			float bottom_margin = (top - (_getHeight() * 2.0)) + (mPixelScaleY * (float)m_bottom_margin * 2.0);

			// высчитываем смещения текста внутри бокса по вертикали, по горизонтали считается автоматически
			float textHeight = mLinesInfo.size() * mPixelCharHeight;
			if (m_align & ALIGN_BOTTOM) bottom -= mPixelScaleY * (mPixelHeight - textHeight) * 2.0;
			else if (! (m_align & ALIGN_TOP)) bottom -= mPixelScaleY * (mPixelHeight - textHeight);

			// данные непосредственно для вывода
			float vertex_top, vertex_bottom, vertex_left, vertex_right;

			// основной цикл
			VectorLineInfo::iterator end = mLinesInfo.end();
			for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

				// пересчет опорных данных
				top = bottom;
				bottom -= charHeightDouble;
				// присваиваем и вершинным
				vertex_top = top;
				vertex_bottom = bottom;

				VectorCharInfo::iterator index = line->begin();
				VectorCharInfo::iterator end_index = line->end();
				// первый всегда длинна строки
				float len = *((float*)(&(*index)));
				++index;
				// второй колличество символов
				size_t count = (*index);
				++index;

				// нуна ли пересчитывать текстурные координаты
				bool texture_crop_height = false;

				if (vertex_top > top_margin) {
					// проверка на полный выход
					if (vertex_bottom > top_margin) {
						cursor += count;
						continue;
					}
					// обрезаем
					vertex_top = top_margin;
					texture_crop_height = true;
				} 
				if (vertex_bottom < bottom_margin) {
					// вообще вниз ушли
					if (vertex_top < bottom_margin) {
						line = end;
						line --;
						cursor += count;
						continue;
					}
					// обрезаем
					vertex_bottom = bottom_margin;
					texture_crop_height = true;
				}

				// пересчет опорных данных
				right = _getDerivedLeft() * 2.0 - 1.0; // выравнивание по левой стороне
				if ( mAlignment == Right ) right += ((_getWidth() * 2.0) - len); // выравнивание по правой стороне
				else if ( mAlignment == Center ) right += ((_getWidth() * 2.0) - len) * 0.5; // выравнивание по центру

				// текущее положение в строке
				size_t cur = cursor;

				// внутренний цикл строки
				for (;index != end_index; ++index) {

					size_t data = (*index);
					// проверяем на смену цвета
					if ( (data & 0xFF000000) == 0xFF000000) {
						color = (Ogre::RGBA) (data & 0x00FFFFFF) | (color & 0xFF000000);
						color_inverse = color ^ 0x00FFFFFF;
						continue;
					}

					// отображаемый символ
					Font::GlyphInfo * info = (Font::GlyphInfo * )data;
					Real horiz_height = info->aspectRatio * mViewportAspectCoef;

					// пересчет опорных данных
					left = right;
					right += horiz_height * charHeightDouble;

					// присваиваем и вершинным
					vertex_left = left;
					vertex_right = right;

					// если пробел и он не выделен, то и не нуно его рисовать
					//if (data == )

					// текущие текстурные координаты
					float texture_left = info->uvRect.left;
					float texture_right = info->uvRect.right;
					float texture_top = info->uvRect.top;
					float texture_bottom = info->uvRect.bottom;

					// нуна ли пересчитывать текстурные координаты
					bool texture_crop_width = false;

					if (vertex_left < left_margin) {
						// проверка на полный выход
						if (vertex_right < left_margin) {
							cur ++;
							continue;
						}
						// обрезаем
						vertex_left = left_margin;
						texture_crop_width = true;
					} 
					if (vertex_right > right_margin) {
						// вообще строку до конца не нуна
						if (vertex_left > right_margin) {
							index = end_index;
							index --;
							cur ++;
							continue;
						}
						// обрезаем
						vertex_right = right_margin;
						texture_crop_width = true;
					}

					// смещение текстуры по вертикили
					if (texture_crop_height) {
						// прибавляем размер смещения в текстурных координатах
						texture_top += (top - vertex_top) * mTextureHeightOne;
						// отнимаем размер смещения в текстурных координатах
						texture_bottom -= (vertex_bottom - bottom) * mTextureHeightOne;
					}

					// смещение текстуры по горизонтали
					if (texture_crop_width) {
						// прибавляем размер смещения в текстурных координатах
						texture_left += (vertex_left - left) * mTextureWidthOne;
						// отнимаем размер смещения в текстурных координатах
						texture_right -= (right - vertex_right) * mTextureWidthOne;
					}

					// выделение текста
					if ( (cur >= mEndSelect) || (cur < mStartSelect) ) {
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

					mRenderOp.vertexData->vertexCount += 6;
					cur ++;

				}

				// следующая строка
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
				// увеличиваем еще на немного
				numChars += DEFAULT_INITIAL_CHARS;

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

				mAllocSize = numChars;
			}

		}

		void updateColours(void)
		{
		}

		void setColour(const ColourValue & _color)
		{
			Ogre::Root::getSingleton().convertColourValue(_color, &mDefaultColor);

			// инвертируемый цвет
			mInverseColor = mDefaultColor ^ 0x00FFFFFF;

			mGeomPositionsOutOfDate = true;
		}

		void setCaption(const DisplayString& text)
		{
			TextAreaOverlayElement::setCaption(text);
			mRawDataOutOfDate = true;
		}

		void setFontName( const String& font )
		{
			TextAreaOverlayElement::setFontName( font );

			// проверяем если нет пробела, то добавляем полюбак
			const Font::UVRect & rect = mpFont->getGlyphTexCoords(' ');
			if ( (rect.height() == 0) && (rect.width() == 0) ) {
				const Font::GlyphInfo & info = mpFont->getGlyphInfo(L'ё');
				mpFont->setGlyphTexCoords(' ', info.uvRect.right, info.uvRect.top, info.uvRect.right + (info.uvRect.right - info.uvRect.left), info.uvRect.bottom, info.aspectRatio / ((info.uvRect.right - info.uvRect.left)  / (info.uvRect.bottom - info.uvRect.top)));
			}
			mRawDataOutOfDate = true;
		}

		void setCharHeight( Real height )
		{
			TextAreaOverlayElement::setCharHeight( height );
			mRawDataOutOfDate = true;
		}

		void updateRawData()
		{
			if (mpFont.isNull()) return;

			// массив для быстрой конвертации цветов
			static const char convert_color[128] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

			// вычисление размера одной единицы в текстурных координатах
			float charHeightDouble = mCharHeight * 2.0;
			const Font::GlyphInfo & info = mpFont->getGlyphInfo('A');
			mTextureHeightOne = (info.uvRect.bottom - info.uvRect.top) / (charHeightDouble);
			mTextureWidthOne = (info.uvRect.right - info.uvRect.left) / (info.aspectRatio * mViewportAspectCoef * charHeightDouble);

			mLinesInfo.clear();

			// создаем первую строчку
			mLinesInfo.push_back(VectorCharInfo());
			mLinesInfo.back().push_back(0); // первый символ всегда ширина в реальных координатах
			mLinesInfo.back().push_back(0); // второй символ, колличество значимых символов
			float len = 0;
			size_t count = 1;

			DisplayString::const_iterator end = mCaption.end();
			for (DisplayString::const_iterator index=mCaption.begin(); index!=end; ++index) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);

				if (character == UNICODE_CR || character == UNICODE_NEL || character == UNICODE_LF) {
					// запоминаем размер предыдущей строки
					mLinesInfo.back()[0] = *((size_t*)(&len));
					mLinesInfo.back()[1] = count;
					count = 1;
					len = 0;

					// и создаем новую
					mLinesInfo.push_back(VectorCharInfo());
					mLinesInfo.back().push_back(0); // первый символ всегда ширина в пикселях
					mLinesInfo.back().push_back(0); // второй символ, колличество значимых символов

					if (character == UNICODE_CR) {
						DisplayString::const_iterator peeki = index;
						peeki++;
						if (peeki != end && OGRE_DEREF_DISPLAYSTRING_ITERATOR(peeki) == UNICODE_LF) index = peeki; // skip both as one newline
					} 
					// следующий символ
					continue;

				} else if (character == '#') {
					// берем следующий символ
					++ index;
					if (index == end) {--index ;continue;} // это защита

					character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);
					// если два подряд, то рисуем один шарп, если нет то меняем цвет
					if (character != '#') {

						// парсим первый символ
						size_t color = convert_color[character & 0x7F];

						// и еще пять символов после шарпа
						for (char i=0; i<5; i++) {
							++ index;
							if (index == end) {--index ;continue;} // это защита
							color <<= 4;
							color += convert_color[ OGRE_DEREF_DISPLAYSTRING_ITERATOR(index) & 0x7F];
						}

						// если нужно, то меняем красный и синий компоненты
						if (mRenderGL) color = ((color&0x00FF0000)>>16)|((color&0x000000FF)<<16)|(color&0xFF00FF00);
				
						// запоминаем цвет, в верхнем байте единицы
						mLinesInfo.back().push_back( color | 0xFF000000 );

						continue;
					}
				}

				const Font::GlyphInfo & info = mpFont->getGlyphInfo(character);
				len += info.aspectRatio * charHeightDouble * mViewportAspectCoef;

				// указатель на инфо о символе
				mLinesInfo.back().push_back( (size_t) &(const_cast<Font::GlyphInfo &>(info)) );
				count ++;

			}

			// запоминаем размер предыдущей строки
			mLinesInfo.back()[0] = *((size_t*)(&len));
			mLinesInfo.back()[1] = count;

			// сбрасывам флаги
			mRawDataOutOfDate = false;
			mOldViewportAspectCoef = mViewportAspectCoef;

		} // void updateRawData()

		void getTextCursorFromPoint(int & _x, int & _y, size_t & _pos)
		{
			// если нуно обновить, или изменились пропорции экрана
			if (mRawDataOutOfDate || (mOldViewportAspectCoef != mViewportAspectCoef) ) updateRawData();

			// позиция символа
			_pos = 0;

			float charHeightDouble = mCharHeight * 2.0;

			// абсалютные координаты
			float x = mPixelScaleX * (float)_x * 2.0;
			float y = ( 1.0f - (mPixelScaleY * (float)_y * 2.0) );

			// опорное смещение вершин
			float left = _getDerivedLeft() * 2.0 - 1.0;
			float top = - ( (_getDerivedTop() * 2.0 ) - 1.0 );
			float right = left;
			float bottom = top;

			// высота в реальных координатах
			float textHeight = mLinesInfo.size() * mPixelCharHeight;
			float height = mPixelScaleY * (mPixelHeight - textHeight) * 2.0;

			// высчитываем смещения текста внутри бокса по вертикали, по горизонтали считается автоматически
			if (m_align & ALIGN_BOTTOM) bottom -= height;
			else if (! (m_align & ALIGN_TOP)) bottom -= height * 0.5;

			// корректируем
			float test = textHeight * mPixelScaleY * 2.0;
			if (y < bottom - test) y = bottom - test;

			// основной цикл
			VectorLineInfo::iterator end = mLinesInfo.end();
			for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

				// пересчет опорных данных
				top = bottom;
				bottom -= charHeightDouble;

				VectorCharInfo::iterator index = line->begin();
				VectorCharInfo::iterator end_index = line->end();

				// первый всегда длинна строки
				float len = *((float*)(&(*index)));
				++index;

				// колличество символов
				size_t count = (*index);
				++index;

				// следующая строчка
				if (y < bottom) {
					_pos += count;
					continue;
				}


				// пересчет опорных данных
				right = _getDerivedLeft() * 2.0 - 1.0; // выравнивание по левой стороне
				if ( mAlignment == Right ) right += ((_getWidth() * 2.0) - len); // выравнивание по правой стороне
				else if ( mAlignment == Center ) right += ((_getWidth() * 2.0) - len) * 0.5; // выравнивание по центру

				if (x <= (1.0 + right)) {
					// в начало строки
					_x = (int)((1.0f + right) / (mPixelScaleX * 2.0));
					_y = (int)((1.0f - top) / (mPixelScaleY * 2.0));
					return;

				} else if (x >= (1.0 + right + len)) {
					// в конец строки
					_x = (int)((1.0f + right + len) / (mPixelScaleX * 2.0));
					_y = (int)((1.0f - top) / (mPixelScaleY * 2.0));
					_pos += count - 1;
					return;
				}

				// внутренний цикл строки
				for (;index != end_index; ++index) {

					size_t data = (*index);
					// проверяем на смену цвета
					if ( (data & 0xFF000000) == 0xFF000000) continue;

					// отображаемый символ
					Font::GlyphInfo * info = (Font::GlyphInfo * )data;
					Real horiz_height = info->aspectRatio * mViewportAspectCoef * charHeightDouble;

					// пересчет опорных данных
					left = right;
					right += horiz_height;

					// попали в символ, сравниваем с половиной ширины
					if (x < (1.0 + right)) {
						if (x < ((1.0 + right) - (horiz_height * 0.5)) ) {
							// в левой половине символа
							_x = (int)((1.0f + left) / (mPixelScaleX * 2.0));

						} else {
							// в правой половине символа
							_pos ++;
							_x = (int)((1.0f + right) / (mPixelScaleX * 2.0));
						}
						_y = (int)((1.0f - top) / (mPixelScaleY * 2.0));
						return;
					}

					// следующий символ
					_pos ++;

				}
			}


		}

		void getTextCursorFromPosition(int & _x, int & _y, size_t & _pos)
		{
			// если нуно обновить, или изменились пропорции экрана
			if (mRawDataOutOfDate || (mOldViewportAspectCoef != mViewportAspectCoef) ) updateRawData();

			// позиция каретки
			size_t pos = _pos;

			float charHeightDouble = mCharHeight * 2.0;

			// опорное смещение вершин
			float left = _getDerivedLeft() * 2.0 - 1.0;
			float top = - ( (_getDerivedTop() * 2.0 ) - 1.0 );
			float right = left;
			float bottom = top;

			// высота в реальных координатах
			float textHeight = mLinesInfo.size() * mPixelCharHeight;
			float height = mPixelScaleY * (mPixelHeight - textHeight) * 2.0;

			// высчитываем смещения текста внутри бокса по вертикали, по горизонтали считается автоматически
			if (m_align & ALIGN_BOTTOM) bottom -= height;
			else if (! (m_align & ALIGN_TOP)) bottom -= height * 0.5;

			// основной цикл
			VectorLineInfo::iterator end = mLinesInfo.end();
			for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

				// пересчет опорных данных
				top = bottom;
				bottom -= charHeightDouble;

				VectorCharInfo::iterator index = line->begin();
				VectorCharInfo::iterator end_index = line->end();

				// первый всегда длинна строки
				float len = *((float*)(&(*index)));
				++index;

				// колличество символов
				size_t count = (*index);
				++index;

				// следующая строчка
				if (pos >= count) {
					pos -= count;
					continue;
				}

				// пересчет опорных данных
				right = _getDerivedLeft() * 2.0 - 1.0; // выравнивание по левой стороне
				if ( mAlignment == Right ) right += ((_getWidth() * 2.0) - len); // выравнивание по правой стороне
				else if ( mAlignment == Center ) right += ((_getWidth() * 2.0) - len) * 0.5; // выравнивание по центру

				if (pos == 0) {
					_x = (int)((1.0f + right) / (mPixelScaleX * 2.0));
					_y = (int)((1.0f - top) / (mPixelScaleY * 2.0));
					return;
				}

				int currect_pos = 0;

				// внутренний цикл строки
				for (;index != end_index; ++index) {

					size_t data = (*index);
					// проверяем на смену цвета
					if ( (data & 0xFF000000) == 0xFF000000) continue;

					// отображаемый символ
					Font::GlyphInfo * info = (Font::GlyphInfo * )data;
					Real horiz_height = info->aspectRatio * mViewportAspectCoef * charHeightDouble;

					// пересчет опорных данных
					left = right;
					right += horiz_height;

					if (pos == currect_pos) {
						_x = (int)((1.0f + left) / (mPixelScaleX * 2.0));
						_y = (int)((1.0f - top) / (mPixelScaleY * 2.0));
						return;
					} else if (pos == (currect_pos+1)) {
						_x = (int)((1.0f + right) / (mPixelScaleX * 2.0));
						_y = (int)((1.0f - top) / (mPixelScaleY * 2.0));
						return;
					}

					// следующий символ
					currect_pos ++;

				}


			}
		}

		inline Ogre::DisplayString getTextCharInfo(wchar_t _char)
		{
			wchar_t buff[2] = {L" "};
			if (_char == '#') return L"##";
			buff[0] = _char;
			return buff;
		}

		inline Ogre::DisplayString getTextNewLine(void)
		{
			return "\n";
		}

		inline Ogre::DisplayString getTagColor(const Ogre::ColourValue & _color)
		{
			char buff[16];
			::wsprintfA(buff, "#%.2X%.2X%.2X\0", (int)(_color.r*255), (int)(_color.g*255), (int)(_color.b*255));
			return buff;
		}

		// возвращает тэг со строкой цвета по умолчанию в строке
		inline Ogre::DisplayString getTagColor()
		{
			char buff[16];
			::wsprintfA(buff, "#%.2X%.2X%.2X\0", (int)((mDefaultColor & 0x00FF0000) >> 16), (int)((mDefaultColor & 0x0000FF00) >> 8), (int)(mDefaultColor & 0x000000FF));
			return buff;
		}

		// возвращает позицию тегу по псевдо позиции
		inline void getTagColor(size_t & _start, size_t & _count)
		{
			bool find = false;
			if (_start == 0) find = true;
			_count = 0;

			size_t start = _start, position = 0, real_position = 0;

			DisplayString::const_iterator end = mCaption.end();
			for (DisplayString::const_iterator index=mCaption.begin(); index!=end; ++index, ++ real_position) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);

				if (character == UNICODE_CR || character == UNICODE_NEL || character == UNICODE_LF) {

					if (character == UNICODE_CR) {
						DisplayString::const_iterator peeki = index;
						peeki ++;
						if (peeki != end && OGRE_DEREF_DISPLAYSTRING_ITERATOR(peeki) == UNICODE_LF) {
							index = peeki; // skip both as one newline
							++ real_position;
						}
					} 

					position ++;
					if (!find) {
						if (position == start) {
							_start = real_position + 1;
							find = true;
						}
					} else return;

					continue;

				} else if (character == '#') {
					// берем следующий символ
					++ index;
					++ real_position;
					if (index == end) {--index ;-- real_position;continue;} // это защита

					character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);
					// если два подряд, то рисуем один шарп, если нет то меняем цвет
					if (character != '#') {

						// и еще пять символов после шарпа
						for (char i=0; i<5; i++) {
							++ index;
							++ real_position;
							if (index == end) {--index; --real_position; continue;} // это защита
						}

						if (find) {
							_count = 7;
							return;
						} else continue;
					}

				}

				position ++;
				if (!find) {
					if (position == start) {
						_start = real_position + 1;
						find = true;
					}
				} else return;

			}

		}

		void convertTextRange(size_t & _start, size_t & _count)
		{

			bool find = false;
			if (_start == 0) find = true;

			size_t position = 0, real_position = 0;
			size_t start = _start, end = _start + _count;

			DisplayString::const_iterator iend = mCaption.end();
			for (DisplayString::const_iterator index=mCaption.begin(); index!=iend; ++index, ++ real_position) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);

				if (character == UNICODE_CR || character == UNICODE_NEL || character == UNICODE_LF) {

					if (character == UNICODE_CR) {
						DisplayString::const_iterator peeki = index;
						peeki ++;
						if (peeki != iend && OGRE_DEREF_DISPLAYSTRING_ITERATOR(peeki) == UNICODE_LF) {
							index = peeki; // skip both as one newline
							++ real_position;
						}
					} 

					position ++;
					if (!find) {
						if (position == start) {
							_start = real_position + 1;
							if (_count == 0) return;
							find = true;
						}
					} else {
						if (position == end) {
							_count = (real_position + 1 - _start);
							return;
						}
					}

					continue;

				} else if (character == '#') {
					// берем следующий символ
					++ index;
					++ real_position;
					if (index == iend) {--index ;-- real_position;continue;} // это защита

					character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);
					// если два подряд, то рисуем один шарп, если нет то меняем цвет
					if (character != '#') {

						// и еще пять символов после шарпа
						for (char i=0; i<5; i++) {
							++ index;
							++ real_position;
							if (index == iend) {--index; --real_position; continue;} // это защита
						}

						continue;
					}

				}

				position ++;
				if (!find) {
					if (position == start) {
						_start = real_position + 1;
						if (_count == 0) return;
						find = true;
					}
				} else {
					if (position == end) {
						_count = (real_position + 1 - _start);
						return;
					}
				}

			}

			// не нашли стартовый вообще
			if (!find) _start = real_position;
			// до последнего символа
			_count = real_position - _start;

		}

		// длинна текста без спецсимволов
		size_t getTextLenght()
		{
			// если нуно обновить, или изменились пропорции экрана
			if (mRawDataOutOfDate || (mOldViewportAspectCoef != mViewportAspectCoef) ) updateRawData();
			// колличество символов
			size_t ret = 0;
			// основной цикл
			VectorLineInfo::iterator end = mLinesInfo.end();
			for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {
				VectorCharInfo::iterator index = line->begin();
				++index;
				ret += (*index);
			}
			return ret - 1;
		}

	}; // class TextSimpleOverlayElement : public TextAreaOverlayElement

} // namespace MyGUI

#endif
