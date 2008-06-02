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
#include "MyGUI_WidgetCreator.h"

#include "MyGUI_WidgetOIS.h"

namespace MyGUI
{

	typedef delegates::CDelegate2<xml::xmlNodePtr, const std::string &> LoadXmlDelegate;
	typedef std::map<Ogre::String, LoadXmlDelegate> MapLoadXmlDelegate;

	class _MyGUIExport Gui : public Ogre::WindowEventListener, public WidgetCreator
	{
		friend class WidgetManager;
		INSTANCE_HEADER(Gui);

	public:
		/** Initialise GUI and all GUI Managers
			@param
				_window where GUI will be drawn
			@param
				_core name of core config file for MyGUI (contain main config files with skins, layers, fonts, etc.)
			@param
				_group OgreResourceGroup where _core and all other config and GUI resource files are
		*/
		void initialise(Ogre::RenderWindow* _window, const std::string& _core = "core.xml", const Ogre::String & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		/** Shutdown GUI and all GUI Managers*/
		void shutdown();

		// methods for creating widgets
		/** Create widget
			@param _type widget type
			@param _skin widget skin
			@param _coord int coordinates of widget (_left, _top, _width, _height)
			@param _align widget align (possible values can be found in enum ALIGN_INFO)
			@param _layer layer where widget will be created (all layers usually defined in core.layer file).
				If your widget will overlap with any other you shoud select _layer with "overlapped" property enabled.
			@param _name if needed (you can use it for finding widget by name later)
		*/
		inline WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return _createWidget(_type, _skin, _coord, _align, _layer, _name);
		}
		/** See Gui::createWidgetT */
		inline WidgetPtr createWidgetT(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidgetT(_type, _skin, IntCoord(_left, _top, _width, _height), _align, _layer, _name);
		}
		/** Create widget using coordinates relative to parent. see Gui::createWidgetT */
		inline WidgetPtr createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidgetT(_type, _skin, IntCoord((int)(_coord.left*mViewSize.width), (int)(_coord.top*mViewSize.height), (int)(_coord.width*mViewSize.width), (int)(_coord.height*mViewSize.height)), _align, _layer, _name);
		}
		/** Create widget using coordinates relative to parent. see Gui::createWidgetT */
		inline WidgetPtr createWidgetRealT(const Ogre::String & _type, const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidgetT(_type, _skin, IntCoord((int)(_left*mViewSize.width), (int)(_top*mViewSize.height), (int)(_width*mViewSize.width), (int)(_height*mViewSize.height)), _align, _layer, _name);
		}

		// templates for creating widgets by type
		/** Same as Gui::createWidgetT but return T* instead of WidgetPtr */
		template <class T> inline T* createWidget(const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetT(T::_getType(), _skin, _coord, _align, _layer, _name));
		}
		/** Same as Gui::createWidgetT but return T* instead of WidgetPtr */
		template <class T> inline T* createWidget(const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetT(T::_getType(), _skin, IntCoord(_left, _top, _width, _height), _align, _layer, _name));
		}
		/** Same as Gui::createWidgetRealT but return T* instead of WidgetPtr */
		template <class T> inline T* createWidgetReal(const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::_getType(), _skin, _coord, _align, _layer, _name));
		}
		/** Same as Gui::createWidgetRealT but return T* instead of WidgetPtr */
		template <class T> inline T* createWidgetReal(const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return static_cast<T*>(createWidgetRealT(T::_getType(), _skin, _left, _top, _width, _height, _align, _layer, _name));
		}

		/** Get width of GUI area */
		inline float getViewWidth() {return mViewSize.width;}
		/** Get height of GUI area */
		inline float getViewHeight() {return mViewSize.height;}
		/** Get aspect of GUI area */
		inline float getViewAspect() {return mViewSize.width / mViewSize.height;}
		/** Get view size of GUI area */
		inline const FloatSize& getViewSize() {return mViewSize;}

		/** Add GUI frame listener */
		void addFrameListener(FrameListener * _listener);
		/** Remove GUI frame listener */
		void removeFrameListener(FrameListener * _listener);

		/** Inject frame entered event.
			This function should be called every frame.
		*/
		void injectFrameEntered(Ogre::Real timeSinceLastFrame);

		// mirror of InputManager methods
		/** Inject MouseMove event
			@return true if event has been processed by GUI
		*/
		bool injectMouseMove(int _absx, int _absy, int _absz);
		/** Inject MousePress event
			@return true if event has been processed by GUI
		*/
		bool injectMousePress(int _absx, int _absy, MouseButton _id);
		/** Inject MouseRelease event
			@return true if event has been processed by GUI
		*/
		bool injectMouseRelease(int _absx, int _absy, MouseButton _id);

		/** Inject KeyPress event
			@return true if event has been processed by GUI
		*/
		bool injectKeyPress(KeyCode _key);
		/** Inject KeyReleas event
			@return true if event has been processed by GUI
		*/
		bool injectKeyRelease(KeyCode _key);

#ifndef MYGUI_NO_OIS
		/** OIS backend for injectMouseMove(int _absx, int _absy, int _absz) */
		bool injectMouseMove( const OIS::MouseEvent & _arg) {return injectMouseMove(_arg.state.X.abs, _arg.state.Y.abs, _arg.state.Z.abs);}
		/** OIS backend injectMousePress(int _absx, int _absy, MouseButton _id) */
		bool injectMousePress( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id ) {return injectMousePress(_arg.state.X.abs, _arg.state.Y.abs, (MouseButton)_id);}
		/** OIS backend for injectMouseRelease(int _absx, int _absy, MouseButton _id) */
		bool injectMouseRelease( const OIS::MouseEvent & _arg , OIS::MouseButtonID _id ) {return injectMouseRelease(_arg.state.X.abs, _arg.state.Y.abs, (MouseButton)_id);}

		/** OIS backend for injectKeyPress(KeyCode _key) */
		bool injectKeyPress(const OIS::KeyEvent & _arg) {return injectKeyPress((KeyCode)_arg.key);}
		/** OIS backend for injectKeyRelease(KeyCode _key) */
		bool injectKeyRelease(const OIS::KeyEvent & _arg) {return injectKeyRelease((KeyCode)_arg.key);}
#endif

		// mirror of WidgetManager method
		/** Destroy any created widget */
		void destroyWidget(WidgetPtr _widget);

		// mirror of WidgetManager method
		/** Destroy vector of widgets */
		void destroyWidgetsVector(VectorWidgetPtr _widgets);

		// mirror of WidgetManager method
		/** Find widget by name */
		WidgetPtr findWidgetT(const std::string& _name);
		/** Find widget by name and prefix */
		inline WidgetPtr findWidgetT(const std::string& _name, const std::string& _prefix)
		{
			return findWidgetT(_prefix + _name);
		}

		// mirror WidgetManager
		/** Find widget by name and cast it to T type.
			If T and found widget have different types cause exception in DEBUG mode.
		*/
		template <class T> inline T* findWidget(const std::string& _name)
		{
			WidgetPtr widget = findWidgetT(_name);
			if (null == widget) return null;
			return castWidget<T>(widget);
		}

		/** Find widget by name and prefix and cast it to T type*/
		template <class T> inline T* findWidget(const std::string& _name, const std::string& _prefix)
		{
			return findWidget<T>(_prefix + _name);
		}

		// mirror PointerManager
		/** Hide pointer */
		void hidePointer();

		// mirror PointerManager
		/** Show pointer */
		void showPointer();

		// mirror PointerManager
		/** Return visibility of pointer */
		bool isShowPointer();

		/** Register delegate for parsing XML blocks */
		LoadXmlDelegate & registerLoadXmlDelegate(const Ogre::String & _key);
		void unregisterLoadXmlDelegate(const Ogre::String & _key);

		/** Load config with any info (file can have different data such other config files that will be loaded, skins, layers, pointers, etc) */
		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		void _load(xml::xmlNodePtr _node, const std::string & _file);

		bool _loadImplement(const std::string & _file, const std::string & _group, bool _match, const std::string & _type, const std::string & _instance);

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

		/** Ogre::WindowEventListener method */
		virtual void windowResized(Ogre::RenderWindow* rw);

		/** Destroy child widget or throw exception if this child widget not found */
		inline void destroyChildWidget(WidgetPtr _widget)
		{
			_destroyChildWidget(_widget);
		}

		/** Destroy all child widgets */
		inline void destroyAllChildWidget()
		{
			_destroyAllChildWidget();
		}

		/** Get name of Gui ResourceGroup*/
		inline const std::string& getResourceGroup()
		{
			return mResourceGroup;
		}

		/** Get GUI viewport index */
		inline Ogre::ushort getActiveViewport()
		{
			return mActiveViewport;
		}

		/** Set GUI viewport index */
		void setActiveViewport(Ogre::ushort _num);

		// mirror LayerManager
		/** Set scene manager where MyGUI will be rendered */
		void setSceneManager(Ogre::SceneManager * _scene);

		inline Ogre::RenderWindow * getRenderWindow() { return mWindow; }

	private:
		// создает виджет
		virtual WidgetPtr _createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		// уд€л€ет неудачника
		void _destroyChildWidget(WidgetPtr _widget);

		// удал€ет всех детей
		void _destroyAllChildWidget();

		void _alignWidget(WidgetPtr _widget, const FloatSize& _old, const FloatSize& _new);

	private:
		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;
		// карта с делегатами дл€ парсинга хмл блоков
		MapLoadXmlDelegate mMapLoadXmlDelegate;

		// размеры экрана
		FloatSize mViewSize;

		// синглтоны гу€
		InputManager * mInputManager;
		SubWidgetManager * mCroppedRectangleManager;
		LayerManager* mLayerManager;
		SkinManager* mSkinManager;
		WidgetManager* mWidgetManager;
		FontManager* mFontManager;
		ControllerManager* mControllerManager;
		PointerManager* mPointerManager;
		ClipboardManager* mClipboardManager;
		LayoutManager* mLayoutManager;
		DynLibManager* mDynLibManager;
		PluginManager* mPluginManager;

		// подписчики на кадры
		ListFrameListener mListFrameListener;

		// окно, на которое мы подписываемс€ дл€ изменени€ размеров
		Ogre::RenderWindow* mWindow;

		std::string mResourceGroup;

		// вьюпорт, с которым работает система
		Ogre::ushort mActiveViewport;

	}; // class Gui

} // namespace MyGUI

#endif // __MYGUI_GUI_H__
