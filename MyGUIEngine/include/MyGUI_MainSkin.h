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

		void setAlpha(float _alpha);

		// скрываем показывает оверлей
		void show();
		void hide();

		// вызывается виджетом, если этот саб скин во весь виджет
		void _updateView();
		// корректируем положение скина, нужно при обрезке отца
		void _correctView();

		void _setAlign(int _width, int _height, bool _update);
		void _setAlign(int _left, int _top, int _width, int _height, bool _update);

		void _attachChild(BasisWidgetPtr _basis, bool _child);
		Ogre::OverlayElement* _getOverlayElement();

		void _setUVSet(const FloatRect & _rect);

		inline static const Ogre::String & _getType() {static Ogre::String type("MainSkin"); return type;}
		inline static bool _isSharedOverlay() {return false;}
		bool _isText() {return false;}

	protected:

		PanelAlphaOverlayElement * mOverlayContainer;

		FloatRect mRectTexture;

	}; // class SubSkin

} // namespace MyGUI

#endif // __MYGUI_MAIN_SKIN_H__