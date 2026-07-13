/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_CoordConverter.h"

namespace MyGUI
{

	FloatRect CoordConverter::convertTextureCoord(const IntCoord& _coord, const IntSize& _textureSize)
	{
		if (!_textureSize.width || !_textureSize.height)
			return {};
		return {
			(float)_coord.left / (float)_textureSize.width,
			(float)_coord.top / (float)_textureSize.height,
			(float)_coord.right() / (float)_textureSize.width,
			(float)_coord.bottom() / (float)_textureSize.height};
	}

	IntCoord CoordConverter::convertFromRelative(const FloatCoord& _coord, const IntSize& _view)
	{
		return {
			int(_coord.left * _view.width),
			int(_coord.top * _view.height),
			int(_coord.width * _view.width),
			int(_coord.height * _view.height)};
	}

	IntSize CoordConverter::convertFromRelative(const FloatSize& _size, const IntSize& _view)
	{
		return {int(_size.width * _view.width), int(_size.height * _view.height)};
	}

	IntPoint CoordConverter::convertFromRelative(const FloatPoint& _point, const IntSize& _view)
	{
		return {int(_point.left * _view.width), int(_point.top * _view.height)};
	}

	FloatCoord CoordConverter::convertToRelative(const IntCoord& _coord, const IntSize& _view)
	{
		return {
			_coord.left / (float)_view.width,
			_coord.top / (float)_view.height,
			_coord.width / (float)_view.width,
			_coord.height / (float)_view.height};
	}

	FloatSize CoordConverter::convertToRelative(const IntSize& _size, const IntSize& _view)
	{
		return {_size.width / (float)_view.width, _size.height / (float)_view.height};
	}

	FloatPoint CoordConverter::convertToRelative(const IntPoint& _point, const IntSize& _view)
	{
		return {_point.left / (float)_view.width, _point.top / (float)_view.height};
	}

	IntCoord CoordConverter::convertFromRelative(const DoubleCoord& _coord, const IntSize& _view)
	{
		return {
			int(_coord.left * _view.width),
			int(_coord.top * _view.height),
			int(_coord.width * _view.width),
			int(_coord.height * _view.height)};
	}

	DoubleCoord CoordConverter::convertToRelativeD(const IntCoord& _coord, const IntSize& _view)
	{
		return {
			_coord.left / (double)_view.width,
			_coord.top / (double)_view.height,
			_coord.width / (double)_view.width,
			_coord.height / (double)_view.height};
	}

	std::tuple<IntCoord, bool, bool> CoordConverter::applyAlign(
		Align _align,
		const IntCoord& _coord,
		const IntSize& _parentSize,
		const IntSize& _oldsize)
	{
		IntCoord result = _coord;
		bool resize = false;
		bool move = false;

		if (_align.isHStretch())
		{
			result.width = _coord.width + (_parentSize.width - _oldsize.width);
			resize = true;
		}
		else if (_align.isRight())
		{
			result.left = _coord.left + (_parentSize.width - _oldsize.width);
			move = true;
		}
		else if (_align.isHCenter())
		{
			result.left = (_parentSize.width - _coord.width) / 2;
			move = true;
		}

		if (_align.isVStretch())
		{
			result.height = _coord.height + (_parentSize.height - _oldsize.height);
			resize = true;
		}
		else if (_align.isBottom())
		{
			result.top = _coord.top + (_parentSize.height - _oldsize.height);
			move = true;
		}
		else if (_align.isVCenter())
		{
			result.top = (_parentSize.height - _coord.height) / 2;
			move = true;
		}

		return {result, resize, move};
	}

} // namespace MyGUI
