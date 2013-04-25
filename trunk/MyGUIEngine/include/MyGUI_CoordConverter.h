/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_COORD_CONVERTER_H__
#define __MYGUI_COORD_CONVERTER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class MYGUI_EXPORT CoordConverter
	{
	public:
		/** Convert pixel coordinates to texture UV coordinates */
		static FloatRect convertTextureCoord(const IntCoord& _coord, const IntSize& _textureSize)
		{
			if (!_textureSize.width || !_textureSize.height) return FloatRect();
			return FloatRect(
				(float)_coord.left / (float)_textureSize.width,
				(float)_coord.top / (float)_textureSize.height,
				(float)_coord.right() / (float)_textureSize.width,
				(float)_coord.bottom() / (float)_textureSize.height);
		}

		/* Convert from relative to pixel coordinates.
			@param _coord relative coordinates.
		*/
		static IntCoord convertFromRelative(const FloatCoord& _coord, const IntSize& _view)
		{
			return IntCoord(int(_coord.left * _view.width), int(_coord.top * _view.height), int(_coord.width * _view.width), int(_coord.height * _view.height));
		}

		/* Convert from relative to pixel coordinates.
			@param _coord relative coordinates.
		*/
		static IntSize convertFromRelative(const FloatSize& _size, const IntSize& _view)
		{
			return IntSize(int(_size.width * _view.width), int(_size.height * _view.height));
		}

		/* Convert from relative to pixel coordinates.
			@param _coord relative coordinates.
		*/
		static IntPoint convertFromRelative(const FloatPoint& _point, const IntSize& _view)
		{
			return IntPoint(int(_point.left * _view.width), int(_point.top * _view.height));
		}

		/* Convert from pixel to relative coordinates.
			@param _coord pixel coordinates.
		*/
		static FloatCoord convertToRelative(const IntCoord& _coord, const IntSize& _view)
		{
			return FloatCoord(_coord.left / (float)_view.width, _coord.top / (float)_view.height, _coord.width / (float)_view.width, _coord.height / (float)_view.height);
		}

		static FloatSize convertToRelative(const IntSize& _size, const IntSize& _view)
		{
			return FloatSize(_size.width / (float)_view.width, _size.height / (float)_view.height);
		}

		static FloatPoint convertToRelative(const IntPoint& _point, const IntSize& _view)
		{
			return FloatPoint(_point.left / (float)_view.width, _point.top / (float)_view.height);
		}
	};

} // namespace MyGUI

#endif // __MYGUI_COORD_CONVERTER_H__
