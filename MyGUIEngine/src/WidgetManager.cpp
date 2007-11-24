
#include "WidgetManager.h"
#include "LayerManager.h"
#include "Widget.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(WidgetManager);

	void WidgetManager::initialise()
	{
		assert(!mIsInitialise);

		// ������� ������� ��������
		mWidgetFactory = new factory::WidgetFactory();
		mButtonFactory = new factory::ButtonFactory();
		mEditFactory = new factory::EditFactory();
		mListFactory = new factory::ListFactory();
		mStaticImageFactory = new factory::StaticImageFactory();
		mVScrollFactory = new factory::VScrollFactory();
		mHScrollFactory = new factory::HScrollFactory();
		mWindowFactory = new factory::WindowFactory();

		mIsInitialise = true;
	}

	void WidgetManager::shutdown()
	{
		if (!mIsInitialise) return;

		mFactoryList.clear();
		mDelegates.clear();

		delete mWindowFactory;
		delete mHScrollFactory;
		delete mVScrollFactory;
		delete mStaticImageFactory;
		delete mListFactory;
		delete mEditFactory;
		delete mButtonFactory;
		delete mWidgetFactory;

		mIsInitialise = false;
	}

	void WidgetManager::registerFactory(WidgetFactoryInterface * _factory)
	{
		mFactoryList.insert(_factory);
		//LOG_MESSAGE("* Register widget factory '" + _factory->getType() + "'");
	}

	void WidgetManager::unregisterFactory(WidgetFactoryInterface * _factory)
	{
		SetWidgetFactory::iterator iter = mFactoryList.find(_factory);
		if (iter != mFactoryList.end()) mFactoryList.erase(iter);
		//LOG_MESSAGE("* Unregister widget factory '" + _factory->getType() + "'");
	}

	WidgetPtr WidgetManager::createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String & _name)
	{
		Ogre::String name;
		if (false == _name.empty()) {
			MapWidgetPtr::iterator iter = mWidgets.find(_name);
			if (iter != mWidgets.end()) OGRE_EXCEPT(0, _name + " - name widget is exist", "WidgetManager::createWidget");
			name = _name;
		} else {
			static long num=0;
			name = Ogre::StringConverter::toString(num++) + "_" + _type;
		}

		for (SetWidgetFactory::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); factory++) {
			if ( (*factory)->getType() == _type) {
				WidgetPtr widget = (*factory)->createWidget(_skin, _x, _y, _cx, _cy, _align, _parent, name);
				mWidgets[name] = widget;
				return widget;
			}
		}
		OGRE_EXCEPT(0, _type + " - no find factory WidgetFactory", "WidgetManager::createWidget");
		return 0;
	}

	void WidgetManager::destroyWidget(WidgetPtr & _widget)
	{
		if (_widget == null) return;
		// ����������� ������ �� ������, ���� �� ��� �����������
		LayerManager::getInstance().detachItem(_widget);

		// ������� ��� � ����� ��� ������
		clearName(_widget);

		// ���� ���� ��� �� ���� ���
		WidgetPtr parent = _widget->getParent();
		if (parent == null) Gui::getInstance().destroyWidget(_widget);
		else parent->destroyWidget(_widget);

		_widget = null;
	}

	void WidgetManager::destroyWidget()
	{
		Gui::getInstance().destroyWidget();
	}

	WidgetPtr WidgetManager::findWidget(const Ogre::String & _name)
	{
		MapWidgetPtr::iterator iter = mWidgets.find(_name);
		if (iter == mWidgets.end()){
			LOG_MESSAGE("Error: Widget \"" + _name + "\" not found");
			return 0;
		}
		return iter->second;
	}

	void WidgetManager::clearName(WidgetPtr _widget)
	{
		if (_widget == null) return;
		MapWidgetPtr::iterator iter = mWidgets.find(_widget->getName());
		if (iter != mWidgets.end()) mWidgets.erase(iter);
	}

	FloatRect WidgetManager::convertOffset(const FloatRect & _offset, Align _align, const IntSize & _parentSkinSize, int _parentWidth, int _parentHeight)
	{
		FloatRect offset = _offset;

		if (_align & ALIGN_RIGHT) {
			if (_align & ALIGN_LEFT) offset.right += _parentWidth - _parentSkinSize.width;
			else offset.left += _parentWidth - _parentSkinSize.width;
		} else if (!(_align & ALIGN_LEFT)) offset.left += (_parentWidth - _parentSkinSize.width) / 2;

		if (_align & ALIGN_BOTTOM) {
			if (_align & ALIGN_TOP) offset.bottom += _parentHeight - _parentSkinSize.height;
			else offset.top += _parentHeight - _parentSkinSize.height;
		} else if (!(_align & ALIGN_TOP)) offset.top += (_parentHeight - _parentSkinSize.height) / 2;

		return offset;
	}

	ParseDelegate & WidgetManager::registerDelegate(const Ogre::String & _key)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter != mDelegates.end()) assert(0 && "name delegate is exist");
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
		if (iter == mDelegates.end()) assert(0 && "name delegate is not find");
		iter->second(_widget, _key, _value);
	}

} // namespace MyGUI