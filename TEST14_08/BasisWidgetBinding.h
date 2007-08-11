#pragma once

#include <vector>
#include <Ogre.h>
#include "WidgetDefines.h"


namespace widget
{
	class WidgetSkinInfo;

	// вспомогательный класс для инициализации сабскинов
	class BasisWidgetBinding
	{
	private:
		// для доступа к внутренним членам
		friend 	WidgetSkinInfo;

	public:
		BasisWidgetBinding(int _x, int _y, int _cx, int _cy, char _aligin, const Ogre::String & _type)
		{
			create(INTRect(_x, _y, _cx, _cy), _aligin, _type);
		}

		BasisWidgetBinding(INTRect _offset, char _aligin, const Ogre::String & _type)
		{
			create(_offset, _aligin, _type);
		}

		void create(int _x, int _y, int _cx, int _cy, char _aligin, const Ogre::String & _type)
		{
			create(INTRect(_x, _y, _cx, _cy), _aligin, _type);
		}

		void create(INTRect _offset, char _aligin, const Ogre::String & _type)
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

		void add(const Ogre::String & _name, float _x, float _y, float _cx, float _cy)
		{
			add(_name, FloatRect(_x, _y, _cx, _cy), Ogre::ColourValue::ZERO, -1);
		}

		void add(const Ogre::String & _name, FloatRect _offset)
		{
			add(_name, _offset, Ogre::ColourValue::ZERO, -1);
		}

		void add(const Ogre::String & _name, float _alpha)
		{
			add(_name, FloatRect(-1, -1, -1, -1), Ogre::ColourValue::ZERO, _alpha);
		}

		void add(const Ogre::String & _name, Ogre::ColourValue  _color)
		{
			add(_name, FloatRect(-1, -1, -1, -1), _color, -1);
		}

		void add(const Ogre::String & _name, FloatRect _offset, Ogre::ColourValue  _color, float _alpha)
		{
			// ищем такой же ключ
			ViewInfo::const_iterator iter = m_states.find(_name);
			if (iter != m_states.end()) assert(0 && "state is exist");
			// добавляем
			m_states[_name] = tagBasisWidgetStateInfo(_offset, _color, _alpha);
		}

	private:
		INTRect m_offset;
		char m_aligin;
		Ogre::String m_type;
		ViewInfo m_states;
	};

} // namespace widget