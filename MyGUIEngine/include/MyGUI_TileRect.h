/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TILE_RECT_H_
#define MYGUI_TILE_RECT_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_ISubWidgetRect.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{

	class RenderItem;

	class MYGUI_EXPORT TileRect : public ISubWidgetRect
	{
		MYGUI_RTTI_DERIVED(TileRect)

	public:
		TileRect();

		void setAlpha(float _alpha) override;

		void setVisible(bool _visible) override;

		void createDrawItem(ITexture* _texture, ILayerNode* _node) override;
		void destroyDrawItem() override;

		// метод для отрисовки себя
		void doRender() override;

		void setStateData(IStateInfo* _data) override;

		/*internal:*/
		void _updateView() override;
		void _correctView() override;

		void _setAlign(const IntSize& _oldsize) override;

		void _setUVSet(const FloatRect& _rect) override;
		void _setColour(const Colour& _value) override;

	protected:
		bool mEmptyView{false};

		VertexColourType mVertexFormat;
		uint32 mCurrentColour{0xFFFFFFFF};

		FloatRect mCurrentTexture;
		IntCoord mCurrentCoord;

		ILayerNode* mNode{nullptr};
		RenderItem* mRenderItem{nullptr};

		IntSize mTileSize;
		size_t mCountVertex;

		float mRealTileWidth{0};
		float mRealTileHeight{0};

		float mTextureHeightOne{0};
		float mTextureWidthOne{0};

		bool mTileH{true};
		bool mTileV{true};
	};

} // namespace MyGUI

#endif // MYGUI_TILE_RECT_H_
