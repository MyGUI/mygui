/*!
	@file
	@author		Albert Semenov
	@date		09/2010
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
#include "MyGUI_SubWidgetBinding.h"

namespace MyGUI
{

	SubWidgetBinding::SubWidgetBinding()
	{
		clear();
	}

	SubWidgetBinding::SubWidgetBinding(const IntCoord& _coord, Align _aligin, const std::string& _type)
	{
		create(_coord, _aligin, _type);
	}

	void SubWidgetBinding::create(const IntCoord& _coord, Align _aligin, const std::string& _type)
	{
		clear();
		mOffset = _coord;
		mAlign = _aligin;
		mType = _type;
	}

	void SubWidgetBinding::clear()
	{
		mType = "";
		mAlign = Align::Default;
		mStates.clear();
	}

	void SubWidgetBinding::add(const std::string& _name, IStateInfo* _data, const std::string& _skin)
	{
		// ищем такой же ключ
		MapStateInfo::const_iterator iter = mStates.find(_name);
		if (iter != mStates.end())
		{
			delete _data;
			MYGUI_LOG(Warning, "state with name '" << _name << "' already exist in skin '" << _skin << "'");
			return;
		}
		// добавляем
		mStates[_name] = _data;
	}

} // namespace MyGUI
