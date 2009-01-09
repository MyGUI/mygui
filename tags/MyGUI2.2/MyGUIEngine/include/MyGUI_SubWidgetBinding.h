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
#include "MyGUI_ISubWidget.h"

namespace MyGUI
{

	// вспомогательный класс для инициализации сабскинов
	class MYGUI_EXPORT SubWidgetBinding
	{
		// для доступа к внутренним членам
		friend class WidgetSkinInfo;

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
			mAlign = Align::Default;
			mStates.clear();
		}

		void add(const std::string & _name, StateInfo * _data, const std::string & _skin)
		{
			// ищем такой же ключ
			MapStateInfo::const_iterator iter = mStates.find(_name);
			if (iter != mStates.end()) {
				delete _data;
				MYGUI_LOG(Warning, "state with name '" << _name << "' already exist in skin '" << _skin << "'");
				return;
			}
			// добавляем
			mStates[_name] = _data;
		}

		void addProperty(const std::string & _key, const std::string & _value)
		{
			mProperties[_key] = _value;
		}

	private:
		IntCoord mOffset;
		Align mAlign;
		std::string mType;
		MapStateInfo mStates;
		MapString mProperties;
	};

} // namespace MyGUI


#endif // __MYGUI_SUB_WIDGET_BINDING_H__
