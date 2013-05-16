/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_POLYGONAL_SKIN_H__
#define __MYGUI_POLYGONAL_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_ISubWidgetRect.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{

	class MYGUI_EXPORT PolygonalSkin :
		public ISubWidgetRect
	{
		MYGUI_RTTI_DERIVED( PolygonalSkin )

	public:
		PolygonalSkin();
		virtual ~PolygonalSkin();

		/** Set line points. */
		void setPoints(const std::vector<FloatPoint>& _points);

		/** Set line width in pixels. */
		void setWidth(float _width);

		/** Set line stroke step. 0 to disable stroke. */
		void setStroke(size_t _value);

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
		void _rebuildGeometry();
		FloatPoint _getPerpendicular(const FloatPoint& _point1, const FloatPoint& _point2);
		// line from center of p1-p2 line to p3
		FloatPoint _getMiddleLine(const FloatPoint& _point1, const FloatPoint& _point2, const FloatPoint& _point3);

	private:
		bool mGeometryOutdated;

		float mLineWidth;
		size_t mLineStroke;
		std::vector<FloatPoint> mLinePoints;
		float mLineLength;

		std::vector<FloatPoint> mResultVerticiesPos;
		std::vector<FloatPoint> mResultVerticiesUV;

		size_t mVertexCount;

		bool mEmptyView;

		VertexColourType mVertexFormat;
		uint32 mCurrentColour;

		FloatRect mCurrentTexture;
		IntCoord mCurrentCoord;

		ILayerNode* mNode;
		RenderItem* mRenderItem;
	};

} // namespace MyGUI

#endif // __MYGUI_POLYGONAL_SKIN_H__
