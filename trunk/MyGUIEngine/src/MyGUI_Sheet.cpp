/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Sheet.h"
#include "MyGUI_Tab.h"
#include "MyGUI_CastWidget.h"

namespace MyGUI
{

	Ogre::String Sheet::WidgetTypeName = "Sheet";

	Sheet::Sheet(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name)
	{
		mOwner = castWidget<Tab>(static_cast<WidgetPtr>(_parent));
	}

	Sheet::~Sheet()
	{
		mOwner->_notifyDeleteSheet(this);
	}

	void Sheet::setCaption(const Ogre::UTFString & _caption)
	{
		mOwner->setSheetName(this, _caption);
	}

	const Ogre::UTFString & Sheet::getCaption()
	{
		return mOwner->getSheetName(this);
	}

	const Ogre::UTFString & Sheet::getSheetName()
	{
		return mOwner->getSheetName(this);
	}

	void Sheet::setSheetName(const Ogre::UTFString & _name, int _width)
	{
		mOwner->setSheetName(this, _name, _width);
	}

	int Sheet::getSheetButtonWidth()
	{
		return mOwner->getSheetButtonWidth(this);
	}

	void Sheet::setSheetButtonWidth(int _width)
	{
		mOwner->setSheetButtonWidth(this, _width);
	}

	void Sheet::selectSheet(bool _smooth)
	{
		mOwner->selectSheet(this, _smooth);
	}

	void Sheet::removeSheet()
	{
		mOwner->removeSheet(this);
	}

} // namespace MyGUI
