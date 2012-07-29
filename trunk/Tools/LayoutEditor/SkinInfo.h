#ifndef _6d228d6b_2fdf_4a5e_9809_ccde42293e68_
#define _6d228d6b_2fdf_4a5e_9809_ccde42293e68_

#include <MyGUI.h>

namespace tools
{
	struct SkinInfo
	{
		SkinInfo(const std::string& _widget_skin, const std::string& _widget_type, const std::string& _widget_button_name) :
			widget_skin(_widget_skin),
			widget_type(_widget_type),
			widget_button_name(_widget_button_name)
		{
		}

		std::string widget_skin;
		std::string widget_type;
		std::string widget_button_name;
	};

	typedef std::vector<SkinInfo> VectorSkinInfo;
	typedef std::map<std::string, VectorSkinInfo> SkinGroups;

}

#endif
