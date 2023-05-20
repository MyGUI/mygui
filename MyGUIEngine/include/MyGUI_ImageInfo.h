/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_IMAGE_INFO_H_
#define MYGUI_IMAGE_INFO_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	struct ImageItem
	{
	public:
		ImageItem() :
			frame_rate(0)
		{
		}

	public:
		float frame_rate;
		std::vector<FloatRect> images;
	};

	typedef std::vector<ImageItem> VectorImages;

	struct MYGUI_EXPORT ImageIndexInfo
	{
	public:
		ImageIndexInfo(std::string_view _texture, const IntSize& _size, const float _rate, const std::vector<IntPoint>& _frames) :
			texture(_texture),
			size(_size),
			rate(_rate),
			frames(_frames)
		{
		}

	public:
		std::string_view texture;
		const IntSize& size;
		const float rate;
		const std::vector<IntPoint>& frames;
	};

} // namespace MyGUI

#endif // MYGUI_IMAGE_INFO_H_
