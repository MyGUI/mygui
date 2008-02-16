/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_SIMPLE_TEXT_H__
#define __MYGUI_SIMPLE_TEXT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_SubWidgetTextInterface.h"
#include "MyGUI_DrawItem.h"

namespace MyGUI
{

	class RenderItem;

	class _MyGUIExport SimpleText : public SubWidgetTextInterface, public DrawItem
	{

	public:
		SimpleText(const SubWidgetInfo &_info, CroppedRectanglePtr _parent);
		virtual ~SimpleText();

		inline static const Ogre::String & _getType() {static Ogre::String type("SimpleText"); return type;}

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

#endif // __MYGUI_SIMPLE_TEXT_H__
