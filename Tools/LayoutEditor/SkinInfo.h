#ifndef _6d228d6b_2fdf_4a5e_9809_ccde42293e68_
#define _6d228d6b_2fdf_4a5e_9809_ccde42293e68_

#include <MyGUI.h>

namespace tools
{
	struct SkinInfo
	{
		SkinInfo(std::string_view _widget_skin, std::string_view _widget_type, std::string_view _widget_button_name) :
			widget_skin(_widget_skin),
			widget_type(_widget_type),
			widget_button_name(_widget_button_name)
		{
		}

		std::string widget_skin;
		std::string widget_type;
		std::string widget_button_name;
	};

	using VectorSkinInfo = std::vector<SkinInfo>;
	using SkinGroups = std::map<std::string, VectorSkinInfo, std::less<>>;

}

#endif
