#pragma once

#include <vector>
#include <Ogre.h>
#include "MainSkin.h"
#include "SubSkin.h"
#include "BasisWidget.h"
#include "delegate.h"


namespace widget
{

	// информация об одном стейте виджета
/*	class StateInfo
	{
	public:
		StateInfo() {};

		StateInfo(const Ogre::ColourValue & _color, float _alpha) :
			m_color(_color),
			m_alpha(_alpha)
		{
		}

		inline const float getAlpha() const {return m_alpha;};
		inline const Ogre::ColourValue & getColour() const {return m_color;};

//	protected:
		std::vector<Ogre::FloatRect> m_offset;
		float m_alpha;
		Ogre::ColourValue m_color;

	}; // class StateInfo

	class BasisWidgetInfo
	{
	public:
		BasisWidgetInfo(INTRect _offset, char _aligin, const Ogre::String & _type) :
			m_offset(_offset),
			m_align(_aligin),
			m_type(_type)
		{
		}

		inline const INTRect & getOffset() const {return m_offset;};
		inline const Ogre::String & getType() const {return m_type;};
		inline const char getAlign() const {return m_align;};

	private:
		INTRect m_offset;
		char m_align;
		Ogre::String m_type;
	}; // class BasisWidgetInfo

	typedef std::map<Ogre::String, StateInfo> StateList;
	typedef StateList::const_iterator StateIterator;
	typedef std::vector<BasisWidgetInfo> BasisList;
	typedef BasisList::const_iterator BasisIterator;

	// полная информация об скине виджета
	class StateSetInfo
	{
	public:
		
		inline const Ogre::String & getMaterial() const {return m_material;};
		inline const Ogre::String & getFontName() const {return m_fontName;};
		inline Ogre::ushort getFontHeight() const {return n_fontHeight;};

		inline const BasisList & getBasisInfo() const {return m_basis;};

//	protected:
		Ogre::String m_material;
		Ogre::String m_fontName;
		Ogre::ushort n_fontHeight;

		StateList m_states;
		BasisList m_basis;

	}; // class StateSetInfo
*/
} // namespace widget