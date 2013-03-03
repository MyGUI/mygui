/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_TILE_RECT_H__
#define __MYGUI_TILE_RECT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_ISubWidgetRect.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{

	class RenderItem;

	class MYGUI_EXPORT TileRect :
		public ISubWidgetRect
	{
		MYGUI_RTTI_DERIVED( TileRect )

	public:
		TileRect();
		virtual ~TileRect();

		virtual void setAlpha(float _alpha);

		virtual void setVisible(bool _visible);

		virtual void createDrawItem(ITexture* _texture, ILayerNode* _node);
		virtual void destroyDrawItem();

		// метод для отрисовки себя
		virtual void doRender();

		virtual void setStateData(IStateInfo* _data);

		/*internal:*/
		virtual void _updateView();
		virtual void _correctView();

		virtual void _setAlign(const IntSize& _oldsize);

		virtual void _setUVSet(const FloatRect& _rect);
		virtual void _setColour(const Colour& _value);

	protected:
		bool mEmptyView;

		VertexColourType mVertexFormat;
		uint32 mCurrentColour;

		FloatRect mCurrentTexture;
		IntCoord mCurrentCoord;

		ILayerNode* mNode;
		RenderItem* mRenderItem;

		IntSize mTileSize;
		size_t mCountVertex;

		float mRealTileWidth;
		float mRealTileHeight;

		float mTextureHeightOne;
		float mTextureWidthOne;

		bool mTileH;
		bool mTileV;
	};

} // namespace MyGUI

#endif // __MYGUI_TILE_RECT_H__
