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

		void add(const std::string & _name, const FloatRect& _rect)
		{
			add(_name, _rect, Ogre::ColourValue::ZERO, -1);
		}

		void add(const std::string & _name, float _alpha)
		{
			add(_name, FloatRect(-1, -1, -1, -1), Ogre::ColourValue::ZERO, _alpha);
		}

		void add(const std::string & _name, const Ogre::ColourValue & _colour)
		{
			add(_name, FloatRect(-1, -1, -1, -1), _colour, -1);
		}

		void add(const std::string & _name, const FloatRect & _rect, const Ogre::ColourValue  & _colour, float _alpha)
		{
			// ищем такой же ключ
			MapSubWidgetStateInfo::const_iterator iter = mStates.find(_name);
			MYGUI_ASSERT(iter == mStates.end(), "state with name '" << _name << "' already exist");
			// добавляем
			mStates[_name] = SubWidgetStateInfo(_rect, _colour, _alpha);
		}

	private:
		IntCoord mOffset;
		Align mAlign;
		std::string mType;
		MapSubWidgetStateInfo mStates;
	};

} // namespace MyGUI


#endif // __MYGUI_SUB_WIDGET_BINDING_H__
