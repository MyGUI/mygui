/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_SimpleText.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_FontManager.h"

namespace MyGUI
{

	const size_t SIMPLETEXT_COUNT_VERTEX = 116 * VERTEX_IN_QUAD;//???

	SimpleText::SimpleText(const SubWidgetInfo &_info, CroppedRectanglePtr _parent) :
		SubWidgetTextInterface(_info.coord, _info.align, _parent),
		mTransparent(false),
		mCurrentCoord(_info.coord),
		mTextOutDate(false),
		mCurrentColour(0xFFFFFFFF),
		mAlpha(ALPHA_MAX),
		mColour(Ogre::ColourValue::White),
		mFontHeight(16),
		mAspectCoef(0),
		mCountVertex(SIMPLETEXT_COUNT_VERTEX),
		mItemKeeper(null),
		mRenderItem(null)
	{
		mCaption = "test";//..........................";//???
		mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());
	}

	SimpleText::~SimpleText()
	{
	}

	void SimpleText::show()
	{
		if (mShow) return;
		mShow = true;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::hide()
	{
		if (false == mShow) return;
		mShow = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::_correctView()
	{
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::_setAlign(const IntCoord& _coord, bool _update)
	{
		_setAlign(_coord.size(), _update);
	}

	void SimpleText::_setAlign(const IntSize& _size, bool _update)
	{

		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание
		if (IS_ALIGN_RIGHT(mAlign)) {
			if (IS_ALIGN_LEFT(mAlign)) {
				// растягиваем
				mCoord.width = mCoord.width + (mParent->getWidth() - _size.width);
				need_update = true;
				mIsMargin = true; // при изменении размеров все пересчитывать
			}
			else {
				// двигаем по правому краю
				mCoord.left = mCoord.left + (mParent->getWidth() - _size.width);
				need_update = true;
			}

		}
		else if (false == IS_ALIGN_LEFT(mAlign)) {
			// выравнивание по горизонтали без растяжения
			mCoord.left = (mParent->getWidth() - mCoord.width) / 2;
			need_update = true;
		}

		if (IS_ALIGN_BOTTOM(mAlign)) {
			if (IS_ALIGN_TOP(mAlign)) {
				// растягиваем
				mCoord.height = mCoord.height + (mParent->getHeight() - _size.height);
				need_update = true;
				mIsMargin = true; // при изменении размеров все пересчитывать
			}
			else {
				mCoord.top = mCoord.top + (mParent->getHeight() - _size.height);
				need_update = true;
			}
		}
		else if (false == IS_ALIGN_TOP(mAlign)) {
			// выравнивание по вертикали без растяжения
			mCoord.top = (mParent->getHeight() - mCoord.height) / 2;
			need_update = true;
		}

		if (need_update) {
			mCurrentCoord = mCoord;
			_updateView();
		}

	}

	void SimpleText::_updateView()
	{
		bool margin = _checkMargin();

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (_checkOutside()) {

				// скрываем
				if (false == mTransparent) mTransparent = true;
				// запоминаем текущее состояние
				mIsMargin = margin;

				// обновить перед выходом
				if (null != mRenderItem) mRenderItem->outOfDate();
				return;

			}
		}

		if ((mIsMargin) || (margin)) { // мы обрезаны или были обрезаны
			mCurrentCoord.width = getViewWidth();
			mCurrentCoord.height = getViewHeight();

		}

		// запоминаем текущее состояние
		mIsMargin = margin;

		// если скин был скрыт, то покажем
		if (mTransparent) mTransparent = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::setTextAlign(Align _align)
	{
		mTextAlign = _align;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::setCaption(const Ogre::DisplayString & _caption)
	{
		mCaption = _caption;
		mTextOutDate = true;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	const Ogre::DisplayString & SimpleText::getCaption()
	{
		return mCaption;
	}

	void SimpleText::setColour(const Ogre::ColourValue & _colour)
	{
		mColour = _colour;
		mCurrentColour = ((uint8)(mAlpha*255) << 24) | ((uint8)(mColour.r*255) << 16) | ((uint8)(mColour.g*255) << 8) | (uint8)(mColour.b*255);
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	const Ogre::ColourValue & SimpleText::getColour()
	{
		return mColour;
	}

	void SimpleText::setAlpha(float _alpha)
	{
		mAlpha = _alpha;
		mCurrentColour = ((uint8)(mAlpha*255) << 24) | ((uint8)(mColour.r*255) << 16) | ((uint8)(mColour.g*255) << 8) | (uint8)(mColour.b*255);
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	float SimpleText::getAlpha()
	{
		return mAlpha;
	}

	void SimpleText::setFontName(const std::string & _font)
	{
		mpFont = FontManager::getInstance().getByName( _font );
		if (mpFont.isNull()) MYGUI_EXCEPT("Could not find font " << _font);
		mpFont->load();
		mpTexture = mpFont->getTextureFont();

		// достаем пробел и табуляцию
		mSpaceGlyphInfo = mpFont->getSpaceGlyphInfo();
		mTabGlyphInfo = mpFont->getTabGlyphInfo();

		// достаем средние точки на текстуре для выделения текста
		Font::GlyphInfo * info = mSpaceGlyphInfo;
		mBackgroundEmpty.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));
		info = mpFont->getSelectGlyphInfo();
		mBackgroundFill.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));
		info = mpFont->getSelectDeactiveGlyphInfo();
		mBackgroundFillDeactive.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));

		mTextOutDate = true;

		// если мы были приаттаченны, то удаляем себя
		if (null != mRenderItem) {
			mRenderItem->removeDrawItem(this);
			mRenderItem = null;
		}

		// если есть текстура, то приаттачиваемся
		if ((false == mpTexture.isNull()) && (null != mItemKeeper)) {
			mRenderItem = mItemKeeper->addToRenderItem(mpTexture->getName(), false, false);
			mRenderItem->addDrawItem(this, mCountVertex);
		}

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	const std::string & SimpleText::getFontName()
	{
		return mpFont->getName();
	}

	void SimpleText::setFontHeight(uint16 _height)
	{
		mFontHeight = _height;
		mTextOutDate = true;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	uint16 SimpleText::getFontHeight()
	{
		return mFontHeight;
	}

	IntSize SimpleText::getTextSize()
	{
		return IntSize(); //???
	}

	IntSize SimpleText::getTextSize(const Ogre::DisplayString& _text)
	{
		return IntSize(); //???
	}

	void SimpleText::_createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item)
	{
		mItemKeeper = _keeper;

		// если уже есть текстура, то атачимся, актуально для смены леера
		if (false == mpTexture.isNull()) {
			mRenderItem = mItemKeeper->addToRenderItem(mpTexture->getName(), false, false);
			mRenderItem->addDrawItem(this, mCountVertex);
		}
	}

	void SimpleText::_destroyDrawItem()
	{
		if (null != mRenderItem) {
			mRenderItem->removeDrawItem(this);
			mRenderItem = null;
		}
		mItemKeeper = null;
	}

	void SimpleText::updateRawData()
	{
		if (mpFont.isNull()) return;
		// сбрасывам флаги
		mAspectCoef = mRenderItem->getAspectCoef();
		mTextOutDate = false;

		// массив для быстрой конвертации цветов
		static const char convert_colour[64] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0};

		// вычисление размера одной единицы в текстурных координатах
		float real_fontHeight = (mRenderItem->getPixScaleY() * (float)mFontHeight * 2.0f);
		Font::GlyphInfo * info = mpFont->getGlyphInfo('A');
		mTextureHeightOne = (info->uvRect.bottom - info->uvRect.top) / (real_fontHeight);
		mTextureWidthOne = (info->uvRect.right - info->uvRect.left) / (info->aspectRatio * mAspectCoef * real_fontHeight);

		mLinesInfo.clear();

		// создаем первую строчку
		mLinesInfo.push_back(VectorCharInfo());
		mLinesInfo.back().push_back(EnumCharInfo()); // первый символ всегда ширина в реальных координатах
		mLinesInfo.back().push_back(EnumCharInfo()); // второй символ, колличество значимых символов
		float len = 0, width = 0;
		size_t count = 1;

		Ogre::DisplayString::const_iterator end = mCaption.end();
		for (Ogre::DisplayString::const_iterator index=mCaption.begin(); index!=end; ++index) {

			Font::CodePoint character = MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index);

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
					if (peeki != end && MYGUI_DEREF_DISPLAYSTRING_ITERATOR(peeki) == Font::FONT_CODE_LF) index = peeki; // skip both as one newline
				}
				// следующий символ
				continue;

			} else if (character == _T('#')) {
				// берем следующий символ
				++ index;
				if (index == end) {--index ;continue;} // это защита

				character = MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index);
				// если два подряд, то рисуем один шарп, если нет то меняем цвет
				if (character != _T('#')) {

					// парсим первый символ
					Ogre::RGBA colour = convert_colour[(character-48) & 0x3F];

					// и еще пять символов после шарпа
					for (char i=0; i<5; i++) {
						++ index;
						if (index == end) {--index ;continue;} // это защита
						colour <<= 4;
						colour += convert_colour[ (MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index)-48) & 0x3F];
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

			len += info->aspectRatio * real_fontHeight * mAspectCoef;

			// указатель на инфо о символе
			mLinesInfo.back().push_back( EnumCharInfo(info) );
			count ++;

		}

		// запоминаем размер предыдущей строки
		mLinesInfo.back()[0] = EnumCharInfo(len);
		mLinesInfo.back()[1] = EnumCharInfo(count);
		if (width < len) width = len;


		// устанавливаем размер текста
		mContextRealSize.set(width, (float)mLinesInfo.size() * real_fontHeight);

	}



	size_t SimpleText::_drawItem(Vertex * _vertex)
	{
		if (mpFont.isNull()) return 0;
		if ((false == mShow) || (mTransparent)) return 0;
		if ((0 >= mCurrentCoord.width) || (0 >= mCurrentCoord.height)) return 0;
		if ((mAspectCoef != mRenderItem->getAspectCoef()) || mTextOutDate) updateRawData();

		size_t vertex_count = 0;

		// текущие цвета
		Ogre::RGBA colour = mCurrentColour;

		float vertex_z = mRenderItem->getMaximumDepth();

		float real_fontHeight = (mRenderItem->getPixScaleY() * (float)mFontHeight * 2.0f);

		float real_left = ((mRenderItem->getPixScaleX() * (float)(mCurrentCoord.left + mParent->getAbsoluteLeft()) + mRenderItem->getHOffset()) * 2) - 1;
		float real_top = -(((mRenderItem->getPixScaleY() * (float)(mCurrentCoord.top + mParent->getAbsoluteTop()) + mRenderItem->getVOffset()) * 2) - 1);

		float real_width = (mRenderItem->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float real_height = (mRenderItem->getPixScaleY() * (float)mCurrentCoord.height * 2);

		float real_right = real_left + real_width;
		float real_bottom = real_top - real_height;

		float left_margin = real_left + (mMargin.left * mRenderItem->getPixScaleX() * 2);
		float right_margin = real_right;
		float top_margin = real_top;
		float bottom_margin = real_bottom;

		// опорное смещение вершин
		float left = real_left, right = 0;
		float top = real_top, bottom = 0;

		mTextAlign = ALIGN_LEFT | ALIGN_VCENTER;

		// сдвиг текста, если вью меньше или автоматическое выравнивание то сдвигаем по внутренним правилам
		float left_shift = 0;
		if ( IS_ALIGN_RIGHT(mTextAlign) ) left_shift = mContextRealSize.width - real_width; // выравнивание по правой стороне
		else if ( IS_ALIGN_HCENTER(mTextAlign) ) left_shift = (mContextRealSize.width - real_width) * 0.5; // выравнивание по центру
		right = left;

		if ( IS_ALIGN_BOTTOM(mTextAlign) ) top += (mContextRealSize.height - real_height);
		else if ( IS_ALIGN_VCENTER(mTextAlign) ) top += (mContextRealSize.height - real_height) * 0.5;
		bottom = top;

		// данные непосредственно для вывода
		float vertex_top, vertex_bottom, vertex_left, vertex_right;

		// основной цикл
		VectorLineInfo::iterator end = mLinesInfo.end();
		for (VectorLineInfo::iterator line = mLinesInfo.begin(); line != end; ++line) {

			// пересчет опорных данных
			top = bottom;
			bottom -= real_fontHeight;
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
			right = real_left - left_shift; // выравнивание по левой стороне
			if ( IS_ALIGN_RIGHT(mTextAlign) ) right += (mContextRealSize.width - len); // выравнивание по правой стороне
			else if ( IS_ALIGN_HCENTER(mTextAlign) ) right += (mContextRealSize.width - len) * 0.5; // выравнивание по центру

			// внутренний цикл строки
			for (;index != end_index; ++index) {

				// проверяем на смену цвета
				if ( index->isColour() ) {
					colour = index->getColour() | (colour & 0xFF000000);
					continue;
				}

				// отображаемый символ
				Font::GlyphInfo * info = index->getGlyphInfo();
				float horiz_height = info->aspectRatio * real_fontHeight * mAspectCoef;

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

				// first triangle - left top
				_vertex[0].x = vertex_left;
				_vertex[0].y = vertex_top;
				_vertex[0].z = vertex_z;
				_vertex[0].colour = colour;
				_vertex[0].u = texture_left;
				_vertex[0].v = texture_top;

				// first triangle - left bottom
				_vertex[1].x = vertex_left;
				_vertex[1].y = vertex_bottom;
				_vertex[1].z = vertex_z;
				_vertex[1].colour = colour;
				_vertex[1].u = texture_left;
				_vertex[1].v = texture_bottom;

				// first triangle - right top
				_vertex[2].x = vertex_right;
				_vertex[2].y = vertex_top;
				_vertex[2].z = vertex_z;
				_vertex[2].colour = colour;
				_vertex[2].u = texture_right;
				_vertex[2].v = texture_top;

				// second triangle - right top
				_vertex[3].x = vertex_right;
				_vertex[3].y = vertex_top;
				_vertex[3].z = vertex_z;
				_vertex[3].colour = colour;
				_vertex[3].u = texture_right;
				_vertex[3].v = texture_top;

				// second triangle = left bottom
				_vertex[4].x = vertex_left;
				_vertex[4].y = vertex_bottom;
				_vertex[4].z = vertex_z;
				_vertex[4].colour = colour;
				_vertex[4].u = texture_left;
				_vertex[4].v = texture_bottom;

				// second triangle - right botton
				_vertex[5].x = vertex_right;
				_vertex[5].y = vertex_bottom;
				_vertex[5].z = vertex_z;
				_vertex[5].colour = colour;
				_vertex[5].u = texture_right;
				_vertex[5].v = texture_bottom;

				// меняем указатель и колличество отрисованных вершин
				_vertex += VERTEX_IN_QUAD;
				vertex_count += VERTEX_IN_QUAD;

			}
		}

		return vertex_count;
	}

} // namespace MyGUI
