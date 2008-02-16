/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_SUB_SKIN_H__
#define __MYGUI_SUB_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_CroppedRectangleInterface.h"
#include "MyGUI_DrawItem.h"

namespace MyGUI
{

	class RenderItem;

	class _MyGUIExport SubSkin : public CroppedRectangleInterface, public DrawItem
	{

	public:
		SubSkin(const SubWidgetInfo &_info, CroppedRectanglePtr _parent);
		virtual ~SubSkin();

		inline static const Ogre::String & _getType() {static Ogre::String type("SubSkin"); return type;}

		void setAlpha(float _alpha);

		void show();
		void hide();

		void _updateView();
		void _correctView();

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		void _setUVSet(const FloatRect& _rect);

		virtual void _createDrawItem(RenderItem * _item);
		virtual void _destroyDrawItem();

		// метод для отрисовки себя
		virtual void _drawItem(Vertex * _vertex, size_t & _count);


	protected:

		FloatRect mRectTexture;
		bool mTransparent;

		uint32 mCurrentAlpha;

		FloatRect mCurrentTexture;
		IntCoord mCurrentCoord;

		RenderItem * mRenderItem;

	};

} // namespace MyGUI

#endif // __MYGUI_SUB_SKIN_H__
