/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SUB_WIDGET_BINDING_H_
#define MYGUI_SUB_WIDGET_BINDING_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ISubWidget.h"

namespace MyGUI
{

	// вспомогательный класс для инициализации сабскинов
	class MYGUI_EXPORT SubWidgetBinding
	{
		// для доступа к внутренним членам
		friend class ResourceSkin;

	public:
		SubWidgetBinding();
		SubWidgetBinding(const IntCoord& _coord, Align _aligin, std::string_view _type);

		void create(const IntCoord& _coord, Align _aligin, std::string_view _type);

		void clear();

		void add(std::string_view _name, IStateInfo* _data, std::string_view _skin);

	private:
		IntCoord mOffset;
		Align mAlign;
		std::string mType;
		MapStateInfo mStates;
	};

} // namespace MyGUI


#endif // MYGUI_SUB_WIDGET_BINDING_H_
