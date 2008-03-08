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
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	/*const size_t SIMPLETEXT_COUNT_VERTEX = 32 * VERTEX_IN_QUAD;

	// рисковать не будем с инлайнами
	#define __MYGUI_DRAW_QUAD(buf, v_left, v_top, v_rignt, v_bottom, v_z, col, t_left, t_top, t_right, t_bottom, count) \
	{\
		buf[0].x = v_left; \
		buf[0].y = v_top; \
		buf[0].z = v_z; \
		buf[0].colour = col; \
		buf[0].u = t_left; \
		buf[0].v = t_top; \
		\
		buf[1].x = v_left; \
		buf[1].y = v_bottom; \
		buf[1].z = v_z; \
		buf[1].colour = col; \
		buf[1].u = t_left; \
		buf[1].v = t_bottom; \
		\
		buf[2].x = v_rignt; \
		buf[2].y = v_top; \
		buf[2].z = v_z; \
		buf[2].colour = col; \
		buf[2].u = t_right; \
		buf[2].v = t_top; \
		\
		buf[3].x = v_rignt; \
		buf[3].y = v_top; \
		buf[3].z = v_z; \
		buf[3].colour = col; \
		buf[3].u = t_right; \
		buf[3].v = t_top; \
		\
		buf[4].x = v_left; \
		buf[4].y = v_bottom; \
		buf[4].z = v_z; \
		buf[4].colour = col; \
		buf[4].u = t_left; \
		buf[4].v = t_bottom; \
		\
		buf[5].x = v_rignt; \
		buf[5].y = v_bottom; \
		buf[5].z = v_z; \
		buf[5].colour = col; \
		buf[5].u = t_right; \
		buf[5].v = t_bottom; \
		\
		buf += VERTEX_IN_QUAD; \
		count += VERTEX_IN_QUAD; \
	}*/


	SimpleText::SimpleText(const SubWidgetInfo &_info, CroppedRectanglePtr _parent) :
		EditText(_info, _parent)
		/*SubWidgetTextInterface(_info.coord, _info.align, _parent),
		mEmptyView(false),
		mCurrentCoord(_info.coord),
		mTextOutDate(false),
		mCurrentColour(0x00FFFFFF), mInverseColour(0x00000000),
		mCurrentAlpha(0xFF000000),
		mColour(Ogre::ColourValue::White),
		mAlpha(ALPHA_MAX),
		mFontHeight(16),
		//mAspectCoef(0),
		mCountVertex(SIMPLETEXT_COUNT_VERTEX),
		mItemKeeper(null),
		mRenderItem(null),
		mBackgroundNormal(true),
		mStartSelect(0), mEndSelect(0),
		mCursorPosition(0), mShowCursor(false)*/
	{
		//mManager = LayerManager::getInstancePtr();

		// потом перенести
		//mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());

		//mStartSelect = 0;
		//mEndSelect = 0;
		//mShowCursor = true;
		//mCursorPosition = 9;
	}

	SimpleText::~SimpleText()
	{
	}

	/*void SimpleText::show()
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

		mEmptyView = ((0 >= getViewWidth()) || (0 >= getViewHeight()));

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (_checkOutside()) {

				// скрываем
				//mEmptyView = true;
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
		//mEmptyView = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::setCaption(const Ogre::DisplayString & _caption)
	{
		mCaption = _caption;
		mTextOutDate = true;

		if (null != mRenderItem) {
			// если вершин не хватит, делаем реалок, с учетом выделения * 2 и курсора
			size_t need = (mCaption.size() * 2 + 2) * VERTEX_IN_QUAD;
			if (mCountVertex < need) {
				mCountVertex = need + SIMPLETEXT_COUNT_VERTEX;
				mRenderItem->reallockDrawItem(this, mCountVertex);
			}
			mRenderItem->outOfDate();
		}
	}

	const Ogre::DisplayString & SimpleText::getCaption()
	{
		return mCaption;
	}

	void SimpleText::setColour(const Ogre::ColourValue & _colour)
	{
		if (mColour == _colour) return;
		mColour = _colour;
		Ogre::Root::getSingleton().convertColourValue(_colour, &mCurrentColour);

		mCurrentColour = (mCurrentColour & 0x00FFFFFF) | mCurrentAlpha;
		mInverseColour = mCurrentColour ^ 0x00FFFFFF;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	const Ogre::ColourValue & SimpleText::getColour()
	{
		return mColour;
	}

	void SimpleText::setAlpha(float _alpha)
	{
		if (mAlpha == _alpha) return;
		mAlpha = _alpha;
		mCurrentAlpha = ((uint8)(mAlpha*255) << 24);
		mCurrentColour = (mCurrentColour & 0x00FFFFFF) | mCurrentAlpha;
		mInverseColour = mCurrentColour ^ 0x00FFFFFF;

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

		info = mpFont->getCursorGlyphInfo();
		mCursorTexture.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));

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

	size_t SimpleText::getSelectStart()
	{
		return mStartSelect;
	}

	size_t SimpleText::getSelectEnd()
	{
		return mEndSelect;
	}

	void SimpleText::setTextSelect(size_t _start, size_t _end)
	{
		mStartSelect=_start;
		mEndSelect=_end;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	bool SimpleText::getSelectBackground()
	{
		return mBackgroundNormal;
	}

	void SimpleText::setSelectBackground(bool _normal)
	{
		if (mBackgroundNormal == _normal) return;
		mBackgroundNormal = _normal;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	bool SimpleText::isCursorShow()
	{
		return mShowCursor;
	}

	void SimpleText::setShowCursor(bool _show)
	{
		if (mShowCursor == _show) return;
		mShowCursor = _show;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	size_t SimpleText::getCursorPosition()
	{
		return mCursorPosition;
	}

	void SimpleText::setCursorPosition(size_t _pos)
	{
		if (mCursorPosition == _pos) return;
		mCursorPosition = _pos;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SimpleText::setTextAlign(Align _align)
	{
		mTextAlign = _align;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	Align SimpleText::getTextAlign()
	{
		return mTextAlign;
	}

	IntSize SimpleText::getTextSize()
	{
		if (null == mRenderItem) return IntSize();
		// если нуно обновить, или изменились пропорции экрана
		if (mTextOutDate) updateRawData();
		return IntSize( (int)(mContextRealSize.width / (mManager->getPixScaleX() * 2.0)), (int)(mLinesInfo.size() * mFontHeight) );
	}

	IntSize SimpleText::getTextSize(const Ogre::DisplayString& _text)
	{
		if (mpFont.isNull() || (null == mRenderItem)) return IntSize();

		float len = 0, width = 0;
		int height = 1;

		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);

		Ogre::DisplayString::const_iterator end = _text.end();
		for (Ogre::DisplayString::const_iterator index=_text.begin(); index!=end; ++index) {

			Font::CodePoint character = MYGUI_DEREF_DISPLAYSTRING_ITERATOR(index);

			if (character == Font::FONT_CODE_CR || character == Font::FONT_CODE_NEL || character == Font::FONT_CODE_LF) {
				if (width < len) width = len;
				len = 0;
				height ++;

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
					// и еще пять символов после шарпа
					for (char i=0; i<5; i++) {
						++ index;
						if (index == end) {--index ;continue;} // это защита
					}
					continue;
				}
			}

			Font::GlyphInfo * info;
			if (Font::FONT_CODE_SPACE == character) info = mpFont->getSpaceGlyphInfo();
			else if (Font::FONT_CODE_TAB == character) info = mpFont->getTabGlyphInfo();
			else info = mpFont->getGlyphInfo(character);

			len += info->aspectRatio * mFontHeight;
		}

		if (width < len) width = len;

		// плюс ширина курсора
		return IntSize((int)width + 2, height * (int)mFontHeight);
	}

	void SimpleText::setViewOffset(IntPoint _point)
	{
		mViewOffset = _point;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}
		
	IntPoint SimpleText::getViewOffset()
	{
		return mViewOffset;
	}

	void SimpleText::updateRawData()
	{
		if (mpFont.isNull()) return;
		// сбрасывам флаги
		//mAspectCoef = mManager->getAspectCoef();
		mTextOutDate = false;

		// массив для быстрой конвертации цветов
		static const char convert_colour[64] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0};

		// вычисление размера одной единицы в текстурных координатах
		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);
		Font::GlyphInfo * info = mpFont->getGlyphInfo('A');
		mTextureHeightOne = (info->uvRect.bottom - info->uvRect.top) / (real_fontHeight);
		mTextureWidthOne = (info->uvRect.right - info->uvRect.left) / (info->aspectRatio * mManager->getAspectCoef() * real_fontHeight);

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

				// ширина курсора
				len += (mManager->getPixScaleX() * 2 * 2);

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

			len += info->aspectRatio * real_fontHeight * mManager->getAspectCoef();

			// указатель на инфо о символе
			mLinesInfo.back().push_back( EnumCharInfo(info) );
			count ++;

		}

		// ширина курсора
		len += (mManager->getPixScaleX() * 2 * 2);

		// запоминаем размер предыдущей строки
		mLinesInfo.back()[0] = EnumCharInfo(len);
		mLinesInfo.back()[1] = EnumCharInfo(count);
		if (width < len) width = len;


		// устанавливаем размер текста
		mContextRealSize.set(width, (float)mLinesInfo.size() * real_fontHeight);
	}

	size_t SimpleText::_drawItem(Vertex * _vertex, bool _update)
	{
		if (_update) mTextOutDate = true;

		if (mpFont.isNull()) return 0;
		if ((false == mShow) || (mEmptyView)) return 0;

		if (mTextOutDate) updateRawData();

		// колличество отрисованных вершин
		size_t vertex_count = 0;

		// позиция отображаемого символа
		size_t cursor = 0;

		// текущие цвета
		uint32 colour_current, colour = mCurrentColour;
		uint32 colour_inverse = mInverseColour;

		FloatPoint background(mBackgroundFill);
		if (false == mBackgroundNormal) background = mBackgroundFillDeactive;

		float vertex_z = mManager->getMaximumDepth();

		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);

		float real_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float real_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		float real_width = (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float real_height = (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);
		float real_right = real_left + real_width;
		float real_bottom = real_top - real_height;

		float margin_left = (mMargin.left * mManager->getPixScaleX() * 2);
		float margin_right = (mMargin.right * mManager->getPixScaleX() * 2);
		float margin_top = (mMargin.top * mManager->getPixScaleY() * 2);
		float margin_bottom = (mMargin.bottom * mManager->getPixScaleY() * 2);

		// опорное смещение вершин
		float left, right, top, bottom = real_top, left_shift = 0;

		// сдвиг текста, если вью меньше или автоматическое выравнивание то сдвигаем по внутренним правилам
		if ( IS_ALIGN_RIGHT(mTextAlign) ) left_shift = mContextRealSize.width - real_width; // выравнивание по правой стороне
		else if ( IS_ALIGN_HCENTER(mTextAlign) ) {
			left_shift = (mContextRealSize.width - real_width) * 0.5; // выравнивание по центру
			// выравниваем по  целому пикселю
			if (!((uint32)(mCurrentCoord.width - mMargin.left - mMargin.right)  & 0x01)) left_shift += mManager->getPixScaleX();
		}

		if ( IS_ALIGN_TOP(mTextAlign) ) 	bottom += margin_top;
		else if ( IS_ALIGN_BOTTOM(mTextAlign) ) bottom += mContextRealSize.height - real_height - margin_bottom;
		else if ( IS_ALIGN_VCENTER(mTextAlign) ) {
			bottom += (margin_top - margin_bottom + mContextRealSize.height - real_height) * 0.5;
			// выравниваем по  целому пикселю
			if (!((uint32)(mCurrentCoord.height - mMargin.top - mMargin.bottom)  & 0x01)) bottom += mManager->getPixScaleY();
		}

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
			size_t count = index->getValueSizeT();
			++index;

			// нуна ли пересчитывать текстурные координаты
			bool texture_crop_height = false;

			if (vertex_top > real_top) {
				// проверка на полный выход
				if (vertex_bottom > real_top) {

					// необходимо парсить теги цветов полюбак
					for (;index != end_index; ++index) {
						// проверяем на смену цвета
						if ( index->isColour() ) {
							colour = index->getColour() | (colour & 0xFF000000);
							colour_inverse = colour ^ 0x00FFFFFF;
						}
					}

					cursor += count;
					continue;
				}
				// обрезаем
				vertex_top = real_top;
				texture_crop_height = true;
			}
			if (vertex_bottom < real_bottom) {
				// вообще вниз ушли
				if (vertex_top < real_bottom) {
					line = end;
					line --;
					cursor += count;
					continue;
				}
				// обрезаем
				vertex_bottom = real_bottom;
				texture_crop_height = true;
			}

			// пересчет опорных данных
			if ( IS_ALIGN_LEFT(mTextAlign) ) right = real_left - left_shift - margin_left; // выравнивание по левой стороне
			else if ( IS_ALIGN_RIGHT(mTextAlign) ) right = real_left - left_shift + (mContextRealSize.width - len) + margin_right; // выравнивание по правой стороне
			else right = real_left - left_shift + (((mContextRealSize.width - len) - margin_left + margin_right) * 0.5); // выравнивание по центру

			// текущее положение в строке
			size_t cur = cursor;

			// внутренний цикл строки
			for (;index != end_index; ++index) {

				// проверяем на смену цвета
				if ( index->isColour() ) {
					colour = index->getColour() | (colour & 0xFF000000);
					colour_inverse = colour ^ 0x00FFFFFF;
					continue;
				}

				// отображаемый символ
				Font::GlyphInfo * info = index->getGlyphInfo();
				float horiz_height = info->aspectRatio * real_fontHeight * mManager->getAspectCoef();

				// пересчет опорных данных
				left = right;
				right += horiz_height;

				// смещение текстуры для фона
				FloatPoint background_current;
				bool select = !( (cur >= mEndSelect) || (cur < mStartSelect) );

				// символ не выделен
				if (false == select) {

					// если пробел или табуляция то не рисуем
					//if ( (info->codePoint == Font::FONT_CODE_SPACE) || (info->codePoint == Font::FONT_CODE_TAB) ) {
					//	cur ++;
					//	continue;
					//}

					colour_current = colour;
					background_current = mBackgroundEmpty;
				}
				// символ выделен
				else {
					// инверсные цвета
					colour_current = colour_inverse;
					background_current = background;
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

				if (vertex_left < real_left) {
					// проверка на полный выход
					if (vertex_right < real_left) {
						cur ++;
						continue;
					}
					// обрезаем
					vertex_left = real_left;
					texture_crop_width = true;
				}
				if (vertex_right > real_right) {
					// вообще строку до конца не нуна
					if (vertex_left > real_right) {
						index ++;
						// для того чтобы теги цвета не терялись, нужно пройти до конца строки
						while (index != end_index) {
							// проверяем на смену цвета
							if ( index->isColour() ) {
								colour = index->getColour() | (colour & 0xFF000000);
								colour_inverse = colour ^ 0x00FFFFFF;
							}
							index ++;
						};
						index --; // чтобы при увеличении показывал на end
						continue;
					}
					// обрезаем
					vertex_right = real_right;
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

				// если нужно рисуем выделение
				if (select) {
					__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
						background_current.left, background_current.top, background_current.left, background_current.top, vertex_count);
				}

				__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
					texture_left, texture_top, texture_right, texture_bottom, vertex_count);

				cur ++;

				if (mShowCursor) {
					if (cur-1 != mCursorPosition) {
						if ((cur == mCursorPosition) && (cur+1 == cursor + count)) {
							if (vertex_right != right) continue;
							if ((vertex_right + (mManager->getPixScaleX() * 4)) > real_right) continue;
							vertex_left = vertex_right;
						}
						else continue;
					}
					else {
						if (vertex_left != left) continue;
						if (vertex_right < (left + (mManager->getPixScaleX() * 4))) continue;
					}

					vertex_right = vertex_left + (mManager->getPixScaleX() * 2);

					// первая половинка белая
					colour_current |= 0x00FFFFFF;

					__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
						mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);

					vertex_left = vertex_right;
					vertex_right += (mManager->getPixScaleX() * 2);

					// вторая половинка черная
					colour_current = colour_current & 0xFF000000;

					__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
						mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);
					
				}

			}

			// следующая строка
			cursor += count;
		}

		// пустая строка, нужно показать курсор
		if (mShowCursor && (1 == cursor)) {
			if ((right >= real_left) && ((right + (mManager->getPixScaleX() * 4)) <= real_right)) {
				vertex_left = right;
				vertex_right = vertex_left + (mManager->getPixScaleX() * 2);

				// первая половинка белая
				colour_current |= 0x00FFFFFF;

				__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
					mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);

				vertex_left = vertex_right;
				vertex_right += (mManager->getPixScaleX() * 2);

				// вторая половинка черная
				colour_current = colour_current & 0xFF000000;

				__MYGUI_DRAW_QUAD(_vertex, vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z, colour_current, 
					mCursorTexture.left, mCursorTexture.top, mCursorTexture.left, mCursorTexture.top, vertex_count);
			}
		}

		return vertex_count;
	}*/

} // namespace MyGUI
