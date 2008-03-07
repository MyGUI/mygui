/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_MAIN_SKIN_H__
#define __MYGUI_MAIN_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_SharedPanelAlphaOverlayElement.h"
#include "MyGUI_CroppedRectangleInterface.h"

namespace MyGUI
{

	class _MyGUIExport MainSkin : public CroppedRectangleInterface
	{

	public:
		MainSkin(const SubWidgetInfo &_info, const Ogre::String & _material, CroppedRectanglePtr _parent, size_t _id);
		~MainSkin();

		void setAlpha(float _alpha);

		// скрываем показывает оверлей
		void show();
		void hide();

		// вызывается виджетом, если этот саб скин во весь виджет
		void _updateView();
		// корректируем положение скина, нужно при обрезке отца
		void _correctView();

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		void _attachChild(CroppedRectanglePtr _basis, bool _child);
		Ogre::OverlayElement* _getOverlayElement();

		void _setUVSet(const FloatRect& _rect);
		void _setMaterialName(const Ogre::String& _material);

		inline static const Ogre::String & _getType() {static Ogre::String type("MainSkin"); return type;}
		inline static bool _isSharedOverlay() {return false;}
		bool _isText() {return false;}

	protected:

		SharedPanelAlphaOverlayElement * mOverlayContainer;

		FloatRect mRectTexture;

	}; // class _MyGUIExport MainSkin : public SubWidgetSkinInterface

} // namespace MyGUI

#endif // __MYGUI_MAIN_SKIN_H__
