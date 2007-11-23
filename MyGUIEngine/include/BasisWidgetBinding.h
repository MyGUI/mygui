#ifndef __BASIS_WIDGET_BINDING_H__
#define __BASIS_WIDGET_BINDING_H__

#include "Prerequest.h"
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

		BasisWidgetBinding(const IntRect & _offset, Align _aligin, const std::string & _type)
		{
			create(_offset, _aligin, _type);
		}

		void create(const IntRect & _offset, Align _aligin, const std::string & _type)
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

		void add(const std::string & _name, const FloatRect & _offset)
		{
			add(_name, _offset, Ogre::ColourValue::ZERO, -1);
		}

		void add(const std::string & _name, float _alpha)
		{
			add(_name, FloatRect(-1, -1, -1, -1), Ogre::ColourValue::ZERO, _alpha);
		}

		void add(const std::string & _name, const Ogre::ColourValue & _color)
		{
			add(_name, FloatRect(-1, -1, -1, -1), _color, -1);
		}

		void add(const std::string & _name, const FloatRect & _offset, const Ogre::ColourValue  & _color, float _alpha)
		{
			// ищем такой же ключ
			MapBasisWidgetStateInfo::const_iterator iter = m_states.find(_name);
			if (iter != m_states.end()) ASSERT(!"state is exist");
			// добавляем
			m_states[_name] = BasisWidgetStateInfo(_offset, _color, _alpha);
		}

	private:
		IntRect m_offset;
		Align m_aligin;
		std::string m_type;
		MapBasisWidgetStateInfo m_states;
	};

} // namespace MyGUI


#endif // __BASIS_WIDGET_BINDING_H__