/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_MAIN_SKIN_H__
#define __MYGUI_MAIN_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_CroppedRectangleInterface.h"
#include "MyGUI_DrawItem.h"
#include "MyGUI_SubSkin.h"

namespace MyGUI
{

	class RenderItem;

	class _MyGUIExport MainSkin : public SubSkin//CroppedRectangleInterface, public DrawItem
	{

	public:
		MainSkin(const SubWidgetInfo &_info, CroppedRectanglePtr _parent);
		virtual ~MainSkin();

		/*inline static const Ogre::String & _getType() {static Ogre::String type("MainSkin"); return type;}

		void setAlpha(float _alpha);

		void show();
		void hide();

		void _updateView();
		void _correctView();

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		void _setUVSet(const FloatRect& _rect);

		virtual void _createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item);
		virtual void _destroyDrawItem();

		// метод для отрисовки себя
		virtual size_t _drawItem(Vertex * _vertex, bool _update);


	protected:

		FloatRect mRectTexture;
		bool mEmptyView;

		uint32 mCurrentAlpha;

		FloatRect mCurrentTexture;
		IntCoord mCurrentCoord;

		RenderItem * mRenderItem;

		LayerManager * mManager;*/
	};

} // namespace MyGUI

#endif // __MYGUI_SUB_SKIN_H__
