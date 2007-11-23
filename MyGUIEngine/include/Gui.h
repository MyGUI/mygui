#ifndef __GUI_H__
#define __GUI_H__

#include "Prerequest.h"
#include "Instance.h"
#include "Common.h"

#include "TextSimpleOverlayElementFactory.h"
#include "TextEditOverlayElementFactory.h"
#include "PanelAlphaOverlayElementFactory.h"

#include "InputManager.h"
#include "BasisWidgetManager.h"
#include "ClipboardManager.h"
#include "LayerManager.h"
#include "SkinManager.h"
#include "WidgetManager.h"
#include "LayoutManager.h"
#include "MyGUI_FontManager.h"
#include "ParserManager.h"
#include "PointerManager.h"
#include "PluginManager.h"
#include "DynLibManager.h"

namespace MyGUI
{
	
	class _MyGUIExport Gui
	{
		friend WidgetManager;
		INSTANCE_HEADER(Gui);

	public:
		void initialise(Ogre::RenderWindow* _window);
		void shutdown();

		WidgetPtr createWidget(const Ogre::String & _type, const Ogre::String & _skin, int _x, int _y, int _cx, int _cy, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "");
		inline WidgetPtr createWidgetReal(const Ogre::String & _type, const Ogre::String & _skin, float _x, float _y, float _cx, float _cy, Align _align, const Ogre::String & _layer, const Ogre::String & _name = "")
		{
			return createWidget(_type, _skin, (int)(_x*m_width), (int)(_y*m_height), (int)(_cx*m_width), (int)(_cy*m_height), _align, _layer, _name);
		}

		inline FloatRect convertToReal(const FloatRect & _rect)
		{
			return FloatRect(_rect.left*m_width, _rect.top*m_height, _rect.right*m_width, _rect.bottom*m_height);
		}

		inline float getWidth() {return m_width;}
		inline float getHeight() {return m_height;}

		// уд€л€ет только негодных батюшке государю
		void destroyWidget(WidgetPtr & _widget);
		// удал€ет всех детей
		void destroyWidget();

	private:
		// вектор всех детей виджетов
		VectorWidgetPtr m_widgetChild;

		float m_height, m_width;

		// фабрики наших оверлеев
		TextSimpleOverlayElementFactory m_factoryTextSimpleOverlay;
		TextEditOverlayElementFactory m_factoryTextEditOverlay;
		PanelAlphaOverlayElementFactory m_factoryPanelAlphaOverlay;

		// синглтоны гу€
		InputManager * mInputManager;
		BasisWidgetManager * mBasisWidgetManager;
		ClipboardManager* mClipboardManager;
		LayerManager* mLayerManager;
		SkinManager* mSkinManager;
		WidgetManager* mWidgetManager;
		LayoutManager* mLayoutManager;
		FontManager* mFontManager;
		ParserManager* mParserManager;
		PointerManager* mPointerManager;
		DynLibManager* mDynLibManager;
		PluginManager* mPluginManager;

	}; // class Gui

} // namespace MyGUI

#endif // __GUI_H__
