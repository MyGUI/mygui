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

	Sheet::Sheet(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name)//,
		//mOwner(null)
	{
		//MYGUI_ASSERT(null != _parent, "Sheet must create only in Tab");
		mOwner = castWidget<Tab>(static_cast<WidgetPtr>(_parent));
	}

	Sheet::~Sheet()
	{
		mOwner->_notifyDeleteSheet(this);
	}

	//void Sheet::setPosition(const IntPoint& _pos) {Widget::setPosition(_pos);}
	//void Sheet::setPosition(const IntCoord& _coord) {Widget::setPosition(_coord);}
	//void Sheet::setSize(const IntSize& _size) {Widget::setSize(_size);}

	//void Sheet::show() {Widget::show();}
	//void Sheet::hide() {Widget::hide();}

	void Sheet::setCaption(const Ogre::DisplayString & _caption)
	{
		//MYGUI_ASSERT(null != mOwner, "Sheet must create only in Tab");
		mOwner->setSheetName(this, _caption);
	}

	const Ogre::DisplayString & Sheet::getCaption()
	{
		//MYGUI_ASSERT(null != mOwner, "Sheet must create only in Tab");
		return mOwner->getSheetName(this);
	}

	const Ogre::DisplayString & Sheet::getSheetName()
	{
		//MYGUI_ASSERT(null != mOwner, "Sheet must create only in Tab");
		return mOwner->getSheetName(this);
	}

	void Sheet::setSheetName(const Ogre::DisplayString & _name, int _width)
	{
		//MYGUI_ASSERT(null != mOwner, "Sheet must create only in Tab");
		mOwner->setSheetName(this, _name, _width);
	}

	int Sheet::getSheetButtonWidth()
	{
		//MYGUI_ASSERT(null != mOwner, "Sheet must create only in Tab");
		return mOwner->getSheetButtonWidth(this);
	}

	void Sheet::setSheetButtonWidth(int _width)
	{
		//MYGUI_ASSERT(null != mOwner, "Sheet must create only in Tab");
		mOwner->setSheetButtonWidth(this, _width);
	}

	void Sheet::selectSheet(bool _smooth)
	{
		//MYGUI_ASSERT(null != mOwner, "Sheet must create only in Tab");
		mOwner->selectSheet(this, _smooth);
	}

	void Sheet::removeSheet()
	{
		//MYGUI_ASSERT(null != mOwner, "Sheet must create only in Tab");
		mOwner->removeSheet(this);
	}

} // namespace MyGUI
