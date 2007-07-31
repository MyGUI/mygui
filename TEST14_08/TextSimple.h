#pragma once

#include <Ogre.h>
//#include "Widget.h"
#include "SubWidget.h"
#include "TextSimpleOverlayElement.h"

namespace widget
{
	using namespace Ogre;


	class TextSimple : public SubWidget
	{

	public:
		TextSimple(char _align, SubWidget * _parent);
		virtual ~TextSimple();

		void update(); // обновления себя и детей

		void show(bool _show);

		void align(int _cx, int _cy, bool _update);
		void align(int _x, int _y, int _cx, int _cy, bool _update);

		void setCaption(const Ogre::DisplayString & _caption);

		void setColour(float _red, float _green, float _blue);
		void setAlpha(float _alpha);

	protected:

		TextSimpleOverlayElement * m_overlayContainer;
		Ogre::ColourValue m_color;

	}; // class TextSimple : public SubWidget

} // namespace widget