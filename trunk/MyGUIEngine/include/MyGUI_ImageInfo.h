/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_IMAGE_INFO_H__
#define __MYGUI_IMAGE_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	struct ImageItem
	{
		ImageItem() : frame_rate(0) {}

		float frame_rate;
		std::vector<FloatRect> images;
	};

	typedef std::vector<ImageItem> VectorImages;

	struct ImageIndexInfo
	{
		ImageIndexInfo(
			const std::string & _texture,
			const IntSize & _size,
			const float _rate,
			const std::vector<IntPoint> & _frames
			) :
			texture(_texture),
			size(_size),
			rate(_rate),
			frames(_frames)
		{
		}

		const std::string & texture;
		const IntSize & size;
		const float rate;
		const std::vector<IntPoint> & frames;
	};

} // namespace MyGUI

#endif // __MYGUI_IMAGE_INFO_H__
