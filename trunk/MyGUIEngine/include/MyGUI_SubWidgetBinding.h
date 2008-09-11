/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_SUB_WIDGET_BINDING_H__
#define __MYGUI_SUB_WIDGET_BINDING_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	// вспомогательный класс для инициализации сабскинов
	class _MyGUIExport SubWidgetBinding
	{
	private:
		// для доступа к внутренним членам
		friend class 	WidgetSkinInfo;

	public:
		SubWidgetBinding()
		{
			clear();
		}

		SubWidgetBinding(const IntCoord& _coord, Align _aligin, const std::string & _type)
		{
			create(_coord, _aligin, _type);
		}

		void create(const IntCoord & _coord, Align _aligin, const std::string & _type)
		{
			clear();
			mOffset = _coord;
			mAlign = _aligin;
			mType = _type;
		}

		void clear()
		{
			mType = "";
			mAlign = 0;
			mStates.clear();
		}

		void add(const std::string & _name, void * _data)
		{
			// ищем такой же ключ
			MapSubWidgetStateInfo::const_iterator iter = mStates.find(_name);
			MYGUI_ASSERT(iter == mStates.end(), "state with name '" << _name << "' already exist");
			// добавляем
			mStates[_name] = SubWidgetStateInfoPtr(_data);
		}

	private:
		IntCoord mOffset;
		Align mAlign;
		std::string mType;
		MapSubWidgetStateInfo mStates;
	};

} // namespace MyGUI


#endif // __MYGUI_SUB_WIDGET_BINDING_H__
