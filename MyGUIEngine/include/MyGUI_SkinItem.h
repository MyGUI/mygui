/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SKIN_ITEM_H_
#define MYGUI_SKIN_ITEM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_ISubWidgetRect.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_ResourceSkin.h"

namespace MyGUI
{

	class MYGUI_EXPORT SkinItem : public LayerItem
	{
	public:
		/** Get text sub widget or nullptr if no text sub widget */
		ISubWidgetText* getSubWidgetText() const;
		/** Get sub widget of first texture or nullptr if no sub widget with texture */
		ISubWidgetRect* getSubWidgetMain() const;

		void _setTextureName(std::string_view _texture);
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

		bool _setSkinItemState(std::string_view _state);
		void _setSubSkinVisible(bool _visible);

	private:
		// вектор всех детей сабскинов
		VectorSubWidget mSubSkinChild;
		// указатель на окно текста
		ISubWidgetText* mText{nullptr};
		// указатель на первый не текстовой сабскин
		ISubWidgetRect* mMainSkin{nullptr};
		// список всех стейтов
		MapWidgetStateInfo mStateInfo;

		std::string mTextureName;
		ITexture* mTexture{nullptr};

		bool mSubSkinsVisible{true};
	};

} // namespace MyGUI

#endif // MYGUI_SKIN_ITEM_H_
