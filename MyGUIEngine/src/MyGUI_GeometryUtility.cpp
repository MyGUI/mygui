/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_GeometryUtility.h"

namespace MyGUI::geometry_utility
{

	VectorFloatPoint cropPolygon(const FloatPoint* _baseVerticiesPos, size_t _size, const IntCoord& _cropRectangle)
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
		VectorFloatPoint result;
		result.reserve(_verticies.size() + 1);
		const bool horizontal = _side == Left || _side == Right;
		const float direction = (_side == Right || _side == Bottom) ? -1.0f : 1.0f;
		for (size_t i = 0; i < _verticies.size(); ++i)
		{
			const FloatPoint& a = _verticies[i];
			const FloatPoint& b = _verticies[(i + 1) % _verticies.size()];
			const float distanceA = direction * ((horizontal ? a.left : a.top) - _sideCoord);
			const float distanceB = direction * ((horizontal ? b.left : b.top) - _sideCoord);
			if (distanceA >= 0)
				result.push_back(a);
			// A vertex on the boundary is already emitted by its own edge.
			if ((distanceA > 0 && distanceB < 0) || (distanceA < 0 && distanceB > 0))
			{
				const float t = distanceA / (distanceA - distanceB);
				FloatPoint intersection(a.left + t * (b.left - a.left), a.top + t * (b.top - a.top));
				if (horizontal)
					intersection.left = (float)_sideCoord;
				else
					intersection.top = (float)_sideCoord;
				result.push_back(intersection);
			}
		}
		_verticies.swap(result);
	}

	void toRenderTarget(FloatPoint* _points, size_t _count, const IntPoint& _origin, const RenderTargetInfo& _info)
	{
		const float left = ((_info.pixScaleX * (_origin.left - _info.leftOffset) + _info.hOffset) * 2) - 1;
		const float top = 1 - ((_info.pixScaleY * (_origin.top - _info.topOffset) + _info.vOffset) * 2);
		for (size_t i = 0; i < _count; ++i)
		{
			_points[i].left = left + _points[i].left * _info.pixScaleX * 2;
			_points[i].top = top - _points[i].top * _info.pixScaleY * 2;
		}
	}

	FloatPoint getPositionInsideRect(
		const FloatPoint& _point,
		const FloatPoint& _corner0,
		const FloatPoint& _corner1,
		const FloatPoint& _corner2)
	{
		FloatPoint point = _point - _corner0;
		FloatPoint dirX = _corner1 - _corner0;
		FloatPoint dirY = _corner2 - _corner0;

		float div = dirX.left * dirY.top - dirX.top * dirY.left;
		if (div == 0.0f)
			return {};
		return {
			(point.top * dirX.left - point.left * dirX.top) / div,
			(point.left * dirY.top - point.top * dirY.left) / div};
	}

	FloatPoint getUVFromPositionInsideRect(
		const FloatPoint& _point,
		const FloatPoint& _v0,
		const FloatPoint& _v1,
		const FloatPoint& _baseUV)
	{
		return {
			_baseUV.left + _point.left * _v0.left + _point.top * _v1.left,
			_baseUV.top + _point.left * _v0.top + _point.top * _v1.top};
	}

} // namespace MyGUI
