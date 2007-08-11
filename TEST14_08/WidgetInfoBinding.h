#pragma once

#include <vector>
#include <Ogre.h>
#include "WidgetInfoDefines.h"


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



	// вспомогательный класс для инициализации одного скина
	class WidgetSkinInfo
	{

	public:
		WidgetSkinInfo(int _cx, int _cy, const Ogre::String &_material, const Ogre::String &_font="", Ogre::ushort _fontHeight=0) :
			m_cx(_cx),
			m_cy(_cy),
			m_material(_material),
			m_font(_font),
			m_fontHeight(_fontHeight)
		{
			checkState("normal");
		}

		void addInfo(const BasisWidgetBinding & _bind)
		{
			checkState(_bind.m_states);
			m_basis.push_back(tagBasisWidgetInfo(_bind.m_type, _bind.m_offset, _bind.m_aligin));
			checkBasis();
			fillState(_bind.m_states, m_basis.size()-1);
		}

	private:
		void checkState(const ViewInfo & _states)
		{
			for (ViewInfo::const_iterator iter = _states.begin(); iter != _states.end(); iter ++) {
				checkState(iter->first);
			}
		}

		inline void checkState(const Ogre::String & _name)
		{
			// ищем такой же ключ
			StateInfo::const_iterator iter = m_states.find(_name);
			if (iter == m_states.end()) {
				// добавляем новый стейт
				m_states[_name] = tagWidgetStateInfo();
			}
		}

		inline void checkBasis()
		{
			// и увеличиваем размер смещений по колличеству сабвиджетов
			for (StateInfo::iterator iter = m_states.begin(); iter!=m_states.end(); iter++) {
				while (iter->second.m_offsets.size() < m_basis.size()) {
					iter->second.m_offsets.push_back(Ogre::FloatRect(0, 0, 0, 0));
				}
			}
		}

		inline void fillState(const ViewInfo & _states, size_t _index)
		{
			for (ViewInfo::const_iterator iter = _states.begin(); iter != _states.end(); iter ++) {
				m_states[iter->first].m_offsets[_index] = iter->second.offset;
			}
		}

	public:
		inline int width() const {return m_cx;};
		inline int height() const {return m_cy;};
		inline const Ogre::String & getMaterial() const {return m_material;};
		inline const Ogre::String & getFontName() const {return m_font;};
		inline Ogre::ushort getFontHeight() const {return m_fontHeight;};
		inline const BasisInfo & getBasisInfo() const {return m_basis;};
		inline const StateInfo & getStateInfo() const {return m_states;};

	private:
		int m_cx, m_cy;
		Ogre::String m_material;
		Ogre::String m_font;
		Ogre::ushort m_fontHeight;
		BasisInfo m_basis;
		StateInfo m_states;

	};




} // namespace widget