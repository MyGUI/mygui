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

		// вспомогательный класс для хранения информации о символе
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

		IntRect mMargin; // перекрытие
		Align mAlign;

		bool mRenderGL;// для конвертирования цвета вершин
		Ogre::RGBA mDefaultColour; // цвет текста
		FloatSize mContextSize; // размер всего текста
		VectorLineInfo mLinesInfo;
		bool mRawDataOutOfDate;
		float mOldViewportAspectCoef; // последний коээфициент
		float mTextureHeightOne, mTextureWidthOne; // размер одной текстурной единицы
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
			// для конвертирования цвета вершин
			mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());
		}

		// обязательно перекрываем, а то он там буферов понасоздает
		void updateColours(void) { }

		// необходимо обновить все что связанно с стекстом
		inline void setAlignment(Align _align)	
		{
			// выравнивание бокса
			mAlign = _align;
			// выравнивание строк внутри бокса
			if (IS_ALIGN_RIGHT(_align)) mAlignment = Right;
			else if (false == IS_ALIGN_LEFT(_align)) mAlignment = Center;
			else mAlignment = Left;
			mGeomPositionsOutOfDate = true;
		}

		// устанавливет размеры по которым резать текст
		inline void setMargin(const IntRect& _margin)
		{
			mMargin = _margin;
			mDerivedOutOfDate = true;
		}

		virtual void updatePositionGeometry()
		{
			// нет шрифта, не вечеринки
			if (mpFont.isNull()) return;

			// если нуно обновить, или изменились пропорции экрана
			updateRawData();

			// текущие цвета
			Ogre::RGBA colour = mDefaultColour;

			checkMemoryAllocation( mCaption.size() );
			mRenderOp.vertexData->vertexCount = 0;

			// Get position / texcoord buffer
			Ogre::HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(OVERLAY_POSITION_BINDING);
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
			float left_margin = (mPixelScaleX * (float)mMargin.left * 2.0) + left;
			float top_margin = top - (mPixelScaleY * (float)mMargin.top * 2.0);
			float right_margin = (left + realWidth) - (mPixelScaleX * (float)mMargin.right * 2.0);
			float bottom_margin = (top - realHeight) + (mPixelScaleY * (float)mMargin.bottom * 2.0);

			// сдвиг текста, если вью меньше или автоматическое выравнивание то сдвигаем по внутренним правилам
			float left_shift = 0;
			if ( mAlignment == Right ) left_shift = mContextSize.width - realWidth; // выравнивание по правой стороне
			else if ( mAlignment == Center ) left_shift = (mContextSize.width - realWidth) * 0.5; // выравнивание по центру
			right = left;

			if ( mAlign & ALIGN_BOTTOM ) top += (mContextSize.height - realHeight);
			else if ( !(IS_ALIGN_TOP(mAlign)) ) top += (mContextSize.height - realHeight) * 0.5;
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
				float len = index->getValueFloat();
				++index;
				// второй колличество символов
				++index;

				// нуна ли пересчитывать текстурные координаты
				bool texture_crop_height = false;

				if (vertex_top > top_margin) {
					// проверка на полный выход
					if (vertex_bottom > top_margin) {

						// необходимо парсить теги цветов полюбак
						for (;index != end_index; ++index) {
							// проверяем на смену цвета
							if ( index->isColour() ) {
								colour = index->getColour() | (colour & 0xFF000000);
							}
						}

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

				// внутренний цикл строки
				for (;index != end_index; ++index) {

					// проверяем на смену цвета
					if ( index->isColour() ) {
						colour = index->getColour() | (colour & 0xFF000000);
						continue;
					}

					// отображаемый символ
					Font::GlyphInfo * info = index->getGlyphInfo();
					Ogre::Real horiz_height = info->aspectRatio * mViewportAspectCoef * realCharHeight;

					// пересчет опорных данных
					left = right;
					right += horiz_height;

					// если пробел или табуляция то не рисуем
					if ( (info->codePoint == Font::FONT_CODE_SPACE) || (info->codePoint == Font::FONT_CODE_TAB) ) continue;

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
						if (vertex_right < left_margin) continue;
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
								// проверяем на смену цвета
								if ( index->isColour() ) {
									colour = index->getColour() | (colour & 0xFF000000);
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

			// достаем пробел и табуляцию
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

			// массив для быстрой конвертации цветов
			static const char convert_colour[128] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

			// вычисление размера одной единицы в текстурных координатах
			float realCharHeight = mCharHeight * 2.0;
			Font::GlyphInfo * info = mpFont->getGlyphInfo('A');
			mTextureHeightOne = (info->uvRect.bottom - info->uvRect.top) / (realCharHeight);
			mTextureWidthOne = (info->uvRect.right - info->uvRect.left) / (info->aspectRatio * mViewportAspectCoef * realCharHeight);

			mLinesInfo.clear();

			// создаем первую строчку
			mLinesInfo.push_back(VectorCharInfo());
			mLinesInfo.back().push_back(EnumCharInfo()); // первый символ всегда ширина в реальных координатах
			mLinesInfo.back().push_back(EnumCharInfo()); // второй символ, колличество значимых символов
			float len = 0, width = 0;
			size_t count = 1;

			Ogre::DisplayString::const_iterator end = mCaption.end();
			for (Ogre::DisplayString::const_iterator index=mCaption.begin(); index!=end; ++index) {

				Font::CodePoint character = OGRE_DEREF_DISPLAYSTRING_ITERATOR(index);

				if (character == Font::FONT_CODE_CR || character == Font::FONT_CODE_NEL || character == Font::FONT_CODE_LF) {
					// запоминаем размер предыдущей строки
					mLinesInfo.back()[0] = EnumCharInfo(len);
					mLinesInfo.back()[1] = EnumCharInfo(count);
					if (width < len) width = len;
					count = 1;
					len = 0;

					// и создаем новую
					mLinesInfo.push_back(VectorCharInfo());
					mLinesInfo.back().push_back(EnumCharInfo()); // первый символ всегда ширина в пикселях
					mLinesInfo.back().push_back(EnumCharInfo()); // второй символ, колличество значимых символов

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
						Ogre::RGBA colour = convert_colour[character & 0x7F];

						// и еще пять символов после шарпа
						for (char i=0; i<5; i++) {
							++ index;
							if (index == end) {--index ;continue;} // это защита
							colour <<= 4;
							colour += convert_colour[ OGRE_DEREF_DISPLAYSTRING_ITERATOR(index) & 0x7F];
						}

						// если нужно, то меняем красный и синий компоненты
						if (mRenderGL) colour = ((colour&0x00FF0000)>>16)|((colour&0x000000FF)<<16)|(colour&0xFF00FF00);
				
						// запоминаем цвет, в верхнем байте единицы
						mLinesInfo.back().push_back( EnumCharInfo(colour, true) );

						continue;
					}
				}

				Font::GlyphInfo * info;
				if (Font::FONT_CODE_SPACE == character) info = mpFont->getSpaceGlyphInfo();
				else if (Font::FONT_CODE_TAB == character) info = mpFont->getTabGlyphInfo();
				else info = mpFont->getGlyphInfo(character);

				len += info->aspectRatio * realCharHeight * mViewportAspectCoef;

				// указатель на инфо о символе
				mLinesInfo.back().push_back( EnumCharInfo(info) );
				count ++;

			}

			// запоминаем размер предыдущей строки
			mLinesInfo.back()[0] = EnumCharInfo(len);
			mLinesInfo.back()[1] = EnumCharInfo(count);
			if (width < len) width = len;


			// сбрасывам флаги
			mRawDataOutOfDate = false;
			mOldViewportAspectCoef = mViewportAspectCoef;

			// устанавливаем размер текста
			mContextSize.set(width, (float)mLinesInfo.size() * mCharHeight * 2.0);

		} // void updateRawData()

	}; // class TextSimpleOverlayElement : public TextAreaOverlayElement

} // namespace MyGUI

#endif // __MYGUI_TEXT_SIMPLE_OVERLAY_ELEMENT_H__