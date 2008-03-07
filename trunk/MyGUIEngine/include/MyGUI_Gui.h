/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_GUI_H__
#define __MYGUI_GUI_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Common.h"
#include "MyGUI_CastWidget.h"
#include "MyGUI_FrameListener.h"
#include "MyGUI_XmlDocument.h"

#include "MyGUI_WidgetOIS.h"

#include "MyGUI_TextSimpleOverlayElementFactory.h"
#include "MyGUI_TextEditOverlayElementFactory.h"
#include "MyGUI_SharedPanelAlphaOverlayElementFactory.h"

namespace MyGUI
{

	typedef delegates::CDelegate2<xml::xmlNodePtr, const std::string &> LoadXmlDelegate;
	typedef std::map<Ogre::String, LoadXmlDelegate> MapLoadXmlDelegate;

	class _MyGUIExport Gui : public Ogre::WindowEventListener
	{
		friend class WidgetManager;
		INSTANCE_HEADER(Gui);

	public:
		void initialise(Ogre::RenderWindow* _window, const std::string& _core = "core.xml", const Ogre::String & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void shutdown();

		// методы и шаблоны дл€ создани€ виджета
		WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidgetT(_type, _skin, IntCoord(_left, _top, _width, _height), _align, _layer, _name);
		}
		inline WidgetPtr createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidgetT(_type, _skin, IntCoord((int)(_coord.left*mViewSize.width), (int)(_coord.top*mViewSize.height), (int)(_coord.width*mViewSize.width), (int)(_coord.height*mViewSize.height)), _align, _layer, _name);
		}
		inline WidgetPtr createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidgetT(_type, _skin, IntCoord((int)(_left*mViewSize.width), (int)(_top*mViewSize.height), (int)(_width*mViewSize.width), (int)(_height*mViewSize.height)), _align, _layer, _name);
		}

		// создание с указанием типа
		template <class T> inline T* createWidget(const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetT(T::_getType(), _skin, _coord, _align, _layer, _name));
		}
		template <class T> inline T* createWidget(const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetT(T::_getType(), _skin, IntCoord(_left, _top, _width, _height), _align, _layer, _name));
		}
		template <class T> inline T* createWidgetReal(const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::_getType(), _skin, _coord, _align, _layer, _name));
		}
		template <class T> inline T* createWidgetReal(const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::_getType(), _skin, _left, _top, _width, _height, _align, _layer, _name));
		}

		inline float getViewWidth() {return mViewSize.width;}
		inline float getViewHeight() {return mViewSize.height;}
		inline float getViewAspect() {return mViewSize.width / mViewSize.height;}
		inline const FloatSize& getViewSize() {return mViewSize;}

		void addFrameListener(FrameListener * _listener);
		void removeFrameListener(FrameListener * _listener);

		// подписка на кадры
		void injectFrameEntered(Ogre::Real timeSinceLastFrame);

		// mirror InputManager
		bool injectMouseMove( const OIS::MouseEvent & _arg);
		bool injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );
		bool injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id );

		bool injectKeyPress(const OIS::KeyEvent & _arg);
		bool injectKeyRelease(const OIS::KeyEvent & _arg);

		void destroyAllWidget();
		void destroyWidget(WidgetPtr _widget);

		// mirror WidgetManager
		WidgetPtr findWidgetT(const std::string& _name);
		template <class T> inline T* findWidget(const std::string& _name)
		{
			WidgetPtr widget = findWidgetT(_name);
			if (null == widget) return null;
			MYGUI_DEBUG_ASSERT(null != dynamic_cast<T*>(widget),
				"Error dynamic cast : dest type = '" << T::_getType() 
				<< "' source name = '" << widget->getName() 
				<< "' source type = '" << widget->getWidgetType() << "'");
			return static_cast<T*>(widget);
		}

		// mirror PointerManager
		void showPointer();
		void hidePointer();
		bool isShowPointer();

		// регестрирует делегат дл€ парсинга блоков
		LoadXmlDelegate & registerLoadXmlDelegate(const Ogre::String & _key);
		void unregisterLoadXmlDelegate(const Ogre::String & _key);

		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

		bool _loadImplement(const std::string & _file, const std::string & _group, bool _match, const std::string & _type, const std::string & _instance);

		// mirror LayoutManager
		VectorWidgetPtr loadLayout(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		/* Convert from relative to pixel coordinates.
			@param
				_coord relative coordinates. (relative to _parent client area coordinates)
			@param
				_parent Widget.
		*/
		IntCoord convertRelativeToInt(const FloatCoord & _coord, WidgetPtr _parent);
		/* Convert from pixel to relative coordinates.
			@param
				_coord relative coordinates. (relative to _parent client area coordinates)
			@param
				_parent Widget.
		*/
		FloatCoord convertIntToRelative(const IntCoord & _coord, WidgetPtr _parent);

		// дл€ оповещений об изменении окна рендера
		virtual void windowResized(Ogre::RenderWindow* rw);

	private:
		// уд€л€ет только негодных батюшке государю
		void _destroyChildWidget(WidgetPtr _widget);
		// удал€ет всех детей
		void _destroyAllChildWidget();

		// выравнивает рутовые виджеты
		void _alignWidget(WidgetPtr _widget, const FloatSize& _old, const FloatSize& _new);

	private:
		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;
		// карта с делегатами дл€ парсинга хмл блоков
		MapLoadXmlDelegate mMapLoadXmlDelegate;

		// размеры экрана
		FloatSize mViewSize;

		// фабрики наших оверлеев
		TextSimpleOverlayElementFactory * mFactoryTextSimpleOverlay;
		TextEditOverlayElementFactory * mFactoryTextEditOverlay;
		SharedPanelAlphaOverlayElementFactory * mFactorySharedPanelAlphaOverlay;

		// синглтоны гу€
		InputManager * mInputManager;
		SubWidgetManager * mCroppedRectangleManager;
		ClipboardManager* mClipboardManager;
		LayerManager* mLayerManager;
		SkinManager* mSkinManager;
		WidgetManager* mWidgetManager;
		LayoutManager* mLayoutManager;
		FontManager* mFontManager;
		PointerManager* mPointerManager;
		DynLibManager* mDynLibManager;
		PluginManager* mPluginManager;
		ControllerManager* mControllerManager;

		// подписчики на кадры
		ListFrameListener mListFrameListener;

		Ogre::RenderWindow * mWindow;

	}; // class Gui

} // namespace MyGUI

#endif // __MYGUI_GUI_H__
