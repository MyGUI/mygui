/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_SimpleText.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_FontManager.h"

namespace MyGUI
{

	const size_t SIMPLETEXT_COUNT_VERTEX = 16 * VERTEX_IN_QUAD;

	SimpleText::SimpleText(const SubWidgetInfo &_info, CroppedRectanglePtr _parent) :
		SubWidgetTextInterface(_info.coord, _info.align, _parent),
		mTransparent(false),
		mRenderItem(null),
		mCurrentCoord(_info.coord),
		mCountVertex(SIMPLETEXT_COUNT_VERTEX),
		mTextOutDate(false),
		mCurrentColour(0xFFFFFFFF),
		mAlpha(ALPHA_MAX),
		mColour(Ogre::ColourValue::White),
		mFontHeight(16),
		mAspectCoef(0)
	{
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

	size_t SimpleText::_drawItem(Vertex * _vertex)
	{
		if (mFont.isNull()) return 0;
		if ((false == mShow) || (mTransparent)) return 0;
		if ((0 >= mCurrentCoord.width) || (0 >= mCurrentCoord.height)) return 0;
		if ((mAspectCoef != mRenderItem->getAspectCoef()) || mTextOutDate) updateRawData();

		return 0;

		/*float vertex_z = mRenderItem->getMaximumDepth();

		float vertex_left = ((mRenderItem->getPixScaleX() * (float)(mCurrentCoord.left + mParent->getAbsoluteLeft()) + mRenderItem->getHOffset()) * 2) - 1;
		float vertex_right = vertex_left + (mRenderItem->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float vertex_top = -(((mRenderItem->getPixScaleY() * (float)(mCurrentCoord.top + mParent->getAbsoluteTop()) + mRenderItem->getVOffset()) * 2) - 1);
		float vertex_bottom = vertex_top - (mRenderItem->getPixScaleY() * (float)mCurrentCoord.height * 2);

		// first triangle - left top
		_vertex[0].x = vertex_left;
		_vertex[0].y = vertex_top;
		_vertex[0].z = vertex_z;
		_vertex[0].colour = mCurrentColour;
		_vertex[0].u = mCurrentTexture.left;
		_vertex[0].v = mCurrentTexture.top;
		

		// first triangle - left bottom
		_vertex[1].x = vertex_left;
		_vertex[1].y = vertex_bottom;
		_vertex[1].z = vertex_z;
		_vertex[1].colour = mCurrentColour;
		_vertex[1].u = mCurrentTexture.left;
		_vertex[1].v = mCurrentTexture.bottom;

		// first triangle - right top
		_vertex[2].x = vertex_right;
		_vertex[2].y = vertex_top;
		_vertex[2].z = vertex_z;
		_vertex[2].colour = mCurrentColour;
		_vertex[2].u = mCurrentTexture.right;
		_vertex[2].v = mCurrentTexture.top;

		// second triangle - right top
		_vertex[3].x = vertex_right;
		_vertex[3].y = vertex_top;
		_vertex[3].z = vertex_z;
		_vertex[3].colour = mCurrentColour;
		_vertex[3].u = mCurrentTexture.right;
		_vertex[3].v = mCurrentTexture.top;

		// second triangle = left bottom
		_vertex[4].x = vertex_left;
		_vertex[4].y = vertex_bottom;
		_vertex[4].z = vertex_z;
		_vertex[4].colour = mCurrentColour;
		_vertex[4].u = mCurrentTexture.left;
		_vertex[4].v = mCurrentTexture.bottom;

		// second triangle - right botton
		_vertex[5].x = vertex_right;
		_vertex[5].y = vertex_bottom;
		_vertex[5].z = vertex_z;
		_vertex[5].colour = mCurrentColour;
		_vertex[5].u = mCurrentTexture.right;
		_vertex[5].v = mCurrentTexture.bottom;

		_count += mCountVertex;*/
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
		mFont = FontManager::getInstance().getByName( _font );
		if (mFont.isNull()) MYGUI_EXCEPT("Could not find font " << _font);
		mFont->load();
		mpMaterial = mFont->getMaterialSelectedFont();
		mpMaterial->setDepthCheckEnabled(false);
		mpMaterial->setLightingEnabled(false);

		// достаем пробел и табуляцию
		mSpaceGlyphInfo = mFont->getSpaceGlyphInfo();
		mTabGlyphInfo = mFont->getTabGlyphInfo();

		// достаем средние точки на текстуре для выделения текста
		Font::GlyphInfo * info = mSpaceGlyphInfo;
		mBackgroundEmpty.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));
		info = mFont->getSelectGlyphInfo();
		mBackgroundFill.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));
		info = mFont->getSelectDeactiveGlyphInfo();
		mBackgroundFillDeactive.set(info->uvRect.left + ((info->uvRect.right-info->uvRect.left)*0.5), info->uvRect.top + ((info->uvRect.bottom-info->uvRect.top)*0.5));

		mTextOutDate = true;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	const std::string & SimpleText::getFontName()
	{
		return mFont->getName();
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

	void SimpleText::updateRawData()
	{
		if (mFont.isNull()) return;
		if (false == mTextOutDate) return;
		mAspectCoef = mRenderItem->getAspectCoef();

		// массив для быстрой конвертации цветов
		static const char convert_colour[64] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0};

		// вычисление размера одной единицы в текстурных координатах
		float realCharHeight = mFontHeight * 2.0;
		Font::GlyphInfo * info = mFont->getGlyphInfo('A');
		mTextureHeightOne = (info->uvRect.bottom - info->uvRect.top) / (realCharHeight);
		mTextureWidthOne = (info->uvRect.right - info->uvRect.left) / (info->aspectRatio * mAspectCoef * realCharHeight);

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
			if (Font::FONT_CODE_SPACE == character) info = mFont->getSpaceGlyphInfo();
			else if (Font::FONT_CODE_TAB == character) info = mFont->getTabGlyphInfo();
			else info = mFont->getGlyphInfo(character);

			len += info->aspectRatio * realCharHeight * mAspectCoef;

			// указатель на инфо о символе
			mLinesInfo.back().push_back( EnumCharInfo(info) );
			count ++;

		}

		// запоминаем размер предыдущей строки
		mLinesInfo.back()[0] = EnumCharInfo(len);
		mLinesInfo.back()[1] = EnumCharInfo(count);
		if (width < len) width = len;


		// сбрасывам флаги
		mTextOutDate = false;

		// устанавливаем размер текста
		mContextSize.set(width, (float)mLinesInfo.size() * mFontHeight * 2.0);
	}

	void SimpleText::_createDrawItem(RenderItem * _item)
	{
		//mRenderItem = _item;
		//mRenderItem->addDrawItem(this, mCountVertex);
	}

	void SimpleText::_destroyDrawItem()
	{
		//mRenderItem->removeDrawItem(this, mCountVertex);
	}

} // namespace MyGUI
