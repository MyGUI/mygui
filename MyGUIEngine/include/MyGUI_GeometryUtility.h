/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_GEOMETRY_UTILITY_H_
#define MYGUI_GEOMETRY_UTILITY_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	typedef std::vector<FloatPoint> VectorFloatPoint;

	namespace geometry_utility
	{
		VectorFloatPoint cropPolygon(FloatPoint* _baseVerticiesPos, size_t _size, const IntCoord& _cropRectangle);

		enum Side {Left, Right, Top, Bottom};
		void cropPolygonSide(VectorFloatPoint& _verticies, int _sideCoord, Side _side);

		// get point position relative to rectangle
		FloatPoint getPositionInsideRect(const FloatPoint& _point, const FloatPoint& _corner0, const FloatPoint& _corner1, const FloatPoint& _corner2);

		FloatPoint getUVFromPositionInsideRect(const FloatPoint& _point, const FloatPoint& _v0, const FloatPoint& _v1, const FloatPoint& _baseUV);

	} // namespace geometry_utility

} // namespace MyGUI

#endif // MYGUI_GEOMETRY_UTILITY_H_
