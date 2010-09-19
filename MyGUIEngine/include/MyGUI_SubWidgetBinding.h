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
#ifndef __MYGUI_SUB_WIDGET_BINDING_H__
#define __MYGUI_SUB_WIDGET_BINDING_H__

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
		SubWidgetBinding(const IntCoord& _coord, Align _aligin, const std::string& _type);

		void create(const IntCoord& _coord, Align _aligin, const std::string& _type);

		void clear();

		void add(const std::string& _name, IStateInfo* _data, const std::string& _skin);

	private:
		IntCoord mOffset;
		Align mAlign;
		std::string mType;
		MapStateInfo mStates;
	};

} // namespace MyGUI


#endif // __MYGUI_SUB_WIDGET_BINDING_H__
