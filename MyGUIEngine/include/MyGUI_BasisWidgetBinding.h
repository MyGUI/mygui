/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_BASIS_WIDGET_BINDING_H__
#define __MYGUI_BASIS_WIDGET_BINDING_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	// вспомогательный класс для инициализации сабскинов
	class _MyGUIExport BasisWidgetBinding
	{
	private:
		// для доступа к внутренним членам
		friend 	WidgetSkinInfo;

	public:
		BasisWidgetBinding()
		{
		}

		BasisWidgetBinding(const IntRect & _offset, Align _aligin, const std::string & _type)
		{
			create(_offset, _aligin, _type);
		}

		void create(const IntRect & _offset, Align _aligin, const std::string & _type)
		{
			clear();
			mOffset = _offset;
			mAlign = _aligin;
			mType = _type;
		}

		void clear()
		{
			mType = "";
			mAlign = 0;
			mStates.clear();
		}

		void add(const std::string & _name, const FloatRect & _offset)
		{
			add(_name, _offset, Ogre::ColourValue::ZERO, -1);
		}

		void add(const std::string & _name, float _alpha)
		{
			add(_name, FloatRect(-1, -1, -1, -1), Ogre::ColourValue::ZERO, _alpha);
		}

		void add(const std::string & _name, const Ogre::ColourValue & _colour)
		{
			add(_name, FloatRect(-1, -1, -1, -1), _colour, -1);
		}

		void add(const std::string & _name, const FloatRect & _offset, const Ogre::ColourValue  & _colour, float _alpha)
		{
			// ищем такой же ключ
			MapBasisWidgetStateInfo::const_iterator iter = mStates.find(_name);
			if (iter != mStates.end()) ASSERT(!"state is exist");
			// добавляем
			mStates[_name] = BasisWidgetStateInfo(_offset, _colour, _alpha);
		}

	private:
		IntRect mOffset;
		Align mAlign;
		std::string mType;
		MapBasisWidgetStateInfo mStates;
	};

} // namespace MyGUI


#endif // __MYGUI_BASIS_WIDGET_BINDING_H__