/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_MASK_PICK_INFO_H__
#define __MYGUI_MASK_PICK_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	class MYGUI_EXPORT MaskPickInfo
	{
	public:
		MaskPickInfo() : width(0), height(0) { }

		bool load(const std::string& _file);

		bool pick(const IntPoint& _point, const IntCoord& _coord) const
		{
			if ((0 == _coord.width) || (0 == _coord.height)) return false;

			int x = ((_point.left * width)-1) / _coord.width;
			int y = ((_point.top * height)-1) / _coord.height;

			return 0 != data[y * width + x];
		}

		bool empty() const
		{
			return data.empty();
		}

	private:
		std::vector<char> data;
		int width, height;
	};

} // namespace MyGUI

#endif // __MYGUI_MASK_PICK_INFO_H__
