/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_GeometryUtility.h"

namespace MyGUI
{

	namespace geometry_utility
	{

		VectorFloatPoint cropPolygon(FloatPoint* _baseVerticiesPos, size_t _size, const IntCoord& _cropRectangle)
		{
			VectorFloatPoint resultVerticiesPos;
			resultVerticiesPos.resize(_size);
			for (size_t i = 0; i < _size; ++i)
			{
				resultVerticiesPos[i] = _baseVerticiesPos[i];
			}

			cropPolygonSide(resultVerticiesPos, _cropRectangle.left, Left);
			cropPolygonSide(resultVerticiesPos, _cropRectangle.right(), Right);
			cropPolygonSide(resultVerticiesPos, _cropRectangle.top, Top);
			cropPolygonSide(resultVerticiesPos, _cropRectangle.bottom(), Bottom);

			return resultVerticiesPos;
		}

		void cropPolygonSide(VectorFloatPoint& _verticies, int _sideCoord, Side _side)
		{
			VectorFloatPoint newVerticies;
			int invert = (_side == Right || _side == Bottom) ? -1 : 1;
			for (size_t i = 0; i < _verticies.size(); ++i)
			{
				FloatPoint& v0 = _verticies[i];
				FloatPoint& v1 = _verticies[(i + 1) % _verticies.size()];
				switch (_side)
				{
				case Left:
				case Right:
					// both inside
					if (invert * v0.left >= invert * _sideCoord && invert * v1.left >= invert * _sideCoord)
						newVerticies.push_back(v0);
					// intersect side (1st vertex in)
					else if (invert * v0.left >= invert * _sideCoord && invert * v1.left < invert * _sideCoord)
					{
						newVerticies.push_back(v0);
						float c = (v0.left - _sideCoord) / (_sideCoord - v1.left);
						newVerticies.push_back(FloatPoint((float)_sideCoord, (v0.top + c * v1.top) / (c + 1)));
					}
					// intersect side (2nd vertex in)
					else if (invert * v0.left <= invert * _sideCoord && invert * v1.left > invert * _sideCoord)
					{
						float c = (v0.left - _sideCoord) / (_sideCoord - v1.left);
						newVerticies.push_back(FloatPoint((float)_sideCoord, (v0.top + c * v1.top) / (c + 1)));
					}
					// else don't add any verticies
					break;
				case Top:
				case Bottom:
					// both inside
					if (invert * v0.top >= invert * _sideCoord && invert * v1.top >= invert * _sideCoord)
						newVerticies.push_back(v0);
					// intersect side (1st vertex in)
					else if (invert * v0.top >= invert * _sideCoord && invert * v1.top < invert * _sideCoord)
					{
						newVerticies.push_back(v0);
						float c = (v0.top - _sideCoord) / (_sideCoord - v1.top);
						newVerticies.push_back(FloatPoint((v0.left + c * v1.left) / (c + 1), (float)_sideCoord));
					}
					// intersect side (2nd vertex in)
					else if (invert * v0.top <= invert * _sideCoord && invert * v1.top > invert * _sideCoord)
					{
						float c = (v0.top - _sideCoord) / (_sideCoord - v1.top);
						newVerticies.push_back(FloatPoint((v0.left + c * v1.left) / (c + 1), (float)_sideCoord));
					}
					// else don't add any verticies
					break;
				}
			}

			_verticies = newVerticies;
		}

		FloatPoint getPositionInsideRect(const FloatPoint& _point, const FloatPoint& _corner0, const FloatPoint& _corner1, const FloatPoint& _corner2)
		{
			FloatPoint result;

			FloatPoint point = _point - _corner0;
			FloatPoint dirX = _corner1 - _corner0;
			FloatPoint dirY = _corner2 - _corner0;

			float div = dirX.left * dirY.top - dirX.top * dirY.left;
			if (div == 0.0f)
				return FloatPoint();
			return FloatPoint(
				(point.top * dirX.left - point.left * dirX.top) / div,
				(point.left * dirY.top - point.top * dirY.left) / div);
		}

		FloatPoint getUVFromPositionInsideRect(const FloatPoint& _point, const FloatPoint& _v0, const FloatPoint& _v1, const FloatPoint& _baseUV)
		{
			return FloatPoint(
				_baseUV.left + _point.left * _v0.left + _point.top * _v1.left,
				_baseUV.top  + _point.left * _v0.top  + _point.top * _v1.top);
		}

	} // namespace geometry_utility

} // namespace MyGUI
