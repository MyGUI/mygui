/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_SimpleText.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_CommonStateInfo.h"
#include "MyGUI_RenderManager.h"

namespace MyGUI
{

	SimpleText::SimpleText() :
		EditText()
	{
	}

	SimpleText::~SimpleText()
	{
	}

	void SimpleText::setViewOffset(const IntPoint& _point)
	{
	}

	/*void SimpleText::updateRawData()
	{
		if (nullptr == mFont) return;
		// сбрасывам флаги
		mTextOutDate = false;

		//FIXME
		const RenderTargetInfo& info = (mRenderItem == nullptr || mRenderItem->getRenderTarget() == nullptr) ?
			RenderManager::getInstance().getInfo() : mRenderItem->getRenderTarget()->getInfo();

		// массив для быстрой конвертации цветов
		static const char convert_colour[64] = { 0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0 };

		// вычисление размера одной единицы в текстурных координатах
		float real_fontHeight = (info.pixScaleY * (float)mFontHeight * 2.0f);//???
		GlyphInfo * glyph_info = mFont->getGlyphInfo('A');
		mTextureHeightOne = (glyph_info->uvRect.bottom - glyph_info->uvRect.top) / (real_fontHeight);
		mTextureWidthOne = (glyph_info->uvRect.right - glyph_info->uvRect.left) / (glyph_info->aspectRatio * info.aspectCoef * real_fontHeight);

		mLinesInfo.clear();

		// создаем первую строчку
		mLinesInfo.push_back(PairVectorCharInfo());
		float len = 0, width = 0;
		size_t count = 1;

		UString::const_iterator end = mCaption.end();
		for (UString::const_iterator index=mCaption.begin(); index!=end; ++index)
		{
			Char character = *index;

			if (character == FontCodeType::CR || character == FontCodeType::NEL || character == FontCodeType::LF)
			{
				// длинна строки, кратна пикселю, плюс курсор
				len = (float)((uint)(len + 0.99f));

				// запоминаем размер предыдущей строки
				mLinesInfo.back().first.set(count, (size_t)len, len * info.pixScaleX * 2.0f);

				if (width < len) width = len;
				count = 1;
				len = 0;

				// и создаем новую
				mLinesInfo.push_back(PairVectorCharInfo());

				if (character == FontCodeType::CR)
				{
					UString::const_iterator peeki = index;
					peeki++;
					if ((peeki != end) && (*peeki == FontCodeType::LF)) index = peeki; // skip both as one newline
				}
				// следующий символ
				continue;
			}
			else if (character == L'#')
			{
				// берем следующий символ
				++ index;
				if (index == end) { --index ;continue; } // это защита

				character = *index;
				// если два подряд, то рисуем один шарп, если нет то меняем цвет
				if (character != L'#')
				{
					// парсим первый символ
					RGBA colour = convert_colour[(character-48) & 0x3F];

					// и еще пять символов после шарпа
					for (char i=0; i<5; i++)
					{
						++ index;
						if (index == end) { --index ;continue; } // это защита
						colour <<= 4;
						colour += convert_colour[ ((*index) - 48) & 0x3F];
					}

					// если нужно, то меняем красный и синий компоненты
					MYGUI_CONVERT_COLOUR(colour, mVertexFormat);

					// запоминаем цвет, в верхнем байте единицы
					mLinesInfo.back().second.push_back(EnumCharInfo(colour, true) );

					continue;
				}
			}

			GlyphInfo * glyph_info;
			if (FontCodeType::Space == character) glyph_info = mFont->getSpaceGlyphInfo();
			else if (FontCodeType::Tab == character) glyph_info = mFont->getTabGlyphInfo();
			else glyph_info = mFont->getGlyphInfo(character);

			len += glyph_info->aspectRatio * (float)mFontHeight;

			// указатель на инфо о символе
			mLinesInfo.back().second.push_back( EnumCharInfo(glyph_info) );
			count ++;
		}

		// длинна строки, кратна пикселю
		len = (float)((uint)(len + 0.99f));

		// запоминаем размер предыдущей строки
		mLinesInfo.back().first.set(count, (size_t)len, len * info.pixScaleX * 2.0f);

		if (width < len) width = len;

		// устанавливаем размер текста
		mContextSize.set(int(width), mLinesInfo.size() * mFontHeight);
		mContextRealSize.set(mContextSize.width * info.pixScaleX * 2.0f, mContextSize.height  * info.pixScaleY * 2.0f);
	}*/

} // namespace MyGUI
