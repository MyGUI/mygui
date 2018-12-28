/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_POLYGONAL_SKIN_H_
#define MYGUI_POLYGONAL_SKIN_H_

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

		/** Set line points. */
		void setPoints(const std::vector<FloatPoint>& _points);

		/** Set line width in pixels. */
		void setWidth(float _width);

		/** Set line stroke step. 0 to disable stroke. */
		void setStroke(size_t _value);

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

#endif // MYGUI_POLYGONAL_SKIN_H_
