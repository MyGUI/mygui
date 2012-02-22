/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#ifndef __MYGUI_SKIN_ITEM_H__
#define __MYGUI_SKIN_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_ISubWidgetRect.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_ResourceSkin.h"

namespace MyGUI
{

	class MYGUI_EXPORT SkinItem :
		public LayerItem
	{
	public:
		SkinItem();

		/** Get text sub widget or nullptr if no text sub widget */
		ISubWidgetText* getSubWidgetText();
		/** Get sub widget of first texture or nullptr if no sub widget with texture */
		ISubWidgetRect* getSubWidgetMain();

		void _setTextureName(const std::string& _texture);
		const std::string& _getTextureName() const;

	protected:
		void _createSkinItem(ResourceSkin* _info);
		void _deleteSkinItem();

		void _setSkinItemAlign(const IntSize& _size);
		void _setSkinItemVisible(bool _value);
		void _setSkinItemColour(const Colour& _value);
		void _setSkinItemAlpha(float _value);
		void _correctSkinItemView();
		void _updateSkinItemView();

		bool _setSkinItemState(const std::string& _state);
		void _setSubSkinVisible(bool _visible);

	private:
		// вектор всех детей сабскинов
		VectorSubWidget mSubSkinChild;
		// указатель на окно текста
		ISubWidgetText* mText;
		// указатель на первый не текстовой сабскин
		ISubWidgetRect* mMainSkin;
		// список всех стейтов
		MapWidgetStateInfo mStateInfo;

		std::string mTextureName;
		ITexture* mTexture;

		bool mSubSkinsVisible;
	};

} // namespace MyGUI

#endif // __MYGUI_SKIN_ITEM_H__
