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

#include "MyGUI_TextSimpleOverlayElementFactory.h"
#include "MyGUI_TextEditOverlayElementFactory.h"
#include "MyGUI_PanelAlphaOverlayElementFactory.h"
#include "MyGUI_SharedPanelAlphaOverlayElementFactory.h"

#include "MyGUI_InputManager.h"
#include "MyGUI_SubWidgetManager.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayoutManager.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_PluginManager.h"
#include "MyGUI_DynLibManager.h"

namespace MyGUI
{
	
	class _MyGUIExport Gui
	{
		friend WidgetManager;
		INSTANCE_HEADER(Gui);

		typedef std::list<WidgetPtr> ListFrameListener;

	public:
		void initialise(Ogre::RenderWindow* _window);
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

		// шаблоны дл€ создани€ виджета по типу
		template <class T>
		inline T* createWidget(const Ogre::String & _skin, const IntCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			Widget * widget = createWidgetT(T::getType(), _skin, _coord, _align, _layer, _name);
			MYGUI_TYPE(T*, widget);
			return static_cast<T*>(widget);
		}
		template <class T>
		inline T* createWidget(const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			Widget * widget = createWidgetT(T::getType(), _skin, IntCoord(_left, _top, _width, _height), _align, _layer, _name);
			MYGUI_TYPE(T*, widget);
			return static_cast<T*>(widget);
		}

		template <class T>
		inline T* createWidgetReal(const Ogre::String & _skin, const FloatCoord& _coord, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			Widget * widget = createWidgetRealT(T::getType(), _skin, _coord, _align, _layer, _name);
			MYGUI_TYPE(T*, widget);
			return static_cast<T*>(widget);
		}
		template <class T>
		inline T* createWidgetReal(const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			Widget * widget = createWidgetRealT(T::getType(), _skin, _left, _top, _width, _height, _align, _layer, _name);
			MYGUI_TYPE(T*, widget);
			return static_cast<T*>(widget);
		}


		inline float getViewWidth() {return mViewSize.width;}
		inline float getViewHeight() {return mViewSize.height;}
		inline const FloatSize& getViewSize() {return mViewSize;}

		// подписка на кадры
		void injectFrameEntered(Ogre::Real timeSinceLastFrame);

		bool addFrameListener(WidgetPtr _listener);
		bool removeFrameListener(WidgetPtr _listener);

	private:
		// уд€л€ет только негодных батюшке государю
		void _destroyChildWidget(WidgetPtr _widget);
		// удал€ет всех детей
		void _destroyAllChildWidget();

	private:
		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;

		// размеры экрана
		FloatSize mViewSize;

		// фабрики наших оверлеев
		TextSimpleOverlayElementFactory * mFactoryTextSimpleOverlay;
		TextEditOverlayElementFactory * mFactoryTextEditOverlay;
		PanelAlphaOverlayElementFactory * mFactoryPanelAlphaOverlay;
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

		ListFrameListener mListFrameListener;
		ListFrameListener mListFrameListenerAdd;

	}; // class Gui

} // namespace MyGUI

#endif // __MYGUI_GUI_H__
