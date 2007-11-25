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

#include "MyGUI_InputManager.h"
#include "MyGUI_BasisWidgetManager.h"
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
	
	class _MyGUIExport Gui : public Ogre::FrameListener
	{
		friend WidgetManager;
		INSTANCE_HEADER(Gui);

		typedef std::list<Ogre::FrameListener*> ListFrameListener;

	public:
		void initialise(Ogre::RenderWindow* _window);
		void shutdown();

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _left, int _top, int _width, int _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _left, float _top, float _width, float _height, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidget(_type, _skin, (int)(_left*mWidth), (int)(_top*mHeight), (int)(_width*mWidth), (int)(_height*mHeight), _align, _layer, _name);
		}

		inline FloatRect convertToReal(const FloatRect & _rect)
		{
			return FloatRect(_rect.left*mWidth, _rect.top*mHeight, _rect.right*mWidth, _rect.bottom*mHeight);
		}

		inline float getWidth() {return mWidth;}
		inline float getHeight() {return mHeight;}

		// уд€л€ет только негодных батюшке государю
		void destroyWidget(WidgetPtr & _widget);
		// удал€ет всех детей
		void destroyAllWidget();

		// подписка на кадры
		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		void addFrameListener(Ogre::FrameListener * _listener);
		void removeFrameListener(Ogre::FrameListener * _listener);

	private:
		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;

		float mHeight, mWidth;

		// фабрики наших оверлеев
		TextSimpleOverlayElementFactory * mFactoryTextSimpleOverlay;
		TextEditOverlayElementFactory * mFactoryTextEditOverlay;
		PanelAlphaOverlayElementFactory * mFactoryPanelAlphaOverlay;

		// синглтоны гу€
		InputManager * mInputManager;
		BasisWidgetManager * mBasisWidgetManager;
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