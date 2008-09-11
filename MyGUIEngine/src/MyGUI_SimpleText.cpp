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

	SimpleText::SimpleText(const SubWidgetInfo &_info, CroppedRectangleInterface * _parent) :
		EditText(_info, _parent)
	{
		mManualView = false;
	}

	SimpleText::~SimpleText()
	{
	}

	void SimpleText::setViewOffset(IntPoint _point)
	{
	}

	void SimpleText::updateRawData()
	{
		if (mpFont.isNull()) return;
		// сбрасывам флаги
		mTextOutDate = false;

		// массив для быстрой конвертации цветов
		static const char convert_colour[64] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0};

		// вычисление размера одной единицы в текстурных координатах
		float real_fontHeight = (mManager->getPixScaleY() * (float)mFontHeight * 2.0f);//???
		Font::GlyphInfo * info = mpFont->getGlyphInfo('A');
		mTextureHeightOne = (info->uvRect.bottom - info->uvRect.top) / (real_fontHeight);
		mTextureWidthOne = (info->uvRect.right - info->uvRect.left) / (info->aspectRatio * mManager->getAspectCoef() * real_fontHeight);

		mLinesInfo.clear();

		// создаем первую строчку
		mLinesInfo.push_back(PairVectorCharInfo());
		//mLinesInfo.back().second.push_back(EnumCharInfo()); // первый символ всегда ширина в реальных координатах
		//mLinesInfo.back().second.push_back(EnumCharInfo()); // второй символ всегда ширина в пикселях
		//mLinesInfo.back().second.push_back(EnumCharInfo()); // третий символ, колличество значимых символов
		float len = 0, width = 0;
		size_t count = 1;

		Ogre::UTFString::const_iterator end = mCaption.end();
		for (Ogre::UTFString::const_iterator index=mCaption.begin(); index!=end; ++index) {

			Font::CodePoint character = *index;

			if (character == Font::FONT_CODE_CR || character == Font::FONT_CODE_NEL || character == Font::FONT_CODE_LF) {

				// длинна строки, кратна пикселю, плюс курсор
				len = (float)((uint)(len + 0.99f));

				// запоминаем размер предыдущей строки
				//mLinesInfo.back().second[0] = EnumCharInfo(len * mManager->getPixScaleX() * 2.0f);
				//mLinesInfo.back().second[1] = EnumCharInfo((size_t)len);
				//mLinesInfo.back().second[2] = EnumCharInfo(count);
				mLinesInfo.back().first.set(count, (size_t)len, len * mManager->getPixScaleX() * 2.0f);

				if (width < len) width = len;
				count = 1;
				len = 0;

				// и создаем новую
				mLinesInfo.push_back(PairVectorCharInfo());
				//mLinesInfo.back().second.push_back(EnumCharInfo()); // первый символ всегда ширина в реальных координатах
				//mLinesInfo.back().second.push_back(EnumCharInfo()); // второй символ всегда ширина в пикселях
				//mLinesInfo.back().second.push_back(EnumCharInfo()); // третий символ, колличество значимых символов

				if (character == Font::FONT_CODE_CR) {
					Ogre::UTFString::const_iterator peeki = index;
					peeki++;
					if ((peeki != end) && (*peeki == Font::FONT_CODE_LF)) index = peeki; // skip both as one newline
				}
				// следующий символ
				continue;

			}
			else if (character == L'#') {
				// берем следующий символ
				++ index;
				if (index == end) {--index ;continue;} // это защита

				character = *index;
				// если два подряд, то рисуем один шарп, если нет то меняем цвет
				if (character != L'#') {

					// парсим первый символ
					Ogre::RGBA colour = convert_colour[(character-48) & 0x3F];

					// и еще пять символов после шарпа
					for (char i=0; i<5; i++) {
						++ index;
						if (index == end) {--index ;continue;} // это защита
						colour <<= 4;
						colour += convert_colour[ ((*index) - 48) & 0x3F];
					}

					// если нужно, то меняем красный и синий компоненты
					if (mRenderGL) colour = ((colour&0x00FF0000)>>16)|((colour&0x000000FF)<<16)|(colour&0xFF00FF00);

					// запоминаем цвет, в верхнем байте единицы
					mLinesInfo.back().second.push_back(EnumCharInfo(colour, true) );

					continue;
				}
			}

			Font::GlyphInfo * info;
			if (Font::FONT_CODE_SPACE == character) info = mpFont->getSpaceGlyphInfo();
			else if (Font::FONT_CODE_TAB == character) info = mpFont->getTabGlyphInfo();
			else info = mpFont->getGlyphInfo(character);

			len += info->aspectRatio * (float)mFontHeight;

			// указатель на инфо о символе
			mLinesInfo.back().second.push_back( EnumCharInfo(info) );
			count ++;

		}

		// длинна строки, кратна пикселю
		len = (float)((uint)(len + 0.99f));

		// запоминаем размер предыдущей строки
		//mLinesInfo.back().second[0] = EnumCharInfo(len * mManager->getPixScaleX() * 2.0f);
		//mLinesInfo.back().second[1] = EnumCharInfo((size_t)len);
		//mLinesInfo.back().second[2] = EnumCharInfo(count);
		mLinesInfo.back().first.set(count, (size_t)len, len * mManager->getPixScaleX() * 2.0f);

		if (width < len) width = len;


		// устанавливаем размер текста
		mContextSize.set(width, (float)mLinesInfo.size() * mFontHeight);
		mContextRealSize.set(mContextSize.width * mManager->getPixScaleX() * 2.0f, mContextSize.height  * mManager->getPixScaleY() * 2.0f);
	}

	void * SimpleText::createStateData(xml::xmlNodePtr _node, xml::xmlNodePtr _root)
	{
		return EditText::createStateData(_node, _root);
	}

} // namespace MyGUI
