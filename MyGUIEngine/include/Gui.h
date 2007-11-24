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
			return createWidget(_type, _skin, (int)(_x*mWidth), (int)(_y*mHeight), (int)(_cx*mWidth), (int)(_cy*mHeight), _align, _layer, _name);
		}

		inline FloatRect convertToReal(const FloatRect & _rect)
		{
			return FloatRect(_rect.left*mWidth, _rect.top*mHeight, _rect.right*mWidth, _rect.bottom*mHeight);
		}

		inline float getWidth() {return mWidth;}
		inline float getHeight() {return mHeight;}

		// ������� ������ �������� ������� ��������
		void destroyWidget(WidgetPtr & _widget);
		// ������� ���� �����
		void destroyWidget();

	private:
		// ������ ���� ����� ��������
		VectorWidgetPtr mWidgetChild;

		float mHeight, mWidth;

		// ������� ����� ��������
		TextSimpleOverlayElementFactory * mFactoryTextSimpleOverlay;
		TextEditOverlayElementFactory * mFactoryTextEditOverlay;
		PanelAlphaOverlayElementFactory * mFactoryPanelAlphaOverlay;

		// ��������� ���
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

	}; // class Gui

} // namespace MyGUI

#endif // __GUI_H__
