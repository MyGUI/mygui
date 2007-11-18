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

		int m_left_margin, m_right_margin, m_top_margin, m_bottom_margin; // перекрытие
		char m_align;

		bool mRenderGL;// для конвертирования цвета вершин
		Ogre::RGBA mDefaultColor; // цвет текста
		Ogre::RGBA mInverseColor; // инверсный цвет текста
		size_t mStartSelect, mEndSelect; // начало и конец выделения
		IntPoint mPointShift; // смещение текста
		FloatSize mContextSize; // размер всего текста
		VectorLineInfo mLinesInfo;
		bool mRawDataOutOfDate;
		float mOldViewportAspectCoef; // последний коээфициент
		float mTextureHeightOne, mTextureWidthOne; // размер одной текстурной единицы
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
			// для конвертирования цвета вершин
			mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());
		}

		inline void setSelect(size_t _start, size_t _end) {mStartSelect=_start;mEndSelect=_end;mGeomPositionsOutOfDate = true;}
		inline size_t getSelectStart() {return mStartSelect;}
		inline size_t getSelectEnd() {return mEndSelect;}

		// обязательно перекрываем, а то он там буферов понасоздает
		void updateColours(void) { }

		// необходимо обновить все что связанно с стекстом
		inline void setAlignment(char _align)	
		{
			// выравнивание бокса
			m_align = _align;
			// выравнивание строк внутри бокса
			if (_align & ALIGN_RIGHT) mAlignment = Right;
			else if (! (_align & ALIGN_LEFT)) mAlignment = Center;
			else mAlignment = Left;
			mGeomPositionsOutOfDate = true;
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
			updateRawData();

			// позиция отображаемого символа
			size_t cursor = 0;

			// текущие цвета
			Ogre::RGBA color_current, color = mDefaultColor;
			Ogre::RGBA color_inverse = mInverseColor;

			checkMemoryAllocation( mCaption.size() );
			mRenderOp.vertexData->vertexCount = 0;

			// Get position / texcoord buffer
			Ogre::HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(MAIN_BUFFER_BINDING);
			float *pVert = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

			// для уменьшения умножений, поможем компилятору =)
			float realCharHeight = mCharHeight * 2.0;
			float realWidth = _getWidth() * 2.0;
			float realHeight = _getHeight() * 2.0;
			float realLeft = _getDerivedLeft() * 2.0;
			float realTop = _getDerivedTop() * 2.0;

			// опорное смещение вершин
			float right, left = realLeft - 1.0;
			float bottom, top = 1.0 - realTop;

			// края обрезки текста
			float left_margin = (mPixelScaleX * (float)m_left_margin * 2.0) + left;
			float top_margin = top - (mPixelScaleY * (float)m_top_margin * 2.0);
			float right_margin = (left + realWidth) - (mPixelScaleX * (float)m_right_margin * 2.0);
			float bottom_margin = (top - realHeight) + (mPixelScaleY * (float)m_bottom_margin * 2.0);

			// сдвиг текста, если вью меньше или автоматическое выравнивание то сдвигаем по внутренним правилам
			float left_shift = 0;
			if (mContextSize.width <= realWidth) {
				if ( mAlignment == Right ) left_shift = mContextSize.width - realWidth; // выравнивание по правой стороне
				else if ( mAlignment == Center ) left_shift = (mContextSize.width - realWidth) * 0.5; // выравнивание по центру
			}
			else left_shift = mPixelScaleX * (float)mPointShift.left * 2.0;
			right = left;

			// сдвиг текста, если вью меньше или автоматическое выравнивание то сдвигаем по внутренним правилам
			if (mContextSize.height <= realHeight) {
				if ( m_align & ALIGN_BOTTOM ) top += (mContextSize.height - realHeight);
				else if ( !(m_align & ALIGN_TOP) ) top += (mContextSize.height - realHeight) * 0.5;
			}
			else top += mPixelScaleY * (float)mPointShift.top * 2.0;
			bottom = top;

			// данные непосредственно для вывода
			float vertex_top, vertex_bottom, vertex_left, vertex_right;

			// основной цикл
			VectorLineInfo::iterator end = mLinesInfo.end();
			for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

				// пересчет опорных данных
				top = bottom;
				bottom -= realCharHeight;
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

						// необходимо парсить теги цветов полюбак
						for (;index != end_index; ++index) {
							size_t data = (*index);
							// проверяем на смену цвета
							if ( (data & 0xFF000000) == 0xFF000000) {
								color = (Ogre::RGBA) (data & 0x00FFFFFF) | (color & 0xFF000000);
								color_inverse = color ^ 0x00FFFFFF;
							}
						}

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
				right = (realLeft - 1.0) - left_shift; // выравнивание по левой стороне
				if ( mAlignment == Right ) right += (mContextSize.width - len); // выравнивание по правой стороне
				else if ( mAlignment == Center ) right += (mContextSize.width - len) * 0.5; // выравнивание по центру

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
					Ogre::Real horiz_height = info->aspectRatio * mViewportAspectCoef * realCharHeight;

					// пересчет опорных данных
					left = right;
					right += horiz_height;

					// смещение текстуры для фона
					FloatPoint background_current;

					// символ не выделен
					if ( (cur >= mEndSelect) || (cur < mStartSelect) ) {

						// если пробел или табуляция то рисуем только при выделении
						if ( (info->codePoint == Font::FONT_CODE_SPACE) || (info->codePoint == Font::FONT_CODE_TAB) ) {
							cur ++;
							continue;
						}

						color_current = color;
						background_current = mBackgroundEmpty;
					}
					// символ выделен
					else {
						// инверсные цвета
						color_current = color_inverse;
						background_current = mBackgroundFill;
					}

					// присваиваем и вершинным
					vertex_left = left;
					vertex_right = right;

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
							index ++;
							// для того чтобы теги цвета не терялись, нужно пройти до конца строки
							while (index != end_index) {
								size_t data = (*index);
								// проверяем на смену цвета
								if ( (data & 0xFF000000) == 0xFF000000) {
									color = (Ogre::RGBA) (data & 0x00FFFFFF) | (color & 0xFF000000);
									color_inverse = color ^ 0x00FFFFFF;
								}
								index ++;
							};
							index --; // чтобы при увеличении показывал на end
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
				// увеличиваем еще на немного
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

			// инвертируемый цвет
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

			// достаем средние точки на текстуре для выделения текста
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

			// массив для быстрой конвертации цветов
			static const char convert_color[128] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

			// вычисление размера одной единицы в текстурных координатах
			float realCharHeight = mCharHeight * 2.0;
			const Font::GlyphInfo & info = mpFont->getGlyphInfo('A');
			mTextureHeightOne = (info.uvRect.bottom - info.uvRect.top) / (realCharHeight);
			mTextureWidthOne = (info.uvRect.right - info.uvRect.left) / (info.aspectRatio * mViewportAspectCoef * realCharHeight);

			mLinesInfo.clear();

			// создаем первую строчку
			mLinesInfo.push_back(VectorCharInfo());
			mLinesInfo.back().push_back(0); // первый символ всегда ширина в реальных координатах
			mLinesInfo.back().push_back(0); // второй символ, колличество значимых символов
			float len = 0, width = 0;
			size_t count = 1;

			Ogre::DisplayString::const_iterator end = mCaption.end();
			for (Ogre::DisplayString::const_iterator index=mCaption.begin(); index!=end; ++index) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);

				if (character == Font::FONT_CODE_CR || character == Font::FONT_CODE_NEL || character == Font::FONT_CODE_LF) {
					// запоминаем размер предыдущей строки
					mLinesInfo.back()[0] = *((size_t*)(&len));
					mLinesInfo.back()[1] = count;
					if (width < len) width = len;
					count = 1;
					len = 0;

					// и создаем новую
					mLinesInfo.push_back(VectorCharInfo());
					mLinesInfo.back().push_back(0); // первый символ всегда ширина в пикселях
					mLinesInfo.back().push_back(0); // второй символ, колличество значимых символов

					if (character == Font::FONT_CODE_CR) {
						Ogre::DisplayString::const_iterator peeki = index;
						peeki++;
						if (peeki != end && OGRE_DEREF_DISPLAYSTRING_ITERATOR(peeki) == Font::FONT_CODE_LF) index = peeki; // skip both as one newline
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
				len += info.aspectRatio * realCharHeight * mViewportAspectCoef;

				// указатель на инфо о символе
				mLinesInfo.back().push_back( (size_t) &(const_cast<Font::GlyphInfo &>(info)) );
				count ++;

			}

			// запоминаем размер предыдущей строки
			mLinesInfo.back()[0] = *((size_t*)(&len));
			mLinesInfo.back()[1] = count;
			if (width < len) width = len;


			// сбрасывам флаги
			mRawDataOutOfDate = false;
			mOldViewportAspectCoef = mViewportAspectCoef;

			// устанавливаем размер текста
			mContextSize.set(width, (float)mLinesInfo.size() * mCharHeight * 2.0);

		} // void updateRawData()

		// возвращает размер текста в пикселях
		inline IntSize getTextSize()
		{
			// если нуно обновить, или изменились пропорции экрана
			updateRawData();
			return IntSize( (int)(mContextSize.width / (mPixelScaleX * 2.0)), (int)(mLinesInfo.size() * mPixelCharHeight) );
		}

		// устанавливает смещение текста в пикселях
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
			// если нуно обновить, или изменились пропорции экрана
			updateRawData();

			// позиция символа
			size_t position = 0;

			// для уменьшения умножений, поможем компилятору =)
			float realCharHeight = mCharHeight * 2.0;
			float realWidth = _getWidth() * 2.0;
			float realHeight = _getHeight() * 2.0;
			float realLeft = _getDerivedLeft() * 2.0;
			float realTop = _getDerivedTop() * 2.0;

			// абсалютные координаты
			float x = mPixelScaleX * (float)_point.left * 2.0;
			float y = ( 1.0f - (mPixelScaleY * (float)_point.top * 2.0) );

			// опорное смещение вершин
			float right, left = realLeft - 1.0;
			float bottom, top = 1.0 - realTop;

			// сдвиг текста, если вью меньше или автоматическое выравнивание то сдвигаем по внутренним правилам
			float left_shift = 0;
			if (mContextSize.width <= realWidth) {
				if ( mAlignment == Right ) left_shift = mContextSize.width - realWidth; // выравнивание по правой стороне
				else if ( mAlignment == Center ) left_shift = (mContextSize.width - realWidth) * 0.5; // выравнивание по центру
			}
			else left_shift = mPixelScaleX * (float)mPointShift.left * 2.0;
			right = left;

			// сдвиг текста, если вью меньше или автоматическое выравнивание то сдвигаем по внутренним правилам
			if (mContextSize.height <= realHeight) {
				if ( m_align & ALIGN_BOTTOM ) top += (mContextSize.height - realHeight);
				else if ( !(m_align & ALIGN_TOP) ) top += (mContextSize.height - realHeight) * 0.5;
			}
			else top += mPixelScaleY * (float)mPointShift.top * 2.0;
			bottom = top;

			// корректируем координату до нижней строки
			if (y < (bottom - mContextSize.height)) y = bottom - mContextSize.height;

			// основной цикл
			VectorLineInfo::iterator end = mLinesInfo.end();
			for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

				// пересчет опорных данных
				top = bottom;
				bottom -= realCharHeight;

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
					position += count;
					continue;
				}

				// пересчет опорных данных
				right = (realLeft - 1.0) - left_shift; // выравнивание по левой стороне
				if ( mAlignment == Right ) right += (mContextSize.width - len); // выравнивание по правой стороне
				else if ( mAlignment == Center ) right += (mContextSize.width - len) * 0.5; // выравнивание по центру

				if (x <= (1.0 + right)) {
					// в начало строки
					_point.left = (int)((1.0f + right) / (mPixelScaleX * 2.0));
					_point.top = (int)((1.0f - top) / (mPixelScaleY * 2.0));
					return position;

				} else if (x >= (1.0 + right + len)) {
					// в конец строки
					_point.left = (int)((1.0f + right + len) / (mPixelScaleX * 2.0));
					_point.top = (int)((1.0f - top) / (mPixelScaleY * 2.0));
					position += count - 1;
					return position;
				}

				// внутренний цикл строки
				for (;index != end_index; ++index) {

					size_t data = (*index);
					// проверяем на смену цвета
					if ( (data & 0xFF000000) == 0xFF000000) continue;

					// отображаемый символ
					Font::GlyphInfo * info = (Font::GlyphInfo * )data;
					Ogre::Real horiz_height = info->aspectRatio * mViewportAspectCoef * realCharHeight;

					// пересчет опорных данных
					left = right;
					right += horiz_height;

					// попали в символ, сравниваем с половиной ширины
					if (x < (1.0 + right)) {
						if (x < ((1.0 + right) - (horiz_height * 0.5)) ) {
							// в левой половине символа
							_point.left = (int)((1.0f + left) / (mPixelScaleX * 2.0));

						} else {
							// в правой половине символа
							position ++;
							_point.left = (int)((1.0f + right) / (mPixelScaleX * 2.0));
						}
						_point.top = (int)((1.0f - top) / (mPixelScaleY * 2.0));
						return position;
					}

					// следующий символ
					position ++;

				}
			}

			return position;
		}

		IntPoint getTextCursorFromPosition(size_t _position)
		{
			// если нуно обновить, или изменились пропорции экрана
			updateRawData();

			// позиция каретки
			size_t pos = _position;

			// для уменьшения умножений, поможем компилятору =)
			float realCharHeight = mCharHeight * 2.0;
			float realWidth = _getWidth() * 2.0;
			float realHeight = _getHeight() * 2.0;
			float realLeft = _getDerivedLeft() * 2.0;
			float realTop = _getDerivedTop() * 2.0;

			// опорное смещение вершин
			float right, left = realLeft - 1.0;
			float bottom, top = 1.0 - realTop;

			// сдвиг текста, если вью меньше или автоматическое выравнивание то сдвигаем по внутренним правилам
			float left_shift = 0;
			if (mContextSize.width <= realWidth) {
				if ( mAlignment == Right ) left_shift = mContextSize.width - realWidth; // выравнивание по правой стороне
				else if ( mAlignment == Center ) left_shift = (mContextSize.width - realWidth) * 0.5; // выравнивание по центру
			}
			else left_shift = mPixelScaleX * (float)mPointShift.left * 2.0;
			right = left;

			// сдвиг текста, если вью меньше или автоматическое выравнивание то сдвигаем по внутренним правилам
			if (mContextSize.height <= realHeight) {
				if ( m_align & ALIGN_BOTTOM ) top += (mContextSize.height - realHeight);
				else if ( !(m_align & ALIGN_TOP) ) top += (mContextSize.height - realHeight) * 0.5;
			}
			else top += mPixelScaleY * (float)mPointShift.top * 2.0;
			bottom = top;

			// основной цикл
			VectorLineInfo::iterator end = mLinesInfo.end();
			for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

				// пересчет опорных данных
				top = bottom;
				bottom -= realCharHeight;

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
				right = (realLeft - 1.0) - left_shift; // выравнивание по левой стороне
				if ( mAlignment == Right ) right += (mContextSize.width - len); // выравнивание по правой стороне
				else if ( mAlignment == Center ) right += (mContextSize.width - len) * 0.5; // выравнивание по центру

				// нашли, возвращаем
				if (pos == 0) return IntPoint((int)((1.0f + right) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));

				int currect_pos = 0;

				// внутренний цикл строки
				for (;index != end_index; ++index) {

					size_t data = (*index);
					// проверяем на смену цвета
					if ( (data & 0xFF000000) == 0xFF000000) continue;

					// отображаемый символ
					Font::GlyphInfo * info = (Font::GlyphInfo * )data;
					Ogre::Real horiz_height = info->aspectRatio * mViewportAspectCoef * realCharHeight;

					// пересчет опорных данных
					left = right;
					right += horiz_height;

					// нашли, возвращаем
					if (pos == currect_pos) return IntPoint((int)((1.0f + left) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));
					else if (pos == (currect_pos+1)) return IntPoint((int)((1.0f + right) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));

					// следующий символ
					currect_pos ++;

				}

			}

			// в самый конец
			return IntPoint((int)((1.0f + right) / (mPixelScaleX * 2.0)), (int)((1.0f - top) / (mPixelScaleY * 2.0)));
		}

	}; // class TextEditOverlayElement : public TextAreaOverlayElement

} // namespace MyGUI

#endif // __TEXT_EDIT_OVERLAY_ELEMENT_H__
