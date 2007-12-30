/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_PointerManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(WidgetManager);

	void WidgetManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise);
		MYGUI_LOG("* Initialise: ", INSTANCE_TYPE_NAME);

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
		mStretchRectangleFactory = new factory::StretchRectangleFactory();
		mComboBoxFactory = new factory::ComboBoxFactory();

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully initialized");
		mIsInitialise = true;
	}

	void WidgetManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG("* Shutdown: ", INSTANCE_TYPE_NAME);

		mFactoryList.clear();
		mDelegates.clear();

		delete mWindowFactory;
		delete mHScrollFactory;
		delete mVScrollFactory;
		delete mStaticTextFactory;
		delete mStaticImageFactory;
		delete mListFactory;
		delete mEditFactory;
		delete mButtonFactory;
		delete mWidgetFactory;
		delete mStretchRectangleFactory;
		delete mComboBoxFactory;

		MYGUI_LOG(INSTANCE_TYPE_NAME, " successfully shutdown");
		mIsInitialise = false;
	}

	void WidgetManager::registerFactory(WidgetFactoryInterface * _factory)
	{
		mFactoryList.insert(_factory);
		MYGUI_LOG("* Register widget factory '", _factory->getType(), "'");
	}

	void WidgetManager::unregisterFactory(WidgetFactoryInterface * _factory)
	{
		SetWidgetFactory::iterator iter = mFactoryList.find(_factory);
		if (iter != mFactoryList.end()) mFactoryList.erase(iter);
		MYGUI_LOG("* Unregister widget factory '", _factory->getType(), "'");
	}

	WidgetPtr WidgetManager::createWidget(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String & _name)
	{
		Ogre::String name;
		if (false == _name.empty()) {
			MapWidgetPtr::iterator iter = mWidgets.find(_name);
			if (iter != mWidgets.end()) MYGUI_EXCEPT(_name + " - name widget is exist");
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
		MYGUI_EXCEPT(_type + " - no find factory WidgetFactory");
		return 0;
	}

	WidgetPtr WidgetManager::findWidgetT(const Ogre::String & _name)
	{
		MapWidgetPtr::iterator iter = mWidgets.find(_name);
		if (iter == mWidgets.end()){
			MYGUI_LOG("Error: Widget \"" , _name , "\" not found");
			return null;
		}
		return iter->second;
	}

	void WidgetManager::unlinkWidget(WidgetPtr _widget)
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
		MYGUI_ASSERT(iter == mDelegates.end() && "name delegate is exist");
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
		MYGUI_ASSERT(iter != mDelegates.end() && "name delegate is not find");
		iter->second(_widget, _key, _value);
	}

	void WidgetManager::destroyWidget(WidgetPtr _widget)
	{
		// иначе возможен бесконечный цикл
		MYGUI_ASSERT(_widget != null);

		// отсоединяем виджет от уровня, если он был присоединен
		LayerManager::getInstance().detachItem(_widget);

		// стираем имя в карте для поиска
		unlinkWidget(_widget);

		// удаляем упоминание в инпуте
		InputManager::getInstance().unlinkWidget(_widget);

		// удаляем упоминание в курсорах
		PointerManager::getInstance().unlinkWidget(_widget);

		// делегирует удаление отцу виджета
		WidgetPtr parent = _widget->getParent();
		if (parent == null) Gui::getInstance()._destroyChildWidget(_widget);
		else parent->_destroyChildWidget(_widget);

		//_widget = null;
	}

	void WidgetManager::destroyAllWidget()
	{
		Gui::getInstance()._destroyAllChildWidget();
	}

} // namespace MyGUI
