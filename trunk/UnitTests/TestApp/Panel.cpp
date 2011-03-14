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

	void Panel::setPosition(const IntPoint& _value)
	{
		Base::setPosition(_value);
	}

	void Panel::setSize(const IntSize& _value)
	{
		Base::setSize(_value);
		overrideArrange();
	}

	void Panel::setCoord(const IntCoord& _value)
	{
		Base::setCoord(_value);
		overrideArrange();
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

			_widget->_setInternalData(size);
		}
	}

	void Panel::updateArrange(Widget* _widget, const IntCoord& _coordPlace)
	{
		updateMeasure(_widget, _coordPlace.size());
		_widget->setCoord(_coordPlace);
	}

	IntSize Panel::overrideMeasure(const IntSize& _sizeAvailable)
	{
		return IntSize();
	}

	void Panel::overrideArrange()
	{
	}

	IntSize Panel::getDesiredSize(Widget* _widget)
	{
		Panel* panel = _widget->castType<Panel>(false);
		if (panel != nullptr)
			return panel->mDesiredSize;

		IntSize* sizePtr = _widget->_getInternalData<IntSize>(false);
		if (sizePtr != nullptr)
			return *sizePtr;

		return IntSize();
	}

	void Panel::invalidateMeasure(Widget* _widget)
	{
		if (_widget->getParent() != nullptr)
		{
			invalidateMeasure(_widget->getParent());
			Panel* panel = _widget->castType<Panel>(false);
			if (panel != nullptr)
				panel->overrideArrange();
		}
		else
		{
			const IntSize& size = _widget->getParentSize();
			Panel* panel = _widget->castType<Panel>(false);
			if (panel != nullptr)
				Panel::updateArrange(panel, IntCoord(0, 0, size.width, size.height));
		}
	}

	void Panel::onWidgetCreated(Widget* _widget)
	{
		Base::onWidgetCreated(_widget);

		invalidateMeasure(this);
	}

	void Panel::onWidgetDestroy(Widget* _widget)
	{
		invalidateMeasure(this);

		Base::onWidgetDestroy(_widget);
	}

} // namespace MyGUI
