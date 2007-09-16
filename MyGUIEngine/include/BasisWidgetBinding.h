#ifndef _BASICWIDGETBINDING_H_
#define _BASICWIDGETBINDING_H_

#include "Prerequest.h"
#include <vector>
//#include <Ogre.h>
#include "WidgetDefines.h"


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

		BasisWidgetBinding(const intRect & _offset, char _aligin, const std::string & _type)
		{
			create(_offset, _aligin, _type);
		}

		void create(const intRect & _offset, char _aligin, const std::string & _type)
		{
			clear();
			m_offset = _offset;
			m_aligin = _aligin;
			m_type = _type;
		}

		void clear()
		{
			m_type = "";
			m_aligin = 0;
			m_states.clear();
		}

		void add(const std::string & _name, const floatRect & _offset)
		{
			add(_name, _offset, Ogre::ColourValue::ZERO, -1);
		}

		void add(const std::string & _name, float _alpha)
		{
			add(_name, floatRect(-1, -1, -1, -1), Ogre::ColourValue::ZERO, _alpha);
		}

		void add(const std::string & _name, const Ogre::ColourValue & _color)
		{
			add(_name, floatRect(-1, -1, -1, -1), _color, -1);
		}

		void add(const std::string & _name, const floatRect & _offset, const Ogre::ColourValue  & _color, float _alpha)
		{
			// ищем такой же ключ
			ViewInfo::const_iterator iter = m_states.find(_name);
			if (iter != m_states.end()) ASSERT(!"state is exist");
			// добавляем
			m_states[_name] = tagBasisWidgetStateInfo(_offset, _color, _alpha);
		}

	private:
		intRect m_offset;
		char m_aligin;
		std::string m_type;
		ViewInfo m_states;
	};

} // namespace MyGUI


#endif