/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_Widget.h"

#include "MyGUI_InputManager.h"
#include "MyGUI_SubWidgetManager.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	const std::string XML_TYPE("List");

	INSTANCE_IMPLEMENT(Gui);

	void Gui::initialise(Ogre::RenderWindow* _window, const std::string& _core, const Ogre::String & _group)
	{
		// самый первый лог
		LogManager::registerSection(MYGUI_LOG_SECTION, MYGUI_LOG_FILENAME);

		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");

		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);
		MYGUI_LOG(Info, "* MyGUI version "
			<< MYGUI_VERSION_MAJOR << "."
			<< MYGUI_VERSION_MINOR << "."
			<< MYGUI_VERSION_PATCH);

		// сохран€ем окно и размеры
		mWindow = _window;
		mViewSize.set(mWindow->getViewport(0)->getActualWidth(), mWindow->getViewport(0)->getActualHeight());

		MYGUI_LOG(Info, "Viewport : " << mViewSize.print());

		registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &Gui::_load);

		// создаем и инициализируем синглтоны
		mRenderManager = new LayerManager();
		mWidgetManager = new WidgetManager();
		mInputManager = new InputManager();
		mCroppedRectangleManager = new SubWidgetManager();
		mSkinManager = new SkinManager();

		mRenderManager->initialise();
		mWidgetManager->initialise();
		mInputManager->initialise();
		mCroppedRectangleManager->initialise();
		mSkinManager->initialise();

		// подписываемс€ на изменение размеров окна и сразу оповещаем
		Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
		windowResized(mWindow);

		// загружаем дефолтные настройки
		load(_core, _group);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void Gui::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		// сразу отписываемс€
		Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);

		unregisterLoadXmlDelegate(XML_TYPE);
		mListFrameListener.clear();
		mMapLoadXmlDelegate.clear();

		_destroyAllChildWidget();

		// деинициализируем и удал€ем синглтоны
		mWidgetManager->shutdown();
		mInputManager->shutdown();
		mSkinManager->shutdown();
		mCroppedRectangleManager->shutdown();
		mRenderManager->shutdown();

		delete mWidgetManager;
		delete mInputManager;
		delete mSkinManager;
		delete mCroppedRectangleManager;
		delete mRenderManager;

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");

		// самый последний лог
		LogManager::shutdown();

		mIsInitialise = false;
	}

	void Gui::addFrameListener(FrameListener * _listener)
	{
		if (null == _listener) return;

		removeFrameListener(_listener);
		mListFrameListener.push_back(_listener);
	}

	void Gui::removeFrameListener(FrameListener * _listener)
	{
		if (null == _listener) return;

		for (ListFrameListener::iterator iter=mListFrameListener.begin(); iter!=mListFrameListener.end(); ++iter) {
			if ((*iter) == _listener) {
				(*iter) = null;
				return;
			}
		}
	}

	void Gui::injectFrameEntered(Ogre::Real timeSinceLastFrame)
	{
		// сначала рассылаем
		ListFrameListener::iterator iter=mListFrameListener.begin();
		while (iter != mListFrameListener.end()) {
			if (null == (*iter)) iter = mListFrameListener.erase(iter);
			else {
				(*iter)->_frameEntered(timeSinceLastFrame);
				++iter;
			}
		};
	}

	bool Gui::injectMouseMove( const OIS::MouseEvent & _arg) {return mInputManager->injectMouseMove(_arg);}
	bool Gui::injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id ) {return mInputManager->injectMousePress(_arg, _id);}
	bool Gui::injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id ) {return mInputManager->injectMouseRelease(_arg, _id);}

	bool Gui::injectKeyPress(const OIS::KeyEvent & _arg) {return mInputManager->injectKeyPress(_arg);}
	bool Gui::injectKeyRelease(const OIS::KeyEvent & _arg) {return mInputManager->injectKeyRelease(_arg);}

	WidgetPtr Gui::createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name)
	{
		WidgetPtr widget = WidgetManager::getInstance().createWidget(_type, _skin, _coord, _align, null, _name);
		mWidgetChild.push_back(widget);
		// присоедин€ем виджет с уровню
		if (false == _layer.empty()) LayerManager::getInstance().attachToLayerKeeper(_layer, widget);
		return widget;
	}

	WidgetPtr Gui::findWidgetT(const std::string& _name)
	{
		return mWidgetManager->findWidgetT(_name);
	}

	LoadXmlDelegate & Gui::registerLoadXmlDelegate(const Ogre::String & _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		MYGUI_ASSERT(iter == mMapLoadXmlDelegate.end(), "name delegate is exist");
		return (mMapLoadXmlDelegate[_key] = LoadXmlDelegate());
	}

	void Gui::unregisterLoadXmlDelegate(const Ogre::String & _key)
	{
		MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(_key);
		if (iter != mMapLoadXmlDelegate.end()) mMapLoadXmlDelegate.erase(iter);
	}

	// уд€л€ет только негодных батюшке государю
	void Gui::_destroyChildWidget(WidgetPtr _widget)
	{
		VectorWidgetPtr::iterator iter = std::find(mWidgetChild.begin(), mWidgetChild.end(), _widget);
		if(iter != mWidgetChild.end())
		{
			delete *iter;
			// удал€ем из списка
			*iter = mWidgetChild.back();
			mWidgetChild.pop_back();
			return;
		}
		MYGUI_EXCEPT("Widget not found");
	}

	// удал€ет всех детей
	void Gui::_destroyAllChildWidget()
	{
		while (false == mWidgetChild.empty()) {
			// отсылаем первый, так как он быстрее найдетс€ в массиве
			// а удаление в векторе производитс€ перестановкой, т.е. быстро
			WidgetPtr widget = mWidgetChild.front();
			WidgetManager::getInstance().destroyWidget(widget);
		}
	}

	void Gui::destroyAllWidget()
	{
		mWidgetManager->destroyAllWidget();
	}

	void Gui::destroyWidget(WidgetPtr _widget)
	{
		mWidgetManager->destroyWidget(_widget);
	}

	bool Gui::load(const std::string & _file, const std::string & _group)
	{
		return _loadImplement(_file, _group, false, "", INSTANCE_TYPE_NAME);
	}

	bool Gui::_loadImplement(const std::string & _file, const std::string & _group, bool _match, const std::string & _type, const std::string & _instance)
	{
		xml::xmlDocument doc;
		std::string file(_group.empty() ? _file : helper::getResourcePath(_file, _group));
		if (file.empty()) {
			MYGUI_LOG(Error, _instance << " : '" << _file << "' not found");
			return false;
		}
		if (false == doc.open(file)) {
			MYGUI_LOG(Error, _instance << " : " << doc.getLastError());
			return false;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (null == root) || (root->getName() != "MyGUI") ) {
			MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'MyGUI' not found");
			return false;
		}

		std::string type;
		if (root->findAttribute("type", type)) {
			MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
			if (iter != mMapLoadXmlDelegate.end()) {
				if ((false == _match) || (type == _type)) (*iter).second(root, file);
				else {
					MYGUI_LOG(Error, _instance << " : '" << _file << "', type '" << _type << "' not found");
					return false;
				}
			}
			else {
				MYGUI_LOG(Error, _instance << " : '" << _file << "', delegate for type '" << type << "'not found");
				return false;
			}
		}
		// предпологаем что будут вложенные
		else if (false == _match) {
			xml::xmlNodeIterator node = root->getNodeIterator();
			while (node.nextNode("MyGUI")) {
				if (node->findAttribute("type", type)) {
					MapLoadXmlDelegate::iterator iter = mMapLoadXmlDelegate.find(type);
					if (iter != mMapLoadXmlDelegate.end()) {
						(*iter).second(node.currentNode(), file);
					}
					else {
						MYGUI_LOG(Error, _instance << " : '" << _file << "', delegate for type '" << type << "'not found");
					}
				}
				else {
					MYGUI_LOG(Error, _instance << " : '" << _file << "', tag 'type' not found");
				}
			}
		}

		return true;
	}

	void Gui::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		// берем детей и крутимс€, основной цикл
		xml::xmlNodeIterator node = _node->getNodeIterator();
		while (node.nextNode(XML_TYPE)) {
			std::string source;
			if (false == node->findAttribute("file", source)) continue;
			std::string group = node->findAttribute("group");
			MYGUI_LOG(Info, "Load ini file '" << source << "' from " << (group.empty() ? "current path" : "resource group : ") << group);
			_loadImplement(source, group, false, "", INSTANCE_TYPE_NAME);
		};
	}

	IntCoord Gui::convertRelativeToInt(const FloatCoord& _coord, WidgetPtr _parent)
	{
		const FloatSize& size = getViewSize();
		if (null == _parent) {
			return IntCoord(_coord.left * size.width, _coord.top * size.height, _coord.width * size.width, _coord.height * size.height);
		}
		const IntCoord& coord = _parent->getClientRect();
		return IntCoord(_coord.left * coord.width, _coord.top * coord.height, _coord.width * coord.width, _coord.height * coord.height);
	}

	FloatCoord Gui::convertIntToRelative(const IntCoord& _coord, WidgetPtr _parent)
	{
		const FloatSize& size = getViewSize();
		if (null == _parent) {
			return FloatCoord(_coord.left / size.width, _coord.top / size.height, _coord.width / size.width, _coord.height / size.height);
		}
		const IntCoord& coord = _parent->getClientRect();
		return FloatCoord(_coord.left / coord.width, _coord.top / coord.height, _coord.width / coord.width, _coord.height / coord.height);
	}

	// дл€ оповещений об изменении окна рендера
	void Gui::windowResized(Ogre::RenderWindow* rw)
	{
		Ogre::Viewport * port = rw->getViewport(0);
		mViewSize.set(port->getActualWidth(), port->getActualHeight());

		mRenderManager->_windowResized(mViewSize);
	}

} // namespace MyGUI
