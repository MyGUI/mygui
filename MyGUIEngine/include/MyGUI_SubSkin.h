/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SUB_SKIN_H_
#define MYGUI_SUB_SKIN_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ISubWidgetRect.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IStateInfo.h"

namespace MyGUI
{

	class RenderItem;

	class MYGUI_EXPORT SubSkin :
		public ISubWidgetRect
	{
		MYGUI_RTTI_DERIVED( SubSkin )

	public:
		SubSkin();
		virtual ~SubSkin();

		virtual void setAlpha(float _alpha);

		virtual void setVisible(bool _visible);

		virtual void setStateData(IStateInfo* _data);

		virtual void createDrawItem(ITexture* _texture, ILayerNode* _node);
		virtual void destroyDrawItem();

		// метод для отрисовки себя
		virtual void doRender();

		/*internal:*/
		virtual void _updateView();
		virtual void _correctView();

		virtual void _setAlign(const IntSize& _oldsize);

		virtual void _setUVSet(const FloatRect& _rect);
		virtual void _setColour(const Colour& _value);

	protected:
		FloatRect mRectTexture;
		bool mEmptyView;

		VertexColourType mVertexFormat;
		uint32 mCurrentColour;

		FloatRect mCurrentTexture;
		IntCoord mCurrentCoord;

		ILayerNode* mNode;
		RenderItem* mRenderItem;

		bool mSeparate;
	};

} // namespace MyGUI

#endif // MYGUI_SUB_SKIN_H_
