/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_SubWidgetBinding.h"

namespace MyGUI
{

	SubWidgetBinding::SubWidgetBinding()
	{
		clear();
	}

	SubWidgetBinding::SubWidgetBinding(const IntCoord& _coord, Align _aligin, std::string_view _type)
	{
		create(_coord, _aligin, _type);
	}

	void SubWidgetBinding::create(const IntCoord& _coord, Align _aligin, std::string_view _type)
	{
		clear();
		mOffset = _coord;
		mAlign = _aligin;
		mType = _type;
	}

	void SubWidgetBinding::clear()
	{
		mType.clear();
		mAlign = Align::Default;
		mStates.clear();
	}

	void SubWidgetBinding::add(std::string_view _name, IStateInfo* _data, std::string_view _skin)
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
		mStates.emplace(_name, _data);
	}

} // namespace MyGUI
