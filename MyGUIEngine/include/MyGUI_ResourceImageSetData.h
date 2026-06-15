/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_RESOURCE_IMAGE_SET_DATA_H_
#define MYGUI_RESOURCE_IMAGE_SET_DATA_H_

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	struct IndexImage
	{
		std::string name;
		float rate{0};
		std::vector<IntPoint> frames;
	};
	using VectorIndexImage = std::vector<IndexImage>;

	struct GroupImage
	{
		std::string name;
		std::string texture;
		IntSize size;
		VectorIndexImage indexes;
	};
	using VectorGroupImage = std::vector<GroupImage>;
	using EnumeratorGroupImage = Enumerator<VectorGroupImage>;

} // namespace MyGUI

#endif // MYGUI_RESOURCE_IMAGE_SET_DATA_H_
