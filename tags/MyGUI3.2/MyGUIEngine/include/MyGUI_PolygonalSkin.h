/*!
	@file
	@author		George Evmenov
	@date		07/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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
