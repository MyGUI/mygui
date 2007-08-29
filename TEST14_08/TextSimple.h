#pragma once

#include <Ogre.h>
#include "BasisWidget.h"
#include "TextSimpleOverlayElement.h"

namespace widget
{
	using namespace Ogre;


	class TextSimple : public BasisWidget
	{

	public:
		TextSimple(const tagBasisWidgetInfo &_info, const String & _material, BasisWidgetPtr _parent);
		virtual ~TextSimple();

		void show(bool _show);

		void setCaption(const Ogre::DisplayString & _caption);
		const Ogre::DisplayString & getCaption();

		void setColour(const Ogre::ColourValue & _color);
		void setAlpha(float _alpha);

		void setFontName(const Ogre::String & _font);
		void setFontName(const Ogre::String & _font, Ogre::ushort _height);
		const Ogre::String & getFontName();

		void setFontHeight(Ogre::ushort _height);
		Ogre::ushort getFontHeight();

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void update(); // обновления себя и детей
		inline void updateText() // обновляем все что касаеться текста
		{
			// узнаем новый размер, там же он и запоминается
			m_overlayContainer->getTextSize(m_cx, m_cy);
			// и делаем полное обновление и выравнивание
			m_margin = true; // при изменении размеров все пересчитывать
			align(m_cx, m_cy, true);
		}

		bool isText() {return true;};

		inline const static Ogre::String & getType() {static Ogre::String type("TextSimple"); return type;};

		OverlayElementPtr getOverlayElement();

	protected:

		TextSimpleOverlayElement * m_overlayContainer;
		Ogre::ColourValue m_color;

	}; // class TextSimple : public BasisWidget

} // namespace widget