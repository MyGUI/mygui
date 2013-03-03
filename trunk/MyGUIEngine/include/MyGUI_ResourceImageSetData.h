/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_RESOURCE_IMAGE_SET_DATA_H__
#define __MYGUI_RESOURCE_IMAGE_SET_DATA_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	struct IndexImage
	{
		IndexImage() :
			rate(0)
		{
		}

		std::string name;
		float rate;
		std::vector<IntPoint> frames;
	};
	typedef std::vector<IndexImage> VectorIndexImage;

	struct GroupImage
	{
		std::string name;
		std::string texture;
		IntSize size;
		VectorIndexImage indexes;
	};
	typedef std::vector<GroupImage> VectorGroupImage;
	typedef Enumerator<VectorGroupImage> EnumeratorGroupImage;

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_IMAGE_SET_DATA_H__
