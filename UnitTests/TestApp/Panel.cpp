/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/

#include "Panel.h"
#include "MyGUI_TextBox.h"

namespace MyGUI
{

	Panel::Panel()
	{
	}

	void Panel::updateMeasure(Widget* _widget, const IntSize& _sizeAvailable)
	{
		Panel* panel = _widget->castType<Panel>(false);
		if (panel != nullptr)
		{
			panel->mDesiredSize = panel->overrideMeasure(_sizeAvailable);
		}
		else
		{
			IntSize size;
			TextBox* text = _widget->castType<TextBox>(false);
			if (text != nullptr)
			{
				size = text->getSize() - text->getTextRegion().size() + text->getTextSize();
			}
			else
			{
				size = _widget->getSize();
			}

			_widget->setUserString("mDesiredSize", size.print());
		}
	}

	void Panel::updateArrange(const IntCoord& _coordPlace, const IntSize& _oldsize)
	{
	}

	IntSize Panel::overrideMeasure(const IntSize& _sizeAvailable)
	{
		return IntSize();
	}

	void Panel::overrideArrange(const IntSize& _sizeOld)
	{
	}

	IntSize Panel::getDesiredSize(Widget* _widget)
	{
		Panel* panel = _widget->castType<Panel>(false);
		if (panel != nullptr)
			return panel->mDesiredSize;

		return IntSize::parse(_widget->getUserString("mDesiredSize"));
	}

} // namespace MyGUI
