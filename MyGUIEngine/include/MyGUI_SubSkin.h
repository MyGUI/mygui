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

	class MYGUI_EXPORT SubSkin : public ISubWidgetRect
	{
		MYGUI_RTTI_DERIVED(SubSkin)

	public:
		SubSkin();

		void setAlpha(float _alpha) override;

		void setVisible(bool _visible) override;

		void setStateData(IStateInfo* _data) override;

		void createDrawItem(ITexture* _texture, ILayerNode* _node) override;
		void destroyDrawItem() override;

		// метод для отрисовки себя
		void doRender() override;

		/*internal:*/
		void _updateView() override;
		void _correctView() override;

		void _setAlign(const IntSize& _oldsize) override;

		void _setUVSet(const FloatRect& _rect) override;
		void _setColour(const Colour& _value) override;

	protected:
		FloatRect mRectTexture;
		bool mEmptyView{false};

		VertexColourType mVertexFormat;
		uint32 mCurrentColour{0xFFFFFFFF};

		FloatRect mCurrentTexture;
		IntCoord mCurrentCoord;

		ILayerNode* mNode{nullptr};
		RenderItem* mRenderItem{nullptr};

		bool mSeparate{false};
	};

} // namespace MyGUI

#endif // MYGUI_SUB_SKIN_H_
