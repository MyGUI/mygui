/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_GEOMETRY_UTILITY_H_
#define MYGUI_GEOMETRY_UTILITY_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_RenderTargetInfo.h"
#include <array>

namespace MyGUI
{

	using VectorFloatPoint = std::vector<FloatPoint>;

	namespace geometry_utility
	{

		VectorFloatPoint cropPolygon(const FloatPoint* _baseVerticiesPos, size_t _size, const IntCoord& _cropRectangle);

		enum Side
		{
			Left,
			Right,
			Top,
			Bottom
		};
		void cropPolygonSide(VectorFloatPoint& _verticies, int _sideCoord, Side _side);

		/** Convert local pixels to render coordinates, accounting for target offsets and reversing Y.
			_origin is the absolute pixel position of the local origin; zero means absolute input coordinates.
		*/
		FloatPoint toRenderTarget(FloatPoint _point, const RenderTargetInfo& _info, IntPoint _origin = {});

		/** Convert rectangle edges without reordering them: top may be greater than bottom in the result. */
		FloatRect toRenderTarget(const FloatRect& _rect, const RenderTargetInfo& _info, IntPoint _origin = {});

		/** Convert an integer rectangle without a temporary point buffer, preserving the floating-point edge calculation. */
		inline FloatRect toRenderTarget(const IntCoord& _coord, const RenderTargetInfo& _info, IntPoint _origin = {})
		{
			const float left = ((_info.pixScaleX * (_origin.left - _info.leftOffset) + _info.hOffset) * 2) - 1;
			const float top = 1 - ((_info.pixScaleY * (_origin.top - _info.topOffset) + _info.vOffset) * 2);
			return {
				left + (float)_coord.left * _info.pixScaleX * 2,
				top - (float)_coord.top * _info.pixScaleY * 2,
				left + (float)_coord.right() * _info.pixScaleX * 2,
				top - (float)_coord.bottom() * _info.pixScaleY * 2};
		}

		/** Convert a point buffer in place using the same pixel coordinates and origin as toRenderTarget. */
		void toRenderTargetInPlace(
			FloatPoint* _points,
			size_t _count,
			const RenderTargetInfo& _info,
			IntPoint _origin = {});

		template<size_t N>
		void toRenderTargetInPlace(FloatPoint (&_points)[N], const RenderTargetInfo& _info, IntPoint _origin = {})
		{
			toRenderTargetInPlace(_points, N, _info, _origin);
		}

		template<size_t N>
		void toRenderTargetInPlace(
			std::array<FloatPoint, N>& _points,
			const RenderTargetInfo& _info,
			IntPoint _origin = {})
		{
			toRenderTargetInPlace(_points.data(), _points.size(), _info, _origin);
		}

		inline void toRenderTargetInPlace(
			VectorFloatPoint& _points,
			const RenderTargetInfo& _info,
			IntPoint _origin = {})
		{
			toRenderTargetInPlace(_points.data(), _points.size(), _info, _origin);
		}

		/** Convert render coordinates to pixels relative to _origin. Pixel scales must be nonzero. */
		FloatPoint fromRenderTarget(FloatPoint _point, const RenderTargetInfo& _info, IntPoint _origin = {});

		// get point position relative to rectangle
		FloatPoint getPositionInsideRect(
			const FloatPoint& _point,
			const FloatPoint& _corner0,
			const FloatPoint& _corner1,
			const FloatPoint& _corner2);

		FloatPoint getUVFromPositionInsideRect(
			const FloatPoint& _point,
			const FloatPoint& _v0,
			const FloatPoint& _v1,
			const FloatPoint& _baseUV);

	} // namespace geometry_utility
} // namespace MyGUI

#endif // MYGUI_GEOMETRY_UTILITY_H_
