/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_PointerManager.h"

#include "MyGUI_WidgetFactory.h"
#include "MyGUI_ButtonFactory.h"
#include "MyGUI_EditFactory.h"
#include "MyGUI_ListFactory.h"
#include "MyGUI_StaticImageFactory.h"
#include "MyGUI_StaticTextFactory.h"
#include "MyGUI_VScrollFactory.h"
#include "MyGUI_HScrollFactory.h"
#include "MyGUI_WindowFactory.h"
#include "MyGUI_ComboBoxFactory.h"
#include "MyGUI_TabFactory.h"
#include "MyGUI_SheetFactory.h"
#include "MyGUI_MessageFactory.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(WidgetManager);

	void WidgetManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		registerUnlinker(this);

		// создаем фабрики виджетов
		mWidgetFactory = new factory::WidgetFactory();
		mButtonFactory = new factory::ButtonFactory();
		mEditFactory = new factory::EditFactory();
		mListFactory = new factory::ListFactory();
		mStaticTextFactory = new factory::StaticTextFactory();
		mStaticImageFactory = new factory::StaticImageFactory();
		mVScrollFactory = new factory::VScrollFactory();
		mHScrollFactory = new factory::HScrollFactory();
		mWindowFactory = new factory::WindowFactory();
		mComboBoxFactory = new factory::ComboBoxFactory();
		mTabFactory = new factory::TabFactory();
		mSheetFactory = new factory::SheetFactory();
		mMessageFactory = new factory::MessageFactory();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void WidgetManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		unregisterUnlinker(this);

		mFactoryList.clear();
		mDelegates.clear();
		mVectorUnlinkWidget.clear();

		delete mWindowFactory;
		delete mHScrollFactory;
		delete mVScrollFactory;
		delete mStaticTextFactory;
		delete mStaticImageFactory;
		delete mListFactory;
		delete mEditFactory;
		delete mButtonFactory;
		delete mWidgetFactory;
		delete mComboBoxFactory;
		delete mTabFactory;
		delete mSheetFactory;
		delete mMessageFactory;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void WidgetManager::registerFactory(WidgetFactoryInterface * _factory)
	{
		mFactoryList.insert(_factory);
		MYGUI_LOG(Info, "* Register widget factory '" << _factory->getType() << "'");
	}

	void WidgetManager::unregisterFactory(WidgetFactoryInterface * _factory)
	{
		SetWidgetFactory::iterator iter = mFactoryList.find(_factory);
		if (iter != mFactoryList.end()) mFactoryList.erase(iter);
		MYGUI_LOG(Info, "* Unregister widget factory '" << _factory->getType() << "'");
	}

	WidgetPtr WidgetManager::createWidget(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String & _name)
	{
		Ogre::String name;
		if (false == _name.empty()) {
			MapWidgetPtr::iterator iter = mWidgets.find(_name);
			MYGUI_ASSERT(iter == mWidgets.end(), "widget with name '" << _name << "' already exist");
			name = _name;
		} else {
			static long num=0;
			name = util::toString(num++, "_", _type);
		}

		for (SetWidgetFactory::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ( (*factory)->getType() == _type) {
				WidgetPtr widget = (*factory)->createWidget(_skin, _coord, _align, _parent, name);
				mWidgets[name] = widget;
				return widget;
			}
		}
		MYGUI_EXCEPT("factory '" << _type << "' not found");
		return null;
	}

	WidgetPtr WidgetManager::findWidgetT(const Ogre::String & _name)
	{
		MapWidgetPtr::iterator iter = mWidgets.find(_name);
		if (iter == mWidgets.end()){
			MYGUI_LOG(Error, "Widget '" << _name << "' not found");
			return null;
		}
		return iter->second;
	}

	void WidgetManager::_unlinkWidget(WidgetPtr _widget)
	{
		if (_widget == null) return;
		MapWidgetPtr::iterator iter = mWidgets.find(_widget->getName());
		if (iter != mWidgets.end()) mWidgets.erase(iter);
	}

	FloatRect WidgetManager::convertOffset(const FloatRect & _offset, Align _align, const IntSize & _parentSkinSize, int _parentWidth, int _parentHeight)
	{
		FloatRect offset = _offset;

		if (IS_ALIGN_RIGHT(_align)) {
			if (IS_ALIGN_LEFT(_align)) offset.right += _parentWidth - _parentSkinSize.width;
			else offset.left += _parentWidth - _parentSkinSize.width;
		}
		else if (false == IS_ALIGN_LEFT(_align)) offset.left += (_parentWidth - _parentSkinSize.width) / 2;

		if (IS_ALIGN_BOTTOM(_align)) {
			if (IS_ALIGN_TOP(_align)) offset.bottom += _parentHeight - _parentSkinSize.height;
			else offset.top += _parentHeight - _parentSkinSize.height;
		}
		else if (false == IS_ALIGN_TOP(_align)) offset.top += (_parentHeight - _parentSkinSize.height) / 2;

		return offset;
	}

	// преобразует точку на виджете в глобальную позицию
	IntPoint WidgetManager::convertToGlobal(const IntPoint& _point, WidgetPtr _widget)
	{
		IntPoint ret = _point;
		WidgetPtr wid = _widget;
		while (wid != null) {
			ret += wid->getPosition();
			wid = wid->getParent();
		}
		return ret;
	}

	ParseDelegate & WidgetManager::registerDelegate(const Ogre::String & _key)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		MYGUI_ASSERT(iter == mDelegates.end(), "delegate with name '" << _key << "' already exist");
		return (mDelegates[_key] = ParseDelegate());
	}

	void WidgetManager::unregisterDelegate(const Ogre::String & _key)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter != mDelegates.end()) mDelegates.erase(iter);
	}

	void WidgetManager::parse(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		MYGUI_ASSERT(iter != mDelegates.end(), "Unknown key '" << _key << "' with value '" << _value << "'");
		iter->second(_widget, _key, _value);
	}

	void WidgetManager::destroyWidget(WidgetPtr _widget)
	{
		// иначе возможен бесконечный цикл
		MYGUI_ASSERT(_widget != null, "widget is deleted");

		// отписываем от всех
		unlinkFromUnlinkers(_widget);

		// делегирует удаление отцу виджета
		WidgetPtr parent = _widget->getParent();
		if (parent == null) Gui::getInstance()._destroyChildWidget(_widget);
		else parent->_destroyChildWidget(_widget);

	}

	void WidgetManager::destroyAllWidget()
	{
		Gui::getInstance()._destroyAllChildWidget();
	}

	void WidgetManager::registerUnlinker(UnlinkWidget * _unlink)
	{
		unregisterUnlinker(_unlink);
		mVectorUnlinkWidget.push_back(_unlink);
	}

	void WidgetManager::unregisterUnlinker(UnlinkWidget * _unlink)
	{
		for (size_t pos=0; pos<mVectorUnlinkWidget.size(); pos++) {
			if (mVectorUnlinkWidget[pos] == _unlink) {
				mVectorUnlinkWidget[pos] = mVectorUnlinkWidget[mVectorUnlinkWidget.size()-1];
				mVectorUnlinkWidget.pop_back();
				return;
			}
		}
	}

	void WidgetManager::unlinkFromUnlinkers(WidgetPtr _widget)
	{
		for (size_t pos=0; pos<mVectorUnlinkWidget.size(); pos++) {
			mVectorUnlinkWidget[pos]->_unlinkWidget(_widget);
		}
	}

} // namespace MyGUI
