/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_COORD_CONVERTER_H_
#define MYGUI_COORD_CONVERTER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include <tuple>

namespace MyGUI
{

	class MYGUI_EXPORT CoordConverter
	{
	public:
		/** Convert pixel coordinates to texture UV coordinates */
		static FloatRect convertTextureCoord(const IntCoord& _coord, const IntSize& _textureSize);

		/* Convert from relative to pixel coordinates.
			@param _coord relative coordinates.
		*/
		static IntCoord convertFromRelative(const FloatCoord& _coord, const IntSize& _view);

		/* Convert from relative to pixel coordinates.
			@param _coord relative coordinates.
		*/
		static IntSize convertFromRelative(const FloatSize& _size, const IntSize& _view);

		/* Convert from relative to pixel coordinates.
			@param _coord relative coordinates.
		*/
		static IntPoint convertFromRelative(const FloatPoint& _point, const IntSize& _view);

		/* Convert from pixel to relative coordinates.
			@param _coord pixel coordinates.
		*/
		static FloatCoord convertToRelative(const IntCoord& _coord, const IntSize& _view);

		static FloatSize convertToRelative(const IntSize& _size, const IntSize& _view);

		static FloatPoint convertToRelative(const IntPoint& _point, const IntSize& _view);

		static IntCoord convertFromRelative(const DoubleCoord& _coord, const IntSize& _view);

		static DoubleCoord convertToRelativeD(const IntCoord& _coord, const IntSize& _view);

		static std::tuple<IntCoord, bool, bool> applyAlign(
			Align _align,
			const IntCoord& _coord,
			const IntSize& _parentSize,
			const IntSize& _oldsize);
	};

} // namespace MyGUI

#endif // MYGUI_COORD_CONVERTER_H_
