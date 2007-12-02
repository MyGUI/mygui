/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_MAIN_SKIN_H__
#define __MYGUI_MAIN_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_PanelAlphaOverlayElement.h"
#include "MyGUI_BasisWidget.h"

namespace MyGUI
{

	class _MyGUIExport MainSkin : public BasisWidget
	{

	public:
		MainSkin(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidgetPtr _parent, size_t _id);
		~MainSkin();

		// вызывается виджетом, если этот саб скин во весь виджет
		void update();
		// скрываем показывает оверлей
		void show();
		void hide();
		// корректируем положение скина, нужно при обрезке отца
		void correct();

		void align(int _width, int _height, bool _update);
		void align(int _left, int _top, int _width, int _height, bool _update);

		void attach(BasisWidgetPtr _basis, bool _child);
		Ogre::OverlayElement* getOverlayElement();

		void setUVSet(const FloatRect & _rect);

		void setAlpha(float _alpha);

		inline static const Ogre::String & getType() {static Ogre::String type("MainSkin"); return type;}
		inline static bool isSharedOverlay() {return false;}
		bool isText() {return false;}

	protected:

		PanelAlphaOverlayElement * mOverlayContainer;

		FloatRect mRectTexture;

	}; // class SubSkin

} // namespace MyGUI

#endif // __MYGUI_MAIN_SKIN_H__