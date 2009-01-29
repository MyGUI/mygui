/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_SUB_SKIN_H__
#define __MYGUI_SUB_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_ISubWidgetRect.h"
#include "MyGUI_WidgetSkinInfo.h"

namespace MyGUI
{

	class RenderItem;

	class MYGUI_EXPORT SubSkin : public ISubWidgetRect
	{
		MYGUI_RTTI_CHILD_HEADER(SubSkin, ISubWidgetRect);

	public:
		SubSkin(const SubWidgetInfo &_info, ICroppedRectangle * _parent);
		virtual ~SubSkin();

		void setAlpha(float _alpha);

		virtual void setVisible(bool _visible);

		void _updateView();
		void _correctView();

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);


		virtual void _setUVSet(const FloatRect& _rect);
		virtual void _setStateData(StateInfo * _data);

		virtual void _createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item);
		virtual void _destroyDrawItem();

		// метод для отрисовки себя
		virtual size_t _drawItem(Vertex * _vertex, bool _update);

		// метод для генерации данных из описания xml
		static StateInfo * createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version);

	protected:

		FloatRect mRectTexture;
		bool mEmptyView;

		uint32 mCurrentAlpha;

		FloatRect mCurrentTexture;
		IntCoord mCurrentCoord;

		RenderItem * mRenderItem;

		LayerManager * mManager;

	};

} // namespace MyGUI

#endif // __MYGUI_SUB_SKIN_H__
