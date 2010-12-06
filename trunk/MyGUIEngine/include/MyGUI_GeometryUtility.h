/*!
	@file
	@author		George Evmenov
	@date		12/2010
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
#ifndef __MYGUI_GEOMETRY_UTILITY_H__
#define __MYGUI_GEOMETRY_UTILITY_H__

#include "MyGUI_Prerequest.h"

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

#endif // __MYGUI_GEOMETRY_UTILITY_H__
